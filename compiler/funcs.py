tnewline = "\n"

class Function:
    def __init__(self, name):
        self.name = name
        self.memo_name = "___MEMO_" + name
        self.cache = self.memo_name + "_cache"
        self.struct = self.memo_name + "_T"
        
        self.body = ""
        self.args = []
    def set_body(self, body):
        self.body = body

    def add_arg(self, arg):
        self.args += arg

    def prelude(self, l):
        return f'''
        #ifndef ___MEMO_STDLIB
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/mman.h>
        #include <math.h>
        #include <time.h>
        #include <string.h>
        #include <unistd.h>
        #define ___MEMO_LEN {l}
        #define ___MEMO_STDLIB
        #endif

        double {self.name}(
            {", ".join(
                ["double" for _ in 
                    range(len(self.args))
                ])
            }
        );

        '''

    def gen_struct(self):
        return f'''
        typedef struct {self.struct} <<
            {"".join(list(map(lambda x: f"double {x};{tnewline}", self.args)))}
            long _hits;
            double _val;
        >> {self.struct};
        '''.replace(">>", "}").replace("<<", "{")

    def gen_memoized(self):
        LBRACK = "{"
        RBRACK = "}"
        tnewline = "\\n"
        return f'''
        double {self.memo_name}({",".join(list(map(lambda x: f"double {x}", self.args)))})
        {self.body}


        {self.struct} * {self.cache} = NULL;
        
        double {self.name}({",".join(list(map(lambda x: f"double {x}", self.args)))})
        <<
            if ({self.cache} == NULL) <<
                {self.cache} = mmap(0, ___MEMO_LEN * sizeof(*{self.cache}),
                                    PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED,
                                    -1, 0);
                if (!{self.cache}) <<printf("alloc fail{tnewline}");exit(1);>>

                memset({self.cache}, -1, ___MEMO_LEN * sizeof(*{self.cache}));
        
            >>

            if ({self.cache}[0]._hits == -1) <<
            if (fork() == 0) <<
        // loading must be done :despair:
        {self.cache}[0]._hits = 0;
                if (access("{self.cache}", F_OK) == 0) <<
                    FILE * fp = fopen("{self.cache}", "r");
                    if (!fp) {{
                    // Rational: If we can't open the file, pretend it doesn't exist
                        goto cantaccess;
                    }}
                    fread({self.cache},
                          ___MEMO_LEN * sizeof(*{self.cache}), 1, fp);
                    fclose(fp);
                >>
               else <<
                    cantaccess:
        memset({self.cache},
                              0, ___MEMO_LEN * sizeof(*{self.cache}));
               >>
        exit(0);
            >>
            else {{
                usleep(2);
            }}
            >>


            if ({self.cache}[0]._hits == -1) <<
              return {self.memo_name}({",".join(self.args)});
            >>
        
            for(int i = 0; i < ___MEMO_LEN; i++) <<
              {"".join(list(map(lambda x: f"if ({self.cache}[i].{x} == {x}) {LBRACK}", self.args)))}
              {self.cache}[i]._hits += 1;
              return {self.cache}[i]._val;
              {RBRACK * len(self.args)}
            >>

            double temp = {self.memo_name}({",".join(self.args)});
            for(int i = 0; i < ___MEMO_LEN; i++) <<
              if ({self.cache}[i]._hits == 0) <<
                {"".join(list(map(lambda x: f"{self.cache}[i].{x} = {x};", self.args)))}
                {self.cache}[i]._hits += 1;
                {self.cache}[i]._val = temp;
                return temp;
              >>
            >>
        >>
        
        '''.replace(">>", "}").replace("<<", "{")

    def save_func(self):
        return f'''

        void {self.memo_name}_save(void) <<
          FILE * fp = fopen("{self.cache}", "w");
          fwrite({self.cache}, ___MEMO_LEN * sizeof(*{self.cache}), 1, fp);
          fclose(fp);
          munmap({self.cache}, ___MEMO_LEN * sizeof(*{self.cache}));
        >>
        
        '''.replace(">>", "}").replace("<<", "{")
    
def parse(inp):
    import re
    import copy
    NAME = 0
    ARGS = 1
    BODY = 3

    STATE = NAME
    count = 0

    cur = None
    bod = ""
    end = []
    
    for tok in re.split("\s+|\n+|\r+", inp):
        if STATE == NAME:
            if re.match("[A-Za-z][a-zA-Z0-9_]*", tok):
                cur = Function(tok)
                STATE = ARGS
            else:
                if tok != "":
                    print(tok, "is invalid name")

        elif STATE == ARGS:
            if re.match("[A-Za-z][a-zA-Z0-9_]*", tok):
                cur.add_arg(tok)
            elif tok == "{":
                bod += tok
                count += 1
                STATE = BODY
            else:
                print(tok, "is invalid arg")

        elif STATE == BODY:
            if tok == "{":
                count += 1
            elif tok == "}":
                count -= 1
            if tok == "}" and count == 0:
                STATE = NAME
                bod += tok
                cur.set_body(bod)

                end += [copy.copy(cur)]
                bod = ""
            else:
                bod += tok + " "
    return end

def gen(fl, sz):
    all_saves = []
    all_memos = []
    end = ""
    for f in fl:
        end += f.prelude(int(sz))
        end += f.gen_struct()
        end += f.gen_memoized()
        end += f.save_func()
        all_saves += [f"{f.memo_name}_save"]
        all_memos += [(f"{f.name}", f.args)]
    return (end, all_saves, all_memos)
