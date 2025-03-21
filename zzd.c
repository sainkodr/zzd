/* zzd.c - encode/decode current date in unordered base 26 */
/* BUILD: cc -o zzd zzd.c */
/* ENCODE: zzd */
/* DECODE: zzd <code> */
/*************************************************************************************************/
#include <stdio.h>
#include <time.h>
/*************************************************************************************************/
#define R_A 73530
#define R_B 26581
#define R_C 90017
/*************************************************************************************************/
static int get_digit(long id, int index) {
  int power;
  /***********************************************/
  power = 1;
  while (index > 0) {
    power *= 26;
    --index;
  }
  /***********************************************/
  return 'A' + (int)(id % (power * 26)) / power;
}
/*************************************************************************************************/
static void encode(void) {
  time_t t;
  const struct tm *tm;
  long i, id, n;
  /***********************************************/
  t = time(NULL);
  tm = localtime(&t);
  n = (tm->tm_year - 125) * 366 + tm->tm_yday;
  id = 0;
  
  for (i = 0; i <= n; ++i) {
    do {
      id = (R_A * id + R_B) % R_C;
    }
    while (id >= 26 * 26 * 26);
  }
  /***********************************************/
  printf("%c%c%c\n", get_digit(id, 2), get_digit(id, 1), get_digit(id, 0));
}
/*************************************************************************************************/
static int decode(char c1, char c2, char c3) {
  static char MT[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  /***********************************************/
  long n, id, y, m, d, id0;
  /***********************************************/
  n = 0;
  id = 0;
  id0 = ((c1 - 'A') * 26 * 26) + ((c2 - 'A') * 26) + (c3 - 'A');
  
  for (;;) {
    do {
      id = (R_A * id + R_B) % R_C;
    }
    while (id >= 26 * 26 * 26);
    
    if (id == id0) {
      break;
    }
    
    ++n;
  }
  /***********************************************/
  y = 2025 + n / 366;
  d = n % 366;
  
  if (y % 4 == 0) {
    MT[1] = 29;
  }
  else if (d == 365) {
    return printf("Impossible day\n");
  }
  
  for (m = 1; d >= MT[m]; ++m) {
    d -= MT[m];
  }
  
  ++d;
  /***********************************************/
  return printf("%i.%02i.%02i\n", y, m, d);
}
/*************************************************************************************************/
int main(int argc, char **argv) {
  if (argc < 2) {
    encode();
  }
  else {
    decode(argv[1][0], argv[1][1], argv[1][2]);
  }
  return 0;
}

