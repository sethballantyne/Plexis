# Plexis

**CURRENT VERSION: [0.9.1](https://github.com/sethballantyne/Plexis/blob/master/build/plexis.0.9.1.zip)**

Plexis a breakout clone I wrote in C++/CLI because I wanted to play around with DirectX and experiment with an idea I had revolving creating GUIs using XML. It's built on top of a simple engine I wrote that makes use of DirectDraw 7 for graphics, DirectSound 8 for audio and DirectInput 8 for input. The level editor is written in C#, using WinForms; you'll need the .NET Framework 4.5 or later to run both the game and the editor. All the levels currently included are just debug levels; the emphasis is on gameplay at the moment. 

The latest build of the game is in the _build_ directory. All the source code is released under the MIT license. 

## Changes in 0.10
* Fixed a bug where the ball was being attached to the bottom of the paddle if the paddle's frame changed. 
* Added the Jumbo, Shrink, Wall, Multi-ball and Fireball power ups.


## Changes in 0.9.1
* Fixed a bug where the error sound was looping whenever the player attempted to change a key binding.
* When a ball collides with two bricks at the same time, the game will now treat both bricks as being hit.
* Capped the ball speed so it can't pass through bricks or the paddle due to high speed.
* PLeD now informs users when it can't load brick sprites.
* Invisible bricks no longer need to be destroyed before the player can transition to the next level.
* Added a sound effect that plays when the player purchases ammo.

## Changes in 0.9
* The player now has a permanent laser, with an initial ammo count of 90. Laser powerups now act as ammo packs when caught, awarding 30 ammo. Both the initial value and the amount of ammo awarded when a powerup is caught are specified in the lasers `initialammo` and `pickupammo` XML attributes.
* Increased the chance of a powerup being generated by a brick when it explodes.
* Added 4 new brick types: bonus life, ammo, instadeath and indestructible.
* Each brick type in `entities.xml` can now specify whether they should be counted in the total number of bricks when 
  a level is loaded. This allows for bricks that don't need to be destroyed in order for the player to transition
  to the next level.
* Added the ability for players to purchase ammo packs at the cost of 1 life during gameplay. This is bound to the
  `L` key by default. The ammo packs are generated as powerups in the center of the screen and have to be caught.

## Changes in 0.8.2
* Scores are capped at 999,999. Previously it was 99,999. This also fixes the bug mentioned in issue 120.
* Blank levels will no longer be loaded by the game.
* A warning now appears when attempting to save blank levels in the editor.
* The editor no longer allows previewing of blank levels.
* Removed a blank button from the editor toolstrip that wasn't supposed to be there.
* The default player name has changed from "DEFAULT" to "EMPTY" when the game writes out a fresh high score file.

## Changes in 0.8.1
* The size of the particles emitted by explosions has changed from 1 pixel to a block of 4. 
* Removed the mouse sensitity option from the options menu. 
* Usability enhancements to the editor.
