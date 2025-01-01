# COMP3016_CW2

## Introduction

This is my OpenGL scene for COMP3016 CW2.
For this coursework I have created an interactable scene in which the player has a short amount of time to destroy
Section 31 ships that are around a moons space station.

## Repository Setup

The file titled OpenGL contains my visual studio solution, the source code, for the scene.
The file titled Section 31 Takeover contains an executable and all needed assets/dependencies to run the scene. (Download this if you wish to play)
The other files are for this git repository/document.

## Running The Executable

To run the executable and play you will first need to download the game folder titled Section 31 Takeover.
Once downloaded if zipped make sure to extract all (Drydock also needs unzipping, see below).
With the folder find the file named Section 31 Takeover.exe and run it.
All assets and dependencies needed should be in the folder and the scene should run.

### Zipped Drydock

Take note that in both the source code and executable file the drydock model is zipped.
If you wish to run the scene with the drydock model please unzip this file and place the drydock.obj in media/drydock where the zip was originally located.

## Video Link

## Dependencies Used

### C++ Standard Library

- Used for managing dynamic arrays, random number generation and debugging with console outputs.
- Modules Used:
	- vector
	- cstdlib
	- ctime
	- iostream

### GLAD (OpenGL Loader-Generator)

- Used to load OpenGL functions.
- Modules Used:
	- glad/glad.h

### GLM (OpenGL Mathematics)

- Used for vector, matrix operations and mathematical utilities.
- Modules Used:
	- glm/ext/vector_float3.hpp
	- glm/gtc/type_ptr.hpp

### ASSIMP (Open Asset Importer Library)

- Used for importing 3D model data and performing post processing.
- Modules Used:
	- assimp/Importer.hpp
	- assimp/scene.h
	- assimp/postprocess.h

### LearnOpenGL

- Used for managing shaders and 3D models.
- Modules Used:
	- learnopengl/shader_m.h
	- learnopengl/model.h

### IrrKlang

- Used to play audio sound effects and background ambiance.
- Modules Used:
	- irrKlang.h

## Game Programming Patterns

### State Pattern

I have used a state pattern in the lasers and section 31 ships in this scene as both of these game objects contain a state differing from their original.
For the laser it can be in either a state of being a moving laser or the state of having hit the shield of an object in space.
Depending on the state a different model is displayed and different movement occurs.
The section 31 ships work similarly but without movement. They are either a ship healthy or an exploded ship.
Once again the state determines the model displayed.

## Game Mechanics

### Ships and Shuttles

- This feature adds ships and shuttles that are either entering or exiting the station.
	- As there is no terrain in my scene generation needed to be added elsewhere.
	- For the ships and shuttles each of them have one of 2 'biomes', entering or exiting.
	- Each ship or shuttle will also have a random set of coordinates which is where they either fly to or from.
	- Station coordinates are an array of fixed paths so that ships or shuttle only enter or exit through a door.
	- Paths outside the station have randomized points and the path is calculated using these points.
	- There is also a set of shuttles that can be generated that fly between the station and a docked ship.

### Player Movement

- This feature allows the player to move their ship around the scene.
	- The player can move their ship using WASD for forwards, back, left and right.
	- The player can move their ship up and down using space and left shift.
	- The player can look around by aiming their cursor in the direction they would like to look (centring the cursor keeps the ship still).
	- Code for this mechanic can be found in the ProcessMouseInput and ProcessUserInput functions.
	- The functions consist of checking the mouse location and calculating new rotation or checking button presses and calculating new position.

### Shooting

- This feature allows the player to shoot lasers in the direction they are facing and defeat any enemies hit.
	- Checks if the fire button (left click) if held down and if the time between shots has passed.
	- Generates a laser in a direction based on the players rotation.
	- The laser has a collider for intersection checks.

### Section 31 Ships (Target Ships)

- This feature allows an amount of enemy ships to be present and destroyed if shot enough.
	- Currently these ships do not do anything they only explode when defeated.
	- They have 5 health with one shot taking off 1 health.
	- Once the health reaches 0 the ship explodes.
	- If no more section 31 ships are present the player wins

### Colliders

- This feature allows the lasers to interact with the section 31 ships and station
	- Colliders have a minimum point and maximum point which are used to create a box collider around an object.
	- Colliders have a function to check if one collider intersects with another.
	- If two colliders intersect based on the intersecting objects a different outcome occurs.
	- For the section 31 ships if they are hit by a laser but not killed the shield effect is shown, if defeated the explosion effect.
	- For the stations collider only the shield effect will be shown as the station cannot be destroyed.

### Timer

- This feature adds a countdown timer that creates a fail condition if it reaches 0
	- Using delta time calculates the time remaining so that frame rate will not effect the speed of the game.
	- Time is rendered in the UI above the station
	- When the timer reaches 0 the game fail screen is shown.

### UI above station

- This features adds UI elements above the space station to show the time and number of ships remaining.
	- The UI is rotated towards the player so that it can be read from any angle.
	- The elements are updated to show the relevant time or ships remaining.

## Sample Screens

### Menu Example

![Menu](/SampleScreens/menu.gif)

### Keyboard Movement

![Keyboard Movement](/SampleScreens/movement.gif)

### Mouse Movement

![Mouse Movement](/SampleScreens/mouse.gif)

### Shooting

![Shooting](/SampleScreens/shooting.gif)

### Section 31 Ships

![Section31](/SampleScreens/section31.gif)

### Shuttles and Ships

![Shuttles And Ships](/SampleScreens/shuttles and ships.gif)

### UI Example

![UI](/SampleScreens/ui.gif)

## Exception Handling

The main use of exception handling can be found at the very start (lines 174 - 200 of OpenGL.cpp).
This is where things like the window, sound engine and GLAD are initialised.
If any of them fail to load then the program is exited and the reason is displayed.

## Evaluation

### Achievements

I am extremely happy with what I have accomplished. 
Despite difficulties I managed to create the experience I was aiming for and more.
I have managed to add in the generation of shuttles and ships entering and exiting the station as I had planned.
Beyond this I also added in the recommended shooting feature which interacts with both the station and enemy ships as well as some UI elements and a timer.
Overall I am pleased with the scene I have made and the features implemented.

### What I Would Do Differently

Although I am very pleased with what I have made there are some improvements that I would like to make.
Firstly I would like to improve upon the section 31 aspect (the ships that get destroyed) as these ships do not move or shoot at all.
This is something I believe with what I have learnt I could now do but didn't for this coursework.
If I did this again I would also like to spend more time trying to get some PBR lighting into the scene as at the moment it feels very bright.

## Credits

### Models

All space ships, shuttles, stations and drydocks are models by ZEO CMF on Sketchfab: https://sketchfab.com/ZEOCMF
Model effects including the shield, laser and explosion are by Jason Bott using Blender and Microsoft Paint

### Skybox

Skybox by skiingpenguins / freezurbern: https://opengameart.org/content/spacemoon-skybox

### Sounds

Laser by Kronos1001 on pixabay: https://pixabay.com/sound-effects/laser-45816/
Shield Hit by Bychop on pixabay: https://pixabay.com/sound-effects/shield-recharging-107016/
Explosion by DavidDumaisAudio on pixabay: https://pixabay.com/sound-effects/sci-fi-explosion-09-190268/
Game Fail by Audiosto on pixabay: https://pixabay.com/sound-effects/wrong-answer-21-199825/
Game Win by Audiosto on pixabay: https://pixabay.com/sound-effects/the-correct-answer-33-183620/
Space Ship Background by tutarap on pixabay: https://pixabay.com/sound-effects/space-ship-102433/