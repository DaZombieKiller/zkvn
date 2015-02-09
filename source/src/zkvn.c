// ZKVN Engine
// Main Source

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdfix.h>
#include "util.h"
#include "util_math.h"
#include "zkvn.h"

// Variables
int vnChosenChoice[MAX_PLAYERS];
int vnHoldTime[MAX_PLAYERS];
vnQueue* vnQ[MAX_PLAYERS];

// Functions
bool KeyDown (int key)
{
	int buttons = ACS_GetPlayerInput(-1, INPUT_BUTTONS);
	if (buttons & key)
		return true;
	return false;
}

// VN Queue Functions
void VN_Queue_Init (void)
{
	int plrNum = ACS_PlayerNumber();
	
	vnHoldTime[plrNum] = 0;
	vnQ[plrNum] = (vnQueue*)malloc(sizeof(vnQueue));
	
	vnQ[plrNum] -> choiceStart = false;
	vnQ[plrNum] -> choiceEnd = false;
	vnQ[plrNum] -> size = 0;
	vnQ[plrNum] -> capacity = VN_QUEUESIZE;
	vnQ[plrNum] -> front = 0;
	vnQ[plrNum] -> rear = -1;
}

void VN_Queue_Pop (void)
{
	int plrNum = ACS_PlayerNumber();
	if (!vnQ[plrNum] -> size)
	{
		#ifdef VN_DEBUG
		ACS_PrintLog("Queue is empty, cannot pop.");
		#endif
		return;
	}
	else
	{
		vnQ[plrNum] -> elements[vnQ[plrNum] -> front].isChoiceStart = false;
		vnQ[plrNum] -> elements[vnQ[plrNum] -> front].isChoiceEnd = false;
		vnQ[plrNum] -> elements[vnQ[plrNum] -> front].isChoice = false;
		vnQ[plrNum] -> size--;
		vnQ[plrNum] -> front++;
		if (vnQ[plrNum] -> front == vnQ[plrNum] -> capacity)
			vnQ[plrNum] -> front = 0;
	}
	return;
}

void VN_Queue_Push (void)
{
	int plrNum = ACS_PlayerNumber();
	if (vnQ[plrNum] -> size == vnQ[plrNum] -> capacity)
	{
		#ifdef VN_DEBUG
		ACS_PrintLog("Queue is full, cannot push.");
		#endif
		return;
	}
	else
	{
		vnQ[plrNum] -> size++;
		vnQ[plrNum] -> rear++;
		if (vnQ[plrNum] -> rear == vnQ[plrNum] -> capacity)
			vnQ[plrNum] -> rear = 0;
	}
	return;
}

void VN_Queue_Element (int element, int type)
{
	int plrNum = ACS_PlayerNumber();
	switch (type)
	{
	case VNQ_CHARACTER:
		vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].characterID = element;
		break;
	case VNQ_GRAPHIC:
		vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].graphicID = element;
		break;
	case VNQ_DIALOGUE:
		vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].dialogueID = element;
		break;
	case VNQ_HOLDTIME:
		vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].holdTime = element;
		break;
	case VNQ_CHOICE_START:
		vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].isChoiceStart = true;
		break;
	case VNQ_CHOICE_END:
		vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].isChoiceEnd = true;
		break;
	case VNQ_CHOICE:
		vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].isChoice = true;
		vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].choiceID = element;
		break;
	case VNQ_DEC_CHOICE:
		vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].decChoiceID = element;
		break;
	}
}

