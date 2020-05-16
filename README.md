# Plexis

Plexis a breakout clone I wrote in C++/CLI because I wanted to play around with DirectX and experiment with an idea I had revolving creating GUIs using XML. It's built on top of a simple engine I wrote that makes use of DirectDraw 7 for graphics, DirectSound 8 for audio and DirectInput 8 for input. The level editor is written in C#, using WinForms; you'll need the .NET Framework 4.5 or later to run both the game and the editor. All the levels currently included are just debug levels; the emphasis is on gameplay at the moment. 

The latest build of the game is in the _build_ directory. All the source code is released under the MIT license. 

## Changes in 0.8.1
* The size of the particles emitted by explosions has changed from 1 pixel to a block of 4. 
* Removed the mouse sensitity option from the options menu. 
* Usability enhancements to the editor.
