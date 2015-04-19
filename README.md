#### ZKVN - A visual novel engine for ZDoom, GZDoom and Zandronum
##### What is ZKVN?
ZKVN is an easy to use visual novel engine for ZDoom and GZDoom. Zandronum 2.0 is supported as well, however automatic word wrap is not supported.
Rather than regular ACS, ZKVN is written in C, and compiled using DavidPH's GDCC.

ZKVN started development as the visual novel system for HDoom, and eventually branched off into its own project, that could be used for other mods as well. Therefore, without HDoom, ZKVN wouldn't exist.
Be sure to check out HDoom if you don't mind seeing demon ladies in lewd situations, it's very well done and the team are a great group of people. Once it starts to get its story going it's only going to get better.

##### How do I use it?
Proper documentation on how to use ZKVN is still in the works, but if you take a look at the little demo that's included with it, it shouldn't be too hard to figure out.

##### How do I compile the source code?
In order to compile the ZKVN source code, you will need to get a copy of GDCC, which is a C to ACS compiler.
GDCC is open-source software, its source code can be obtained at:
[https://github.com/DavidPH/GDCC](https://github.com/DavidPH/GDCC)

1. Download the latest version of GDCC from [here](https://www.dropbox.com/sh/e4msp35vxp61ztg/AAALcmttOua20tkcs82NoElWa). The latest version will always be at the bottom. Get either the "Win32" or "Win64" version depending on your computer.
2. Extract the archive to the /source/GDCC/ folder (create it if it doesn't exist), which should be located wherever the ZKVN source is.
3. Click on "make_pk3.bat", which will compile the source code and package everything into a pk3.