int VN_Queue_Front (int type)
{
	int plrNum = ACS_PlayerNumber();
	if (!vnQ[plrNum] -> size)
	{
		#ifdef VN_DEBUG
		ACS_PrintLog("Queue is full, no value retrieved.");
		#endif
		return 0;
	}
	switch (type)
	{
	case VNQ_CHARACTER:
		return vnQ[plrNum] -> elements[vnQ[plrNum] -> front].characterID;
	case VNQ_GRAPHIC:
		return vnQ[plrNum] -> elements[vnQ[plrNum] -> front].graphicID;
	case VNQ_DIALOGUE:
		return vnQ[plrNum] -> elements[vnQ[plrNum] -> front].dialogueID;
	case VNQ_HOLDTIME:
		return vnQ[plrNum] -> elements[vnQ[plrNum] -> front].holdTime;
	case VNQ_CHOICE_START:
		return vnQ[plrNum] -> elements[vnQ[plrNum] -> front].isChoiceStart;
	case VNQ_CHOICE_END:
		return vnQ[plrNum] -> elements[vnQ[plrNum] -> front].isChoiceEnd;
	case VNQ_CHOICE:
		return vnQ[plrNum] -> elements[vnQ[plrNum] -> front].choiceID;
	case VNQ_DEC_CHOICE:
		return vnQ[plrNum] -> elements[vnQ[plrNum] -> front].decChoiceID;
	}
	return 0;
}

