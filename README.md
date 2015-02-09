#### ZKVN - A visual novel engine for ZDoom, GZDoom and Zandronum
##### What is ZKVN?
ZKVN is an easy to use visual novel engine for ZDoom and GZDoom. Zandronum 2.0 is supported as well, however automatic word wrap is not supported.
Rather than regular ACS, ZKVN is written in C, and compiled using DavidPH's GDCC.

ZKVN started development as the visual novel system for HDoom, and eventually branched off into its own project, that could be used for other mods as well. Therefore, without HDoom, ZKVN wouldn't exist.
Be sure to check out HDoom if you don't mind seeing demon ladies in lewd situations, it's very well done and the team are a great group of people. Once it starts to get its story going it's only going to get better.

##### How do I use it?
My goal for ZKVN was to avoid the need to edit the source code as much as possible. As such, everything you would need to change is located within the decorate and language lumps.
For example, in the little demo included with ZKVN, this is how things are set up:

First, we define the specifics for our characters: their names, graphics, dialogue and the choices that can be made by the player within dialogue.
This is all done in the language lump. The language lump included with ZKVN is commented to help you out with the syntax and usage.

```
// Choices
VNO_1 = "How easy is ZKVN?";
VNO_2 = "What's left to do?";

// Character names
VNC_1 = "Mr. Nobody";

// Graphics
// Mr. Nobody
VNG_1_1 = "IMP";

// Dialogue
// Mr. Nobody
VND_1_1 = "Hi! Welcome to the ZKVN demo! I'm Mr. Nobody "
		"and I will be your guide. What would you like to "
		"know?";

VND_1_2 = "If you examine the LANGUAGE and DECORATE lumps "
		"inside the pk3, you'll notice how easy ZKVN "
		"is to use!";

VND_1_3 = "ZKVN is surprisingly close to completion, all "
		"that's really left to do is add some additional "
		"transition effects.";
```

Dialogue itself is "scripted" in decorate like so:

```
// Constants
#include "decorate/zkvn_const.txt"
const int VNC_MRNOBODY = 1;

// Mr Nobody
ACTOR MrNobody : SwitchingDecoration 1337
{
	//$Category ZK-VN Demo
	//$Title "Mr. Nobody"
	//$Sprite TROOA1
	Radius 20
	Height 56
	Activation THINGSPEC_Switch|THINGSPEC_ThingTargets
	+USESPECIAL
	+SOLID
	States
	{
	Spawn:
		TROO A 1
		Loop
	Active:
		// Prevent multiple dialogue activations
		TNT1 A 0 A_ChangeFlag("USESPECIAL", false)
		
		//
		// Make sure that you use ACS_NamedExecuteWithResult or CallACS for all scripts or
		// you will run into issues.
		//
		// Syntax:
		// SetHoldTime, <Hold Time>
		// AddDialogue, <Character ID>, <Graphic ID>, <Dialogue ID>
		// AddChoice, <Language Choice ID>, <Custom Choice ID>
		// AddChoiceStart, <Type>, <Numbered> // Type 0 = Default, Type 1 = Alternate, Type 2 = Default with UI, Type 3 = Alternate with UI
		// AddChoiceEnd // This script is a marker, so it takes no arguments
		// ChoiceMade // This script will return the custom choice ID of the last choice the player made
		// ChoiceDone // This script should be used inside A_JumpIf and followed by a "Wait"
		// IsQueueEmpty // This script should be used inside A_JumpIf and followed by a "Wait"
		// ForceFadeIn // Forces the next piece of dialogue to fade in
		// SetGraphicSize, <Width>, <Height>
		// SetGraphicPos, <X Pos> * 65536, <Y Pos> * 65536
		// VoiceActingOn // Turns on voice acting
		// VoiceActingOff // Turns off voice acting
		// SetFadeTime, <Fade Time> * 65536
		//
		// Commands that take floating point values (such as SetGraphicPos and SetFadeTime)
		// must have " * 65536" after the values. This is due to a decorate limitation.
		//
		// The "custom choice ID" is a number that you associate with the choice,
		// which is unique to the choice. "ChoiceMade" uses this number rather
		// than the language choice ID because this way it allows more control over conversations.
		//
		// If the Graphic ID is -1, then no graphic will be displayed.
		// This is useful if you want to have dialogue with no portrait.
		//
		TNT1 A 0 A_FaceTarget
		// Using SetGraphicSize, SetGraphicPos and SetFadeTime isn't really required here, as "1280x960", "0.5, 0.5" and "1.0" are the default values.
		// They are only used here to show HOW to use them. Also note that SetFadeTime isn't really useful outside of the VERY beginning of a dialogue,
		// or in the rare case that ForceFadeIn gets used.
		TNT1 A 0 ACS_NamedExecuteWithResult("SetFadeTime", 1.0 * 65536)
		TNT1 A 0 ACS_NamedExecuteWithResult("SetGraphicSize", 1280, 960) // Mr. Nobody's graphic is 1280x960 pixels
		TNT1 A 0 ACS_NamedExecuteWithResult("SetGraphicPos", 0.5 * 65536, 0.5 * 65536) // Set character portrait position to 0.5, 0.5
		TNT1 A 0 ACS_NamedExecuteWithResult("SetHoldTime", -1)
		TNT1 A 0 ACS_NamedExecuteWithResult("AddDialogue", VNC_MRNOBODY, 1, 1)
		TNT1 A 0 ACS_NamedExecuteWithResult("AddChoiceStart") // Using AddChoiceStart with no parameters will make it use type 0 without numbering
		TNT1 A 0 ACS_NamedExecuteWithResult("AddChoice", 1, 1)
		TNT1 A 0 ACS_NamedExecuteWithResult("AddChoice", 2, 2)
		TNT1 A 0 ACS_NamedExecuteWithResult("AddChoiceEnd")
		TROO A 1 A_JumpIf(CallACS("ChoiceDone"), 1)
		Wait
		TNT1 A 0 A_JumpIf(1 == CallACS("ChoiceMade"), "Choice1")
		TNT1 A 0 A_JumpIf(2 == CallACS("ChoiceMade"), "Choice2")
		Goto Done
		
	Choice1:
		TNT1 A 0 ACS_NamedExecuteWithResult("AddDialogue", VNC_MRNOBODY, 1, 2)
		Goto Done
	Choice2:
		TNT1 A 0 ACS_NamedExecuteWithResult("AddDialogue", VNC_MRNOBODY, 1, 3)
		Goto Done
		
	Done:
		// Here we wait for the stack to empty (all dialogue to be cleared)
		TROO A 1 A_JumpIf(CallACS("IsQueueEmpty"), 1)
		Wait
		TNT1 A 0 A_ChangeFlag("USESPECIAL", true)
		Goto Spawn
	}
}
```

Something that I neglected to mention in the comments, is that if you set the hold time to -1, then the dialogue will not automatically advance.

##### How do I compile the source code?
In order to compile the ZKVN source code, you will need to get a copy of GDCC, which is a C to ACS compiler.
GDCC is open-source software, its source code can be obtained at:
[https://github.com/DavidPH/GDCC](https://github.com/DavidPH/GDCC)

1. Download the latest version of GDCC from [here](https://www.dropbox.com/sh/e4msp35vxp61ztg/AAALcmttOua20tkcs82NoElWa). The latest version will always be at the bottom. Get either the "Win32" or "Win64" version depending on your computer.
2. Extract the archive to the /source/GDCC/ folder, which should be located wherever the ZKVN source is.
3. To compile for ZDoom and GZDoom, click on "build-zdoom.bat". To compile for Zandronum 2.0 and up, click on "build-zan.bat".
4. Wait for the compile process to finish, and then you should find a freshly compiled "zkvn.bin" in the "acs" folder.
