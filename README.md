# Carroms
Carroms made in OpenGL2 - Developed on a Mac

To compile on a mac: g++ carroms.cpp  -framework GLUT -framework OpenGL
To run: ./a.out

This carroms game has the following controls:

Mouse:
	Drag the striker with the right mouse button down to move it along the board.
	Click and release the left mouse button to adjust the power and direction of the strike.

Keyboard:
	Use 'a' and 'c' to adjust the direction of the strike.
	Use the up and down arrow to adjust the power of the strike.
	Use the left and right arrow to move the striker.
	Use the spacebar to shoot. 

Use the 'esc' button to exit the game at any time

At the start screen you can choose which color you would like to hit.
You must hit all the coins of your chosen color and the red coin to win.
If you hit a coin it appears in your dock at the bottom-left side of your screen.
You can choose the level at which you want to play i.e Easy, Medium and Hard using the 'l' key at the start screen.
For easy, you must hit all the coins of your chosen color.
for medium, you must hit all the coins of your chosen color before the timer runs out. (Starts at 50)
for hard, you must hit all the coins of your chosen color before the timer runs out (Starts at 30)
For both the medium and hard levels, You start with a particular score and for every second that goes by you lose a point. A coin of your color will earn you 10 points and a coin of the other color will cost you 5 points. If the striker falls into the pocket you lose 5 points.

Also at the start screen you can choose between three surfaces i.e Smooth, Normal and Rough. 
Each of these is distinguished by the friction of the surface. You can use 'f' to choose between these at the start screen.