// Scripts
Z_SCRIPT (SCR_VN_MAIN) ENTER void VNMain (void)
{
	// Initialise the visual novel queue
	VN_Queue_Init();
	
	// Main VN code
	bool ignoreUse, ignoreForward, ignoreBack, fadeIn = true;
	int i, ht, hudBoundX, hudBoundY, hudBoundW, hudBoundH, hudClipW, plrNum = ACS_PlayerNumber();
	
	__str vnStr_Speaking = ACS_StrParam("\C%LS%LS", s"VN_SPEAKING_COL", s"VN_SPEAKING");
	__str vnStr_SpeakingFont = ACS_StrParam("%LS", s"VN_SPEAKINGFONT");
	
	__str vnStr_Name;
	__str vnStr_NameFont = ACS_StrParam("%LS", s"VN_NAMEFONT");
	
	__str vnStr_Dialogue;
	__str vnStr_DialogueFont = ACS_StrParam("%LS", s"VN_DIALOGUEFONT");
	
	__str vnStr_Graphic;
	__str vnStr_InterfaceGraphic = ACS_StrParam("%LS", s"VN_INTERFACE");
	
	__str vnStr_ChoiceFont = ACS_StrParam("%LS", s"VN_CHOICEFONT");
	
	int hmDialogue = 10;
	int hmName = 11;
	int hmSpeaking = 12;
	int hmChoices = 14;
	int hmInterface = 15;
	int hmGraphic = 16;
	
	// Hud sizes
	vnQ[plrNum] -> hudSizeGraphicW = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_DEFAULT_HUD_SIZE_GRAPHIC_W")));
	vnQ[plrNum] -> hudSizeGraphicH = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_DEFAULT_HUD_SIZE_GRAPHIC_H")));
	vnQ[plrNum] -> hudSizeChoiceW = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_CHOICE_W")));
	vnQ[plrNum] -> hudSizeChoiceH = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_CHOICE_H")));
	vnQ[plrNum] -> hudSizeInterfaceW = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_INTERFACE_W")));
	vnQ[plrNum] -> hudSizeInterfaceH = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_INTERFACE_H")));
	vnQ[plrNum] -> hudSizeNameW = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_NAME_W")));
	vnQ[plrNum] -> hudSizeNameH = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_NAME_H")));
	vnQ[plrNum] -> hudSizeSpeakingW = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_SPEAKING_W")));
	vnQ[plrNum] -> hudSizeSpeakingH = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_SPEAKING_H")));
	vnQ[plrNum] -> hudSizeDialogueW = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_DIALOGUE_W")));
	vnQ[plrNum] -> hudSizeDialogueH = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_SIZE_DIALOGUE_H")));
	
	// Hud positions
	vnQ[plrNum] -> hudPosGraphicX = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_DEFAULT_HUD_POS_GRAPHIC_X")));
	vnQ[plrNum] -> hudPosGraphicY = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_DEFAULT_HUD_POS_GRAPHIC_Y")));
	vnQ[plrNum] -> hudPosAltChoiceX = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_ALTCHOICE_X")));
	vnQ[plrNum] -> hudPosAltChoiceY = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_ALTCHOICE_Y")));
	vnQ[plrNum] -> hudPosChoiceX = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_CHOICE_X")));
	vnQ[plrNum] -> hudPosChoiceY = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_CHOICE_Y")));
	vnQ[plrNum] -> hudPosInterfaceX = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_INTERFACE_X")));
	vnQ[plrNum] -> hudPosInterfaceY = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_INTERFACE_Y")));
	vnQ[plrNum] -> hudPosNameX = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_NAME_X")));
	vnQ[plrNum] -> hudPosNameY = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_NAME_Y")));
	vnQ[plrNum] -> hudPosSpeakingX = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_SPEAKING_X")));
	vnQ[plrNum] -> hudPosSpeakingY = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_SPEAKING_Y")));
	vnQ[plrNum] -> hudPosDialogueX = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_DIALOGUE_X")));
	vnQ[plrNum] -> hudPosDialogueY = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_POS_DIALOGUE_Y")));
	
	// Misc
	vnQ[plrNum] -> hudFadeTime = atok(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_FADE_TIME")));
	
	// Dialogue choice stuff
	__str choiceStr;
	int selectedChoice;
	int choiceCount;
	int choiceListType;
	bool choiceListNumbered;
	
	while (true)
	{
		if (!IsQueueEmpty())
		{
			ACS_SetPlayerProperty(0, ON, PROP_TOTALLYFROZEN);
			ACS_SetActorVelocity(0, 0k, 0k, ACS_GetActorVelZ(0), false, true);
			
			if (VN_Queue_Front(VNQ_CHOICE) || VN_Queue_Front(VNQ_CHOICE_END))
				VN_Queue_Pop();
			else if (VN_Queue_Front(VNQ_CHOICE_START))
			{
				// Hide dialogue UI if it's visible
				ACS_HudMessage(HUDMSG_PLAIN, hmDialogue, CR_UNTRANSLATED, 0k, 0k, 0k, "");
				ACS_HudMessage(HUDMSG_PLAIN, hmName, CR_UNTRANSLATED, 0k, 0k, 0k, "");
				ACS_HudMessage(HUDMSG_PLAIN, hmSpeaking, CR_UNTRANSLATED, 0k, 0k, 0k, "");
				ACS_HudMessage(HUDMSG_PLAIN, hmInterface, CR_UNTRANSLATED, 0k, 0k, 0k, "");
				ACS_HudMessage(HUDMSG_PLAIN, hmGraphic, CR_UNTRANSLATED, 0k, 0k, 0k, "");
				
				// Wait for end of choice block to be added
				while (!vnQ[plrNum] -> choiceEnd) {}
				
				// Get number of choices
				choiceCount = 0;
				i = vnQ[plrNum] -> front + 1;
				
				while ((vnQ[plrNum] -> elements[i].isChoiceEnd) == false)
				{
					choiceCount++;
					i++;
				}
				
				// Display and handle choices
				selectedChoice = 0;
				ignoreUse = true;
				while (true)
				{
					// HUD size and font
					ACS_SetHudSize(vnQ[plrNum] -> hudSizeChoiceW, vnQ[plrNum] -> hudSizeChoiceH, true);
					ACS_SetFont(vnStr_ChoiceFont);
					
					// Variables
					choiceStr = s"";
					
					// Build choice string
					for (i = 0; i < choiceCount; i++)
					{
						if (selectedChoice == i)
						{
							if (vnQ[plrNum] -> elements[vnQ[plrNum] -> front].choiceNumbered)
								choiceStr = ACS_StrParam("%S\n\C%LS%d. %LS", choiceStr, s"VN_SEL_CHOICE_COL", i + 1, ACS_StrParam("VNO_%d", vnQ[plrNum] -> elements[vnQ[plrNum] -> front + 1 + i].choiceID));
							else
								choiceStr = ACS_StrParam("%S\n\C%LS%LS", choiceStr, s"VN_SEL_CHOICE_COL", ACS_StrParam("VNO_%d", vnQ[plrNum] -> elements[vnQ[plrNum] -> front + 1 + i].choiceID));
						}
						else
						{
							if (vnQ[plrNum] -> elements[vnQ[plrNum] -> front].choiceNumbered)
								choiceStr = ACS_StrParam("%S\n\C%LS%d. %LS", choiceStr, s"VN_CHOICE_COL", i + 1, ACS_StrParam("VNO_%d", vnQ[plrNum] -> elements[vnQ[plrNum] -> front + 1 + i].choiceID));
							else
								choiceStr = ACS_StrParam("%S\n\C%LS%LS", choiceStr, s"VN_CHOICE_COL", ACS_StrParam("VNO_%d", vnQ[plrNum] -> elements[vnQ[plrNum] -> front + 1 + i].choiceID));
						}
					}
					
					// Display choices
					if (vnQ[plrNum] -> elements[vnQ[plrNum] -> front].choiceType == 0)
						ACS_HudMessage(HUDMSG_PLAIN, hmChoices, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosChoiceX, vnQ[plrNum] -> hudPosChoiceY, 0k, "%S", choiceStr);
					else if (vnQ[plrNum] -> elements[vnQ[plrNum] -> front].choiceType == 1)
						ACS_HudMessage(HUDMSG_PLAIN, hmChoices, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosAltChoiceX, vnQ[plrNum] -> hudPosAltChoiceY, 0k, "%S", choiceStr);
					else if (vnQ[plrNum] -> elements[vnQ[plrNum] -> front].choiceType == 2)
					{
						ACS_HudMessage(HUDMSG_PLAIN, hmChoices, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosChoiceX, vnQ[plrNum] -> hudPosChoiceY, 0k, "%S", choiceStr);
						ACS_SetHudSize(vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudSizeGraphicW, vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudSizeGraphicH, true);
						ACS_SetFont(vnStr_Graphic);
						ACS_HudMessage(HUDMSG_PLAIN, hmGraphic, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosGraphicX, vnQ[plrNum] -> hudPosGraphicY, 0k, "A");
						
						ACS_SetHudSize(vnQ[plrNum] -> hudSizeInterfaceW, vnQ[plrNum] -> hudSizeInterfaceH, true);
						ACS_SetFont(vnStr_InterfaceGraphic);
						ACS_HudMessage(HUDMSG_PLAIN, hmInterface, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosInterfaceX, vnQ[plrNum] -> hudPosInterfaceY, 0k, "A");
						
						ACS_SetHudSize(vnQ[plrNum] -> hudSizeNameW, vnQ[plrNum] -> hudSizeNameH, true);
						ACS_SetFont(vnStr_NameFont);
						ACS_HudMessage(HUDMSG_PLAIN, hmName, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosNameX, vnQ[plrNum] -> hudPosNameY, 0k, "%S", vnStr_Name);
						ACS_SetHudSize(vnQ[plrNum] -> hudSizeSpeakingW, vnQ[plrNum] -> hudSizeSpeakingH, true);
						ACS_SetFont(vnStr_SpeakingFont);
					}
					else if (vnQ[plrNum] -> elements[vnQ[plrNum] -> front].choiceType == 3)
					{
						ACS_HudMessage(HUDMSG_PLAIN, hmChoices, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosAltChoiceX, vnQ[plrNum] -> hudPosAltChoiceY, 0k, "%S", choiceStr);
						ACS_SetHudSize(vnQ[plrNum] -> elements[vnQ[plrNum] -> front - 1].hudSizeGraphicW, vnQ[plrNum] -> elements[vnQ[plrNum] -> front - 1].hudSizeGraphicH, true);
						ACS_SetFont(vnStr_Graphic);
						ACS_HudMessage(HUDMSG_PLAIN, hmGraphic, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosGraphicX, vnQ[plrNum] -> hudPosGraphicY, 0k, "A");
						
						ACS_SetHudSize(vnQ[plrNum] -> hudSizeInterfaceW, vnQ[plrNum] -> hudSizeInterfaceH, true);
						ACS_SetFont(vnStr_InterfaceGraphic);
						ACS_HudMessage(HUDMSG_PLAIN, hmInterface, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosInterfaceX, vnQ[plrNum] -> hudPosInterfaceY, 0k, "A");
						
						ACS_SetHudSize(vnQ[plrNum] -> hudSizeNameW, vnQ[plrNum] -> hudSizeNameH, true);
						ACS_SetFont(vnStr_NameFont);
						ACS_HudMessage(HUDMSG_PLAIN, hmName, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosNameX, vnQ[plrNum] -> hudPosNameY, 0k, "%S", vnStr_Name);
						ACS_SetHudSize(vnQ[plrNum] -> hudSizeSpeakingW, vnQ[plrNum] -> hudSizeSpeakingH, true);
						ACS_SetFont(vnStr_SpeakingFont);
					}
					
					// Select choices
					if (KeyDown(BT_FORWARD) && !ignoreForward)
					{
						ignoreForward = true;
						selectedChoice--;
						if (selectedChoice < 0)
							selectedChoice = choiceCount - 1;
					}
					
					if (KeyDown(BT_BACK) && !ignoreBack)
					{
						ignoreBack = true;
						selectedChoice++;
						if (selectedChoice > choiceCount - 1)
							selectedChoice = 0;
					}
					
					if ((KeyDown(BT_USE) || KeyDown(BT_ATTACK) || KeyDown(BT_ALTATTACK)) && !ignoreUse)
					{
						ignoreUse = true;
						vnChosenChoice[plrNum] = vnQ[plrNum] -> elements[vnQ[plrNum] -> front + 1 + selectedChoice].decChoiceID;
						vnQ[plrNum] -> choiceStart = false;
						vnQ[plrNum] -> choiceEnd = false;
						ACS_HudMessage(HUDMSG_PLAIN, hmChoices, CR_UNTRANSLATED, 0k, 0k, 0k, "");
						break;
					}
					
					if (!KeyDown(BT_USE) && !KeyDown(BT_ATTACK) && !KeyDown(BT_ALTATTACK))
							ignoreUse = false;
					if (!KeyDown(BT_FORWARD))
							ignoreForward = false;
					if (!KeyDown(BT_BACK))
							ignoreBack = false;
					ACS_Delay(1);
				}
				VN_Queue_Pop();
			}
			else
			{
				if (fadeIn || vnQ[plrNum] -> elements[vnQ[plrNum] -> front].forceFadeIn)
				{
					fadeIn = false;
					vnStr_Name = ACS_StrParam("\C%LS%LS", s"VN_NAME_COL", ACS_StrParam("VNC_%d", VN_Queue_Front(VNQ_CHARACTER)));
					vnStr_Graphic = ACS_StrParam("%LS", ACS_StrParam("VNG_%d_%d", VN_Queue_Front(VNQ_CHARACTER), VN_Queue_Front(VNQ_GRAPHIC)));
					vnStr_Dialogue = ACS_StrParam("\C%LS%LS", s"VN_DIALOGUE_COL", ACS_StrParam("VND_%d_%d", VN_Queue_Front(VNQ_CHARACTER), VN_Queue_Front(VNQ_DIALOGUE)));
					if (VN_Queue_Front(VNQ_GRAPHIC) == -1)
						vnStr_Graphic = s"";
					
					ACS_SetHudSize(vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudSizeGraphicW, vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudSizeGraphicH, true);
					ACS_SetFont(vnStr_Graphic);
					ACS_HudMessageX(HUDMSG_FADEINOUT, hmGraphic, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosGraphicX, vnQ[plrNum] -> hudPosGraphicY, 0k, vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudFadeTime, "A");
					
					ACS_SetHudSize(vnQ[plrNum] -> hudSizeInterfaceW, vnQ[plrNum] -> hudSizeInterfaceH, true);
					ACS_SetFont(vnStr_InterfaceGraphic);
					ACS_HudMessageX(HUDMSG_FADEINOUT, hmInterface, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosInterfaceX, vnQ[plrNum] -> hudPosInterfaceY, 0k, vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudFadeTime, "A");
					
					ACS_SetHudSize(vnQ[plrNum] -> hudSizeNameW, vnQ[plrNum] -> hudSizeNameH, true);
					ACS_SetFont(vnStr_NameFont);
					ACS_HudMessageX(HUDMSG_FADEINOUT, hmName, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosNameX, vnQ[plrNum] -> hudPosNameY, 0k, vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudFadeTime, "%S", vnStr_Name);
					ACS_SetHudSize(vnQ[plrNum] -> hudSizeSpeakingW, vnQ[plrNum] -> hudSizeSpeakingH, true);
					ACS_SetFont(vnStr_SpeakingFont);
					ACS_HudMessageX(HUDMSG_FADEINOUT, hmSpeaking, CR_GOLD, vnQ[plrNum] -> hudPosSpeakingX, vnQ[plrNum] -> hudPosSpeakingY, 0k, vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudFadeTime, "%S", vnStr_Speaking);
					
					ACS_Delay(TICRATE * vnQ[plrNum] -> hudFadeTime);
				}
				
				vnStr_Name = ACS_StrParam("\C%LS%LS", s"VN_NAME_COL", ACS_StrParam("VNC_%d", VN_Queue_Front(VNQ_CHARACTER)));
				vnStr_Graphic = ACS_StrParam("%LS", ACS_StrParam("VNG_%d_%d", VN_Queue_Front(VNQ_CHARACTER), VN_Queue_Front(VNQ_GRAPHIC)));
				vnStr_Dialogue = ACS_StrParam("\C%LS%LS", s"VN_DIALOGUE_COL", ACS_StrParam("VND_%d_%d", VN_Queue_Front(VNQ_CHARACTER), VN_Queue_Front(VNQ_DIALOGUE)));
				if (VN_Queue_Front(VNQ_GRAPHIC) == -1)
					vnStr_Graphic = s"";
				
				ACS_SetHudSize(vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudSizeGraphicW, vnQ[plrNum] -> elements[vnQ[plrNum] -> front].hudSizeGraphicH, true);
				ACS_SetFont(vnStr_Graphic);
				ACS_HudMessage(HUDMSG_PLAIN, hmGraphic, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosGraphicX, vnQ[plrNum] -> hudPosGraphicY, 0k, "A");
				
				ACS_SetHudSize(vnQ[plrNum] -> hudSizeInterfaceW, vnQ[plrNum] -> hudSizeInterfaceH, true);
				ACS_SetFont(vnStr_InterfaceGraphic);
				ACS_HudMessage(HUDMSG_PLAIN, hmInterface, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosInterfaceX, vnQ[plrNum] -> hudPosInterfaceY, 0k, "A");
				
				ACS_SetHudSize(vnQ[plrNum] -> hudSizeNameW, vnQ[plrNum] -> hudSizeNameH, true);
				ACS_SetFont(vnStr_NameFont);
				ACS_HudMessage(HUDMSG_PLAIN, hmName, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosNameX, vnQ[plrNum] -> hudPosNameY, 0k, "%S", vnStr_Name);
				ACS_SetHudSize(vnQ[plrNum] -> hudSizeSpeakingW, vnQ[plrNum] -> hudSizeSpeakingH, true);
				ACS_SetFont(vnStr_SpeakingFont);
				ACS_HudMessage(HUDMSG_PLAIN, hmSpeaking, CR_UNTRANSLATED, vnQ[plrNum] -> hudPosSpeakingX, vnQ[plrNum] -> hudPosSpeakingY, 0k, "%S", vnStr_Speaking);
				ACS_SetHudSize(vnQ[plrNum] -> hudSizeDialogueW, vnQ[plrNum] -> hudSizeDialogueH, true);
				ACS_SetFont(vnStr_DialogueFont);
				
				// Voice acting
				if (vnQ[plrNum] -> elements[vnQ[plrNum] -> front].voiceActing)
					ACS_PlaySound(0, ACS_StrParam("%LS", ACS_StrParam("VNA_%d_%d", VN_Queue_Front(VNQ_CHARACTER), VN_Queue_Front(VNQ_DIALOGUE))), CHAN_BODY, 1k, false, ATTN_NORM);
				
				// Word wrap
				#ifndef ZAN_ACS
				hudBoundX = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_BOUNDARY_X")));
				hudBoundY = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_BOUNDARY_Y")));
				hudBoundW = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_BOUNDARY_W")));
				hudBoundH = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_BOUNDARY_H")));
				hudClipW = atoi(ACSStrToChar(ACS_StrParam("%LS", s"VN_HUD_WRAP_W")));
				ACS_SetHudClipRect(hudBoundX, hudBoundY, hudBoundW, hudBoundH, hudClipW);
				#endif
				ACS_HudMessage(HUDMSG_PLAIN, hmDialogue, CR_WHITE, vnQ[plrNum] -> hudPosDialogueX, vnQ[plrNum] -> hudPosDialogueY, 0k, "%S", vnStr_Dialogue);
				#ifndef ZAN_ACS
				ACS_SetHudClipRect(0, 0, 0, 0, 0);
				#endif
				
				i = 0;
				ht = VN_Queue_Front(VNQ_HOLDTIME);
				while (i != ht)
				{
					if (ht >= 0)
						i++;
					if (!KeyDown(BT_USE) && !KeyDown(BT_ATTACK) && !KeyDown(BT_ALTATTACK))
						ignoreUse = false;
					if ((KeyDown(BT_USE) || KeyDown(BT_ATTACK) || KeyDown(BT_ALTATTACK)) && !ignoreUse)
					{
						ignoreUse = true;
						break;
					}
					ACS_Delay(1);
				}
				
				// Stop voice acting
				ACS_StopSound(0, CHAN_BODY);
				
				VN_Queue_Pop();
			}
		}
		else
		{
			fadeIn = true;
			ACS_SetPlayerProperty(0, OFF, PROP_TOTALLYFROZEN);
			ACS_HudMessage(HUDMSG_PLAIN, hmDialogue, CR_UNTRANSLATED, 0k, 0k, 0k, "");
			ACS_HudMessage(HUDMSG_PLAIN, hmName, CR_UNTRANSLATED, 0k, 0k, 0k, "");
			ACS_HudMessage(HUDMSG_PLAIN, hmSpeaking, CR_UNTRANSLATED, 0k, 0k, 0k, "");
			ACS_HudMessage(HUDMSG_PLAIN, hmInterface, CR_UNTRANSLATED, 0k, 0k, 0k, "");
			ACS_HudMessage(HUDMSG_PLAIN, hmGraphic, CR_UNTRANSLATED, 0k, 0k, 0k, "");
		}
		
		ACS_Delay(1);
	}
	return;
}

Z_SCRIPT (SCR_VN_ADD_DIALOGUE) void AddDialogue (int characterID, int graphicID, int dialogueID)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	VN_Queue_Push();
	VN_Queue_Element(characterID, VNQ_CHARACTER);
	VN_Queue_Element(graphicID, VNQ_GRAPHIC);
	VN_Queue_Element(dialogueID, VNQ_DIALOGUE);
	VN_Queue_Element(vnHoldTime[plrNum], VNQ_HOLDTIME);
	vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].hudSizeGraphicW = vnQ[plrNum] -> hudSizeGraphicW;
	vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].hudSizeGraphicH = vnQ[plrNum] -> hudSizeGraphicH;
	vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].hudPosGraphicX = vnQ[plrNum] -> hudPosGraphicX;
	vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].hudPosGraphicY = vnQ[plrNum] -> hudPosGraphicY;
	vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].forceFadeIn = vnQ[plrNum] -> forceFadeIn;
	vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].voiceActing = vnQ[plrNum] -> voiceActing;
	vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].hudFadeTime = vnQ[plrNum] -> hudFadeTime;
	vnQ[plrNum] -> forceFadeIn = false;
	return;
}

