#ifndef INC_ZKVN_H_
#define INC_ZKVN_H_

// Main Header
#define VN_QUEUESIZE 500

#define VNQ_CHARACTER 1
#define VNQ_GRAPHIC 2
#define VNQ_DIALOGUE 3
#define VNQ_HOLDTIME 4

#define VNQ_CHOICE_START 5
#define VNQ_CHOICE_END 6
#define VNQ_CHOICE 7
#define VNQ_DEC_CHOICE 8

#define SCR_VN_MAIN 200
#define SCR_VN_ADD_DIALOGUE 201
#define SCR_VN_ADD_CHOICE 202
#define SCR_VN_ADD_CHOICE_START 203
#define SCR_VN_ADD_CHOICE_END 204
#define SCR_VN_ISQUEUEEMPTY 205
#define SCR_VN_SET_HOLD_TIME 206
#define SCR_VN_CHOICE_MADE 207
#define SCR_VN_CHOICE_DONE 208
#define SCR_VN_FORCE_FADEIN 209
#define SCR_VN_SET_GRAPHIC_SIZE 210
#define SCR_VN_VOICE_ACTING_ON 211
#define SCR_VN_VOICE_ACTING_OFF 212
#define SCR_VN_SET_GRAPHIC_POS 213
#define SCR_VN_SET_FADE_TIME 214

typedef struct vnQElement
{
	int characterID;
	int graphicID;
	int dialogueID;
	int holdTime;
	int choiceID;
	int decChoiceID;
	int choiceType;
	
	bool forceFadeIn;
	bool isChoiceStart;
	bool isChoiceEnd;
	bool isChoice;
	bool choiceNumbered;
	bool voiceActing;
	accum hudFadeTime;
	
	// Hud sizes
	int hudSizeGraphicW;
	int hudSizeGraphicH;
	
	// Hud positions
	accum hudPosGraphicX;
	accum hudPosGraphicY;
} vnQElement;

typedef struct vnQueue
{
	bool forceFadeIn;
	bool choiceStart;
	bool choiceEnd;
	bool voiceActing;
	int capacity;
	int size;
	int front;
	int rear;
	vnQElement elements[VN_QUEUESIZE];
	
	// Hud sizes
	int hudSizeGraphicW;
	int hudSizeGraphicH;
	int hudSizeChoiceW;
	int hudSizeChoiceH;
	int hudSizeInterfaceW;
	int hudSizeInterfaceH;
	int hudSizeNameW;
	int hudSizeNameH;
	int hudSizeSpeakingW;
	int hudSizeSpeakingH;
	int hudSizeDialogueW;
	int hudSizeDialogueH;
	
	// Hud positions
	accum hudPosGraphicX;
	accum hudPosGraphicY;
	accum hudPosAltChoiceX;
	accum hudPosAltChoiceY;
	accum hudPosChoiceX;
	accum hudPosChoiceY;
	accum hudPosInterfaceX;
	accum hudPosInterfaceY;
	accum hudPosNameX;
	accum hudPosNameY;
	accum hudPosSpeakingX;
	accum hudPosSpeakingY;
	accum hudPosDialogueX;
	accum hudPosDialogueY;
	
	// Misc
	accum hudFadeTime;
} vnQueue;

extern bool vnForceFadeIn[MAX_PLAYERS];
extern int vnChosenChoice[MAX_PLAYERS];
extern int vnHoldTime[MAX_PLAYERS];
extern vnQueue* vnQ[MAX_PLAYERS];

bool KeyDown (int key);
void VN_Queue_Init (void);
void VN_Queue_Pop (void);
void VN_Queue_Push (void);
int VN_Queue_Front (int type);
void VN_Queue_Element (int element, int type);

Z_SCRIPT (SCR_VN_MAIN) ENTER void VNMain (void);
Z_SCRIPT (SCR_VN_ADD_DIALOGUE) void AddDialogue (int characterID, int graphicID, int dialogueID);
Z_SCRIPT (SCR_VN_ADD_CHOICE) void AddChoice (int choiceID, int decChoiceID);
Z_SCRIPT (SCR_VN_ADD_CHOICE_START) void AddChoiceStart (int type, bool numbered);
Z_SCRIPT (SCR_VN_ADD_CHOICE_END) void AddChoiceEnd (void);
Z_SCRIPT (SCR_VN_ISQUEUEEMPTY) bool IsQueueEmpty (void);
Z_SCRIPT (SCR_VN_SET_HOLD_TIME) void SetHoldTime (int holdTime);
Z_SCRIPT (SCR_VN_CHOICE_MADE) int ChoiceMade (void);
Z_SCRIPT (SCR_VN_CHOICE_DONE) bool ChoiceDone (void);
Z_SCRIPT (SCR_VN_FORCE_FADEIN) void ForceFadeIn (void);
Z_SCRIPT (SCR_VN_SET_GRAPHIC_SIZE) void SetGraphicSize (int width, int height);
Z_SCRIPT (SCR_VN_VOICE_ACTING_ON) void VoiceActingOn (void);
Z_SCRIPT (SCR_VN_VOICE_ACTING_OFF) void VoiceActingOff (void);
Z_SCRIPT (SCR_VN_SET_GRAPHIC_POS) void SetGraphicPos (accum posx, accum posy);
Z_SCRIPT (SCR_VN_SET_FADE_TIME) void SetFadeTime (accum fadeTime);

#endif
