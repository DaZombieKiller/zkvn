#ifndef INC_UTIL_MATH_H_
#define INC_UTIL_MATH_H_

#include "defs.h"

#define TIME_SEC	35
#define TIME_MIN	(35*60)
#define TIME_HOUR	(TIME_MIN*60)
#define TIME_DAY	(TIME_HOUR*24)

fixed FAbs(fixed);
fixed AbsSine(fixed, int);
fixed Sine(fixed, int);
int Round(fixed);
int Distance(int, int);
fixed FDistance(int, int);
int ReturnPercentage(int, int);
fixed FReturnPercentage(fixed, fixed);
int ReturnPercentageMinMax(int, int, int);
fixed FReturnPercentageMinMax(fixed, fixed, fixed);
int ReturnValuePercentage(int, int, int);
fixed FReturnValuePercentage(fixed, fixed, fixed);
fixed FMax(fixed, fixed);
fixed FMin(fixed, fixed);
fixed FMaxMin(fixed, fixed, fixed);
int SRandom(int, int, int);
fixed MagnitudeThree(fixed, fixed, fixed);
fixed MagDistance(fixed, fixed, fixed, fixed, fixed, fixed);

int IndexFlagLong (long long int);

#endif