Z_SCRIPT (SCR_VN_ADD_CHOICE) void AddChoice (int choiceID, int decChoiceID)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	VN_Queue_Push();
	VN_Queue_Element(choiceID, VNQ_CHOICE);
	VN_Queue_Element(decChoiceID, VNQ_DEC_CHOICE);
	return;
}

Z_SCRIPT (SCR_VN_ADD_CHOICE_START) void AddChoiceStart (int type, bool numbered)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	VN_Queue_Push();
	VN_Queue_Element(true, VNQ_CHOICE_START);
	vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].choiceType = type;
	vnQ[plrNum] -> elements[vnQ[plrNum] -> rear].choiceNumbered = numbered;
	vnQ[ACS_PlayerNumber()] -> choiceStart = true;
	return;
}

Z_SCRIPT (SCR_VN_ADD_CHOICE_END) void AddChoiceEnd (void)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	VN_Queue_Push();
	VN_Queue_Element(true, VNQ_CHOICE_END);
	vnQ[ACS_PlayerNumber()] -> choiceEnd = true;
	return;
}

Z_SCRIPT (SCR_VN_ISQUEUEEMPTY) bool IsQueueEmpty (void)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	if (!vnQ[plrNum] -> size)
		return true;
	return false;
}

Z_SCRIPT (SCR_VN_SET_HOLD_TIME) void SetHoldTime (int holdTime)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	vnHoldTime[ACS_PlayerNumber()] = holdTime;
	return;
}

