#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

int
strncmp(const char *p, const char *q, uint n)
{
 while(n > 0 && *p && *p == *q)
   n--, p++, q++;
 if(n == 0)
   return 0;
 return (uchar)*p - (uchar)*q;
}

char*
strdup(const char *s)
{
  char *d = (char *) malloc(strlen((char *)s) + 1);
  if (d != 0) strcpy (d, (char *) s);
  return d;
}

/* https://android.googlesource.com/platform/bionic/+/6861c6f/libc/string/strsep.c */
char *
strsep(char **stringp, const char *delim)
{
  char *s;
  const char *spanp;
  int c, sc;
  char *tok;
  if ((s = *stringp) == 0)
    return 0;
  for (tok = s;;) {
    c = *s++;
    spanp = delim;
    do {
      if ((sc = *spanp++) == c) {
        if (c == 0)
          s = 0;
        else
          s[-1] = 0;
        *stringp = s;
        return (tok);
      }
    } while (sc != 0);
  }
}

// https://github.com/NewbiZ/xv6/blob/master/distrib/ulibc/src/string/strcat.c
char*
strcat(char* s1, const char* s2)
{
  char* b = s1;

  while (*s1) ++s1;
  while (*s2) *s1++ = *s2++;
  *s1 = 0;

  return b;
}

static void
putc_buf(char *buf, char c)
{
  char a[2];
  a[0] = c;
  a[1] = '\0';
  strcat(buf, a);
}

static void
sprintint(char *b, int xx, int base, int sgn)
{
  static char digits[] = "0123456789ABCDEF";
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
  }

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);
  if(neg)
    buf[i++] = '-';

  while(--i >= 0)
    putc_buf(b, buf[i]);
}

// Print to the given buf. Only understands %d, %x, %p, %s.
void
sprintf(char *buf, char *fmt, ...)
{
  char *s;
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
      } else {
        putc_buf(buf, c);
      }
    } else if(state == '%'){
      if(c == 'd'){
        sprintint(buf, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
        sprintint(buf, *ap, 16, 0);
        ap++;
      } else if(c == 's'){
        s = (char*)*ap;
        ap++;
        if(s == 0)
          s = "(null)";
        while(*s != 0){
          putc_buf(buf, *s);
          s++;
        }
      } else if(c == 'c'){
        putc_buf(buf, *ap);
        ap++;
      } else if(c == '%'){
        putc_buf(buf, c);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc_buf(buf, '%');
        putc_buf(buf, c);
      }
      state = 0;
    }
  }
}

int dup2(int src, int des)
{
  close(des);
  dup(src);
  return src;
}
