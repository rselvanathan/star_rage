This is the Resources folder which contains the files that will be read by the Software.

The highscore file contains the highscore list, and the rest of the files are there for the Input System for the game.



 - ContextList - Contains all the available Input Contexts 

 - DefaultContext - Is the Default Context list, with the Default Buttons for the game. This file will be accessed whenever
		    the user wants Default Settings back. This file will only be read from and not written into.

 - MainContext - contains the Button Presses and mouse values for the gameplay itself. This file can be accessed and changed
		 (KeyBinds, mouse values) by the user through a Settings Menu.

 - MenuContext - This contains button presses for the menu Part only, and will not be accessed or changed

 - MouseRanges - Contains the default Mouse Values.


The Structure and what each Text file contains is desribed below.

-----------------------------------------------------

 		ContextList.reso

1st line - The number of contexts available

Then it will contain THe name of the context and the Source of the Context file.

-----------------------------------------------------

		DefaultContext/MainContext/MenuContext

All the context files follow the same pattern - They all have a single value, which represents the number of "x". Then in pairs
the Raw Input value will be linked to a "x" value. However a few final lines dont follow this pattern.

1st Column - with single value :
1st line - Number of States
THen, 
Raw_Button_Press with the State Value itself

2nd Column - with single value :
1st line - Number of Actions
Then,
Raw_Button_Press with the Action Value itself

3rd Column - with single value :
1st Line - Number of Mouse Axes
Then
Raw_Input_Axis with the Mouse Range (Mouse Range has another aswell, will describe later)

4th Column - with Single Value :
1st Line - Number of Mouse Ranges
2nd Line - Contains The Maximum and Minimum Sensitivity Values
Then,
THe Range with its Sensitivity Value

Final Column with the single value contains whether or not the Mouse Up/Down Turn is inverted or not.
when its "-1" it is not inverted and when its "1" it is inverted.

--------------------------------------------------------
		
		MouseRanges.reso

This file contains the actual ranges of the Mouse values.

1st line - contains the Number of ranges

Then the Range is linked with 4 values which are : Minimum Mouse Input, Maximum Mouse Input, Minimum Mouse Output, Maximum Mouse Output,
in that exact order. This is essentially clamping the Input and Output values when reading mouse values.

