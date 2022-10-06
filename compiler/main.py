
from funcs import *

newline = "\n"
tnewline = "\\n"

file = input("File > ")

mem = input("memo size > ")

if int(mem) < 1:
    print("memo size too small, set to smallest of 1")
    print("this is a limit of our threading implementation")
    mem = "1"

with open(file, "r") as fp:
    data = fp.read()
    final = parse(data)
    generated = gen(final, mem)

    all_memos = generated[2]



    out = input("output file (without extension) > ")

    with open(out, "w") as fp2:
        fp2.write(generated[0])

    with open(out + ".example-main", "w") as fp2:
        fp2.write(f'''
        #include <stdio.h>
        #include <string.h>
        #include <stdlib.h>


        {f"{newline}".join(f"double {y}( { ','.join('double' for _ in x)  } );" for y, x in all_memos)};
        {"".join(f"void {x}(void);" for x in generated[1])}
        
        int main(void) {{

          while(1) {{
            char buf[100];
            memset(buf, 0, 100);
            printf("func name: ");
            scanf("%s", buf);

            if (strcmp(buf, "exit") == 0) {{break;}}

        
            { "".join(list(map(lambda x: f"""

            if (strcmp("{x[0]}", buf) == 0) {{
              {"".join(list(map(lambda y: f"double {y}; ", x[1])))}
              printf("{len(x[1])} args > ");
              scanf("{("%lf " * len(x[1])).strip()}", {",".join(map(lambda y: f"&{y}", x[1]))});

              double res = {x[0]}({",".join(x[1])});

              printf("{tnewline}res> %.16f{tnewline}", res);
         
                     }}
        fflush(stdout);

        if (strcmp(buf, "") == 0) {{break;}}
        
        """, all_memos)))  }
		

        }}
           {"".join(list(map(lambda x: f'{x}();{newline}', generated[1])))}
        }}
        ''')
    import os
    os.system(f"clang-format {out} > ___MEMO_TEMP")
    os.system(f"clang-format {out}.example-main > ___MEMO_TEMP_MAIN")
    os.system(f"mv ___MEMO_TEMP {out}.c")
    os.system(f"mv ___MEMO_TEMP_MAIN {out}.example-main.c")
    os.system(f"rm {out}")
    os.system(f"rm {out}.example-main")
