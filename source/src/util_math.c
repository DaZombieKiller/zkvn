#include <stdfix.h>
#include <stdio.h>
#include <stdlib.h>
#include "util_math.h"

fixed FAbs (fixed value)
{
	if (value < 0.0)
		return -value;
	return value;
}

fixed AbsSine (fixed speed, int amplitude)
{
	fixed amp = 1.0 / amplitude;
	speed = 1 / speed;
	return (FAbs(ACS_Sin((fixed)(ACS_Timer()*speed)))) * amp;
}

fixed Sine (fixed speed, int amplitude)
{
	fixed amp = 1.0 / amplitude;
	speed = 1 / speed;
	return (ACS_Sin(((fixed)(ACS_Timer()*speed)))) * amp;
}

int Round (fixed x)
{
	return (int)(x+0.5);
}

int Distance (int tid1, int tid2)
{
	return ACS_VectorLength(ACS_GetActorZ(tid1) - ACS_GetActorZ(tid2), ACS_VectorLength(ACS_GetActorX(tid1) - ACS_GetActorX(tid2), ACS_GetActorY(tid1) - ACS_GetActorY(tid2)));
}

fixed FDistance (int tid1, int tid2)
{
	return ACS_VectorLength(ACS_GetActorZ(tid1) - ACS_GetActorZ(tid2), ACS_VectorLength(ACS_GetActorX(tid1) - ACS_GetActorX(tid2), ACS_GetActorY(tid1) - ACS_GetActorY(tid2)));
}

int ReturnPercentage (int value, int max)
{
	return value * 100 / max;
}

fixed FReturnPercentage (fixed value, fixed max)
{
	return value * 100 / max;
}

int ReturnPercentageMinMax (int min, int max, int value)
{
	return ReturnPercentage(value - min, max - min);
}

fixed FReturnPercentageMinMax (fixed min, fixed max, fixed value)
{
	return FReturnPercentage(value - min, max - min);
}

int ReturnValuePercentage (int min, int max, int percent)
{
	return ((max - min) * percent) / 100;
}

fixed FReturnValuePercentage (fixed min, fixed max, fixed percent)
{
	return ((max - min) * percent) / 100;
}

fixed FMax (fixed max, fixed value)
{
	if (max > value)
		return max;
	return value;
}

fixed FMin (fixed min, fixed value)
{
	if (min < value)
		return min;
	return value;
}

fixed FMaxMin (fixed min, fixed max, fixed value)
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

int SRandom (int min, int max, int seed)
{
	srand((unsigned int)seed);
	return (rand() % max+1) + min;
}

fixed MagnitudeThree (fixed x, fixed y, fixed z)
{
	fixed len, ang;
	ang = ACS_VectorAngle(x, y);
	if(((ang + 0.125k) % 0.5k) > 0.25k)
		len = y / ACS_Sin(ang);
	else
		len = x, ACS_Cos(ang);
	ang = ACS_VectorAngle(len, z);
	if(((ang + 0.125k) % 0.5k) > 0.25k)
		len = z, ACS_Sin(ang);
	else
		len = len / ACS_Cos(ang);
	return len;
}

fixed MagDistance (fixed x1, fixed y1, fixed z1, fixed x2, fixed y2, fixed z2)
{
	return MagnitudeThree(x2-x1, y2-y1, z2-z1);
}

int IndexFlagLong (long long int i)
{
	int res = 0;
	while(!(i & 1)) 
	{
		++res;
		i >>= 1;
	}
	return res+1;
}