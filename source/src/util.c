// util.c
// Utility functions.

#include <stdfix.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "util.h"

// Gross hack to convert a C string to an ACS string using __nprintf.
acstr_t ctoacstr (const char *cstring)
{
	ACS_BeginPrint();
	__nprintf("%s", cstring);
	return ACS_EndString();
}

// Converts an ACS string to a C string
// Written by DavidPH, so thanks to him!
char* ACSStrToChar (__str s)
{
	static char* buf;
	static size_t alloc;
	size_t len = ACS_StrLen(s) + 1;
	if (alloc < len)
		buf = realloc(buf, len);
	for (size_t i = 0; i != len; ++i)
		buf[i] = s[i];
	return buf;
}

// Converts a string to accum
accum atok (char *str)
{
	accum rSide, lSide = strtoi((const char*)str, &str, 10);
	if (*str == '.')
	{
		str++;
		rSide = strtoi((const char*)str, &str, 10);
	}
	else
		rSide = 0;
	return lSide + (rSide / 10);
}

// Gross hack to get the player's name, by printing it to a string.
acstr_t GetPlayerName (int index)
{
	ACS_BeginPrint();
	ACS_PrintName(index);
	return ACS_EndString();
}

// Find an unused TID
int FindFreeTID (void)
{
	int low = -32768;
	int high = 32767;
    for (int tid = low; tid < high+1; tid++)
    {
		if (!tid)
			tid++;
        if (!ACS_ThingCount(T_NONE, tid))
            return tid;
    }
    return 0;
}

acstr_t ACS_StrParam (const char *format, ...)
{
	va_list arg;
	
	ACS_BeginPrint();
	
	va_start(arg, format);
	__vnprintf(format, arg);
	va_end(arg);
	
	return ACS_EndString();
}

int ACS_Print (const char *format, ...)
{
	int ret;
	va_list arg;
	
	ACS_BeginPrint();
	
	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);
	
	ACS_EndPrint();
	return ret;
}

int ACS_PrintBold (const char *format, ...)
{
	int ret;
	va_list arg;
	
	ACS_BeginPrint();
	
	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);
	
	ACS_EndPrintBold();
	return ret;
}

// Called PrintLog rather than Log to avoid confusion with logarithm
int ACS_PrintLog (const char *format, ...)
{
	int ret;
	va_list arg;
	
	ACS_BeginPrint();
	
	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);
	
	ACS_EndLog();
	return ret;
}

int ACS_HudMessage (int flags, int id, int color, fixed x, fixed y, fixed holdTime, const char *format, ...)
{
	int ret = 0;
	va_list arg;
	
	ACS_BeginPrint();
	
	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);
	
	ACS_MoreHudMessage();
	ACS_OptHudMessage(flags, id, color, x, y, holdTime);
	ACS_EndHudMessage();
	
	return ret;
}

int ACS_HudMessageX (int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, const char *format, ...)
{
	int ret = 0;
	va_list arg;
	
	ACS_BeginPrint();
	
	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);
	
	ACS_MoreHudMessage();
	ACS_OptHudMessage(flags, id, color, x, y, holdTime);
	ACS_EndHudMessageX(oarg1);
	
	return ret;
}

int ACS_HudMessageXX (int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, fixed oarg2, const char *format, ...)
{
	int ret = 0;
	va_list arg;
	
	ACS_BeginPrint();
	
	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);
	
	ACS_MoreHudMessage();
	ACS_OptHudMessage(flags, id, color, x, y, holdTime);
	ACS_EndHudMessageXX(oarg1, oarg2);
	
	return ret;
}

int ACS_HudMessageXXX (int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, fixed oarg2, fixed oarg3, const char *format, ...)
{
	int ret = 0;
	va_list arg;
	
	ACS_BeginPrint();
	
	va_start(arg, format);
	ret = __vnprintf(format, arg);
	va_end(arg);
	
	ACS_MoreHudMessage();
	ACS_OptHudMessage(flags, id, color, x, y, holdTime);
	ACS_EndHudMessageXXX(oarg1, oarg2, oarg3);
	
	return ret;
}

void SetFlag (int *a, int flag, bool toggle)
{
	if(toggle == true)
		*a = *a | flag;
	if(toggle == false)
		*a = *a & (~flag);
}

bool GetFlag (int *a, int flag)
{
	if (*a & flag)
		return true;
	return false;
}
