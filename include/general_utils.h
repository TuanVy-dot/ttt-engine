#ifndef GENERAL_UTILS

#define GENERAL_UTILS

#define II_INRANGE(x, min, max) (x >= min && x <= max)
#define IX_INRANGE(x, min, max) (x >= min && x < max)
#define XI_INRANGE(x, min, max) (x > min && x <= max)
#define XX_INRANGE(x, min, max) (x > min && x < max)

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#endif
