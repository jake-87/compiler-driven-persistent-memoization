diff x y {
  if (x > y) {
    return x - y;
  }
  return y - x;
}

mysqrt n {
  double i = 1, count = n;
  usleep(((long)n % 64) * 10);
  while (diff(i*i, n) > 0.001) {
    if (i*i > n) {
      i -= count * 0.5;
    } else {
      i += count * 0.5;
    }
    count = count / 2;
    if (count == 0) {
      if (diff(i*i, n) < 0.5) {
        return i;
      }
      count = 1;
    }
  }
  return i;
}
