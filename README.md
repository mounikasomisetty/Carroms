# Carroms
Carroms made in OpenGL2 - Developed on a Mac<br>
<br>
To compile on a mac: g++ carroms.cpp  -framework GLUT -framework OpenGL<br>
To run: ./a.out<br>
<br>
This carroms game has the following controls:<br>
<br>
<br>
Mouse:<br>
<ul>
	<li>Drag the striker with the right mouse button down to move it along the board.</li>
	<li>Click and release the left mouse button to adjust the power and direction of the strike.</li>
</ul>
<br>
Keyboard:<br>
<ul>
	<li>Use 'a' and 'c' to adjust the direction of the strike.</li>
	<li>Use the up and down arrow to adjust the power of the strike.</li>
	<li>Use the left and right arrow to move the striker.</li>
	<li>Use the spacebar to shoot. </li>
</ul>
<br>
Use the 'esc' button to exit the game at any time<br>
<br>
At the start screen you can choose which color you would like to hit.<br>
You must hit all the coins of your chosen color and the red coin to win.<br>
If you hit a coin it appears in your dock at the bottom-left side of your screen.<br>
You can choose the level at which you want to play i.e Easy, Medium and Hard using the 'l' key at the start screen.
For easy, you must hit all the coins of your chosen color.<br>
for medium, you must hit all the coins of your chosen color before the timer runs out. (Starts at 50)<br>
for hard, you must hit all the coins of your chosen color before the timer runs out (Starts at 30)<br>
For both the medium and hard levels, You start with a particular score and for every second that goes by you lose a point. A coin of your color will earn you 10 points and a coin of the other color will cost you 5 points. If the striker falls into the pocket you lose 5 points.<br>
<br>
Also at the start screen you can choose between three surfaces i.e Smooth, Normal and Rough. <br>
Each of these is distinguished by the friction of the surface. You can use 'f' to choose between these at the start screen.<br>
