/* $Id: mcchrono.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
/* #include <time.h> */
#include <sys/time.h>

#ifdef HP
struct timeval {
  uint32_t	tv_sec;		/* seconds */
  int32_t		tv_usec;	/* and microseconds */
};

struct timezone {
  int32_t	tz_minuteswest;	/* minutes west of Greenwich */
  int32_t	tz_dsttime;	/* type of dst correction */
};
#endif

typedef struct timeval chrono;


#define PERF_DIR "/perf/"
#define PERF_EXT ".perf"

/* ============== */
/* prototypes for mcchrono.c */
/* ============== */

extern int32_t usecs();

extern void start_chrono(
  chrono *tp
);

extern int32_t read_chrono(
  chrono *tp
);

extern void save_time(
  int32_t n,
  int32_t t,
  char *imagename,
  char *funcname
);

extern void save_time2(
  int32_t n,
  int32_t n2,
  int32_t t,
  char *imagename,
  char *funcname
);

extern void save_time4(
  int32_t n,
  int32_t n2,
  int32_t na,
  int32_t na2,
  int32_t t,
  char *imagename,
  char *funcname
);

