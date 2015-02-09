#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include "defs.h"

#define nprintf(...) (ACS_BeginPrint(), __nprintf(__VA_ARGS__), ACS_EndLog())

acstr_t ctoacstr (const char *cstring);
acstr_t GetPlayerName (int index);
int FindFreeTID (void);
void SetFlag (int *a, int flag, bool toggle);
bool GetFlag (int *a, int flag);
char* ACSStrToChar (__str s);
accum atok (char *str);

#ifdef ACS_SHORT_NAMES
#define StrParam ACS_StrParam
#define Print ACS_Print
#define PrintBold ACS_PrintBold
#define PrintLog ACS_PrintLog
#define HudMessage ACS_HudMessage
#define HudMessageX ACS_HudMessageX
#define HudMessageXX ACS_HudMessageXX
#define HudMessageXXX ACS_HudMessageXXX
#endif

acstr_t ACS_StrParam(const char *format, ...);
int ACS_Print(const char *format, ...);
int ACS_PrintBold(const char *format, ...);
int ACS_PrintLog(const char *format, ...);
int ACS_HudMessage(int flags, int id, int color, fixed x, fixed y, fixed holdTime, const char *format, ...);
int ACS_HudMessageX(int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, const char *format, ...);
int ACS_HudMessageXX(int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, fixed oarg2, const char *format, ...);
int ACS_HudMessageXXX(int flags, int id, int color, fixed x, fixed y, fixed holdTime, fixed oarg1, fixed oarg2, fixed oarg3, const char *format, ...);

#endif