Z_SCRIPT (SCR_VN_CHOICE_MADE) int ChoiceMade (void)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	return vnChosenChoice[plrNum];
}

Z_SCRIPT (SCR_VN_CHOICE_DONE) bool ChoiceDone (void)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	if (vnQ[plrNum] -> choiceStart == false)
		return true;
	return false;
}

Z_SCRIPT (SCR_VN_FORCE_FADEIN) void ForceFadeIn (void)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	vnQ[plrNum] -> forceFadeIn = true;
	return;
}

Z_SCRIPT (SCR_VN_SET_GRAPHIC_SIZE) void SetGraphicSize (int width, int height)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	vnQ[plrNum] -> hudSizeGraphicW = width;
	vnQ[plrNum] -> hudSizeGraphicH = height;
	return;
}

Z_SCRIPT (SCR_VN_VOICE_ACTING_ON) void VoiceActingOn (void)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	vnQ[plrNum] -> voiceActing = true;
	return;
}

Z_SCRIPT (SCR_VN_VOICE_ACTING_OFF) void VoiceActingOff (void)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	vnQ[plrNum] -> voiceActing = false;
	return;
}

Z_SCRIPT (SCR_VN_SET_GRAPHIC_POS) void SetGraphicPos (accum posx, accum posy)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	vnQ[plrNum] -> hudPosGraphicX = posx;
	vnQ[plrNum] -> hudPosGraphicY = posy;
	return;
}

Z_SCRIPT (SCR_VN_SET_FADE_TIME) void SetFadeTime (accum fadeTime)
{
	if (ACS_PlayerNumber() == -1)
		ACS_SetActivatorToTarget(0);
	int plrNum = ACS_PlayerNumber();
	vnQ[plrNum] -> hudFadeTime = fadeTime;
	return;
}
