#ifndef INC_DEFS_H_
#define INC_DEFS_H_

#ifdef ZAN_ACS
#include <ACS_Zandronum.h>
#else
#include <ACS_ZDoom.h>
#endif

#include <stdbool.h>

// A bunch of definitions for GDCC extensions
#define I_SCRIPT(n) [[address(n), call("ScriptI")]]
#define S_SCRIPT [[call("ScriptS")]]
#define ACS_I_SCRIPT(n) EXTACS I_SCRIPT(n)
#define ACS_S_SCRIPT EXTACS S_SCRIPT
#define EXTACS [[extern("ACS")]]
#define ACS_SCRIPT EXTACS S_SCRIPT
#define OPEN [[script("Open")]]
#define ENTER [[script("Enter")]]
#define DISCONNECT [[script("Disconnect")]]
#define DEATH [[script("Death")]]
#define NET [[script("Net")]]
#define CLIENTSIDE [[script("Clientside")]]
#define ALLOC(n) [[alloc_Loc(n)]]
#define OPT_ARGS(n) [[optional_args(n)]]
#ifdef ZAN_ACS
#define Z_SCRIPT(n) EXTACS [[address(n), call("ScriptI")]]
#else
#define Z_SCRIPT(n) EXTACS [[call("ScriptS")]]
#endif

// Definitions for color codes
#define COL_BRICK     "\Ca"
#define COL_TAN       "\Cb"
#define COL_GRAY      "\Cc"
#define COL_GREEN     "\Cd"
#define COL_BROWN     "\Ce"
#define COL_GOLD      "\Cf"
#define COL_RED       "\Cg"
#define COL_BLUE      "\Ch"
#define COL_ORANGE    "\Ci"
#define COL_WHITE     "\Cj"
#define COL_YELLOW    "\Ck"
#define COL_RETURN    "\Cl"
#define COL_BLACK     "\Cm"
#define COL_LBLUE     "\Cn"
#define COL_CREAM     "\Co"
#define COL_OLIVE     "\Cp"
#define COL_DARKGREEN "\Cq"
#define COL_DARKRED   "\Cr"
#define COL_DARKBROWN "\Cs"
#define COL_PURPLE    "\Ct"
#define COL_DARKGRAY  "\Cu"
#define COL_CYAN      "\Cv"
#define COL_DEFPRINT  "\C-"
#define COL_DEFPRINTB "\C+"
#define COL_DEFCHAT   "\C*"
#define COL_DEFTCHAT  "\C!"

#define TICRATE 35

#ifdef ZAN_ACS
#define MAX_PLAYERS 64
#else
#define MAX_PLAYERS 8
#endif

#define off 0
#define on 1

typedef int flag_t;
typedef __str acstr_t;

#endif
