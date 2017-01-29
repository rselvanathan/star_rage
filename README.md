![Imgur](http://i.imgur.com/V4c6E39.jpg)

Star Rage is a 3D 3rd Person shooter game set in space. The game involves surviving the waves of enemy space fighters and captial ships as well as avoiding the asteroids that are randomly flying across space. 

This game was completed as part of the final year project and was chosen to be shown off at the 2012 'Digital Shoreditch' event in London. The project was developed from scratch only using C++ and OpenGL.

Key Features
- Physics (Kinematics, Elastic Collisions) 
- Octree Collision Detection 
- State - Based AI with utility functions allowing them to choose goals 
- 3rd Person Camera using Quaternion mathematics (to avoid "Gimbal Lock") 
- Particle Effects and Screen Decals 
- Flexible Input System (ability to remap keys and mouse settings)

Notes & Instructions

https://www.dropbox.com/s/utq8o21ke66jwvp/Star_Rage_EXECUTABLE.zip

The executable above can be run on any machine that has OpenGL installed , DirectX installed (for the input only) and windows running. The executable may ask for the "Windows Microsoft Visual C++ 2010 Redistributable Package" if it has never been installed before, which is necessary to run any C++ executable built using the VS2010 IDE. The download will come with the installation file (if the file does not work you can download it here: http://www.microsoft.com/download/en/details.aspx?id=5555) 

To view the source code it is suggested to use the VS2010 IDE, however you can use a standard "notepad" like software. The source code download will contain further notes and instructions to help navigate through the folders. 

To view the source code, with intended group/folder seperations please open the "VS10/Final_Year_Game/Final_Year_Game.sln" file. This will open both the engine and the game source files viewable on Visual Studio.

Author : Romesh Selvanathan
