#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define NOC 8
#define MAXPOWER 0.05
#define LESSTHAN 0.00000005
#define PI 3.141092653589
#define DEG2RAD(deg) (deg * PI / 180)

void render(void);
int score = 30;
bool choice_black = true;
int number_of_coins = NOC;
bool first = true;
bool right_button_down = false;
bool start = false;
bool gameOver = false;
bool win = false;
int redHitIn = 0,whiteHitIn = 0,blackHitIn = 0;
int flags[NOC + 1][NOC + 1] = {0};
int strikerflags[NOC + 1] = {0};
class Striker;

char* myprintfint(int integer){
 	if(integer == 0)
 	{
 		return "0";
 	}
 	bool isNeg = false;
 	if(integer < 0)
 	{
 		isNeg = true;
 		integer *= -1;
 	}
 	char number[1000];
 	int i=0, k = integer;
 	while(k !=0)
 	{
 		k=k/10;
 		i++;
 	}
 	number[i--]='\0';
 	while(integer!=0)
 	{
 		number[i]=integer%10 + '0';
 		integer=integer/10;
 		i--;
 	}
 	//printf("Check: %s\n",number);
 	if(isNeg)
 	{
 		char negnumber[1000];
 		negnumber[0] = '\0';
 		strcat(negnumber,"-");
 		strcat(negnumber,number);
 		return negnumber;
 	}
 	return number;
}

class Vector{
	public:
		float x;
		float y;
	public:
		void make(float x1, float y1)
		{
			x = x1;
			y = y1;
		}
		float dotproduct(Vector v2)
		{
			return x*v2.x + y*v2.y;
		}
		float mod()
		{
			return sqrt(x*x + y*y);
		}
		Vector projection(Vector v2)
		{
			Vector result;
			float mag;
			mag = (x*v2.x + y*v2.y);
			float mod;
			mod = (x*x + y*y);
			if(mod)
			{
				mag = mag/mod;
				result.x = x*mag;
				result.y = y*mag;
			}
			else{
				result.x = 0;
				result.y = 0;
			}
			return result;
		}
		Vector add(Vector v2)
		{
			Vector result;
			result.x = x + v2.x;
			result.y = y + v2.y;
			return result;
		}
		Vector sub(Vector v2)
		{
			Vector result;
			result.x = x - v2.x;
			result.y = y - v2.y;
			return result;
		}
		void print()
		{
			printf("( %f , %f )\n",x,y);
		}
		void scale(float k)
		{
			x = x*k;
			y = y*k;
		}
};

class Board{
	public:
		int width;
		int length;
		float innerBoard;
		float innerCircle;
		float pocketCircle;
		float redBoard;
		float pocketPos;
		float redCircleRad;
		float redCirclePos;
		float coeff_of_friction;
		int level = 0;
		int friction = 0;
	public:
		void init()
		{
			// Initialize the board with some default values
			width = 640;
			length = 640;
			innerBoard = 0.9f;
			innerCircle = 0.2f;
			pocketCircle = 0.0455f;
			redBoard = 0.65f;
			pocketPos = 0.87f;
			redCircleRad = 0.03f;
			redCirclePos = 0.62f;
			coeff_of_friction = 0.2;
		}
		void makeCircle(float x, float rad, float y){
			//Helper method to draw circles
			float circlex,circley;
			
			 glPushMatrix();
			 glTranslatef(x, y, 0.0f);
			 glBegin(GL_LINES);
			 for (int i = 0; i < 360; i++)
			 {
				 circlex = rad * cos(i);
				 circley = rad * sin(i);
				 glVertex3f(circlex,circley,0);
				 glVertex3f(0,0,0);
				 circlex = rad * cos(i + 0.1);
				 circley = rad * sin(i + 0.1);
				 glVertex3f(circlex,circley,0);
				 glVertex3f(0,0,0);
			 }
			 glEnd();
			glPopMatrix();
		}
		void makeEmptyCircle(float x, float rad, float y){
			// Helper method to draw Empty cirlces
			float circlex,circley;
			 glPushMatrix();
			 glTranslatef(x, y, 0.0f);
			  glBegin(GL_LINES);
			 for (int i = 0; i < 360; i++)
			 {
				 circlex = rad * cos(i);
				 circley = rad * sin(i);
				 glVertex3f(circlex,circley,0);
				 circlex = rad * cos(i + 0.1);
				 circley = rad * sin(i + 0.1);
				 glVertex3f(circlex,circley,0);
			 }
			 glEnd();
			glPopMatrix();
		}
		void makeBoard(){
			    // draw outer board
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glColor3ub( 89, 62, 26);
				glRectf(-0.990f,0.990f, 0.990f, -0.990f);

			    //draw inner board
			    if(friction == 0)
					glColor3ub(210, 180, 140);
				else if(friction == 1)
					glColor3ub(182, 153, 117);
				else if(friction == 2)
					glColor3ub(128, 128, 0);
				glRectf(-innerBoard,innerBoard, innerBoard, -innerBoard);
			    //draw red inner lines
			    glColor3ub( 105 ,38,28 );
			    glBegin(GL_LINE_LOOP);
			    glVertex2f( -redBoard, redBoard );
			    glVertex2f( -redBoard, -redBoard );
			    glVertex2f( redBoard, -redBoard );
			    glVertex2f( redBoard, redBoard );
			    glEnd();

			    //draw red inner cirlces
			    makeEmptyCircle(-redCirclePos,redCircleRad,redCirclePos);
			    makeEmptyCircle(redCirclePos,redCircleRad,-redCirclePos);
			    makeEmptyCircle(redCirclePos,redCircleRad,redCirclePos);
			    makeEmptyCircle(-redCirclePos,redCircleRad,-redCirclePos);

			    //draw red center design
			    makeEmptyCircle(0,innerCircle,0);
			    makeEmptyCircle(0,innerCircle/4,0);
			    makeEmptyCircle(0,innerCircle/2,0);
			    makeEmptyCircle(0,3*innerCircle/4,0);

			    //draw pockets
			    glColor3f(0.5f, 0.5f, 0.5f);
			    makeCircle(-pocketPos,pocketCircle,pocketPos);
			    makeCircle(pocketPos,pocketCircle,-pocketPos);
			    makeCircle(pocketPos,pocketCircle,pocketPos);
			    makeCircle(-pocketPos,pocketCircle,-pocketPos);
		}
		void printText(int len, char* scoreprefix, int x, int y)
		{
			// Helper Method to print text
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D( 0, width, 0, length );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();
			glRasterPos2i( x , y );  // move x and y pixels from the left and bottom edges
			for ( int i = 0; i < len; ++i ) {
			    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreprefix[i]);
			}
			glPopMatrix();
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
			// printf("%s\n",scoreprefix);
		}
		void printTextSmall(int len, char* scoreprefix, int x, int y)
		{
			// Helper method to print text in a small font
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D( 0, width, 0, length );
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();
			glRasterPos2i( x , y );  // move x and y pixels from the left and bottom edges
			for ( int i = 0; i < len; ++i ) {
			    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, scoreprefix[i]);
			}
			glPopMatrix();
			glMatrixMode( GL_PROJECTION );
			glPopMatrix();
			glMatrixMode( GL_MODELVIEW );
			// printf("%s\n",scoreprefix);
		}
		void printScore()
		{
			// To print the score. Int score is made to a string and passed to helper method
			char scoreprefix[20];
			char* str = (char*)malloc(sizeof(char)*10);
			str = myprintfint(score);
			scoreprefix[0] = '\0';
			strcat(scoreprefix,"Score: ");
			strcat(scoreprefix,str);
			int len = strlen(scoreprefix);
			printText(len, scoreprefix, width/2 - width/50, length - length/25);
			if(choice_black == true)
			{
				char mess10[] = "You chose: Black";
				printTextSmall(strlen(mess10), mess10, 20, length - length/25);
			}
			else
			{
				char mess10[] = "You chose: White";
				printTextSmall(strlen(mess10), mess10, 20, length - length/25);
			}	
		}
		void gameOver()
		{
			// gameOver
			start = false;
			glColor3ub(255,255,255);
			if(win == true)
			{
				char mess10[] = "You won!";
				printText(strlen(mess10), mess10, width/2 - 10, length - 25 - 25);
			}
			else
			{
				char mess10[] = "You lost!";
				printText(strlen(mess10), mess10, width/2 - 10, length - 25 - 25);
			}
			printScore();
		}
		void startUp(){
			//Bad way but only way to add text/instructions to the start screen
			glColor3ub(255,255,255);
			char mess[] = "Welcome to Carroms! Press 's' to start playing!";
			printText(strlen(mess), mess, width/20 + 10, length - 25);
			char mess2[] = "Keyboard Controls:";
			printText(strlen(mess2), mess2, width/20 + 15, length - 25 - 20);
			char mess3[] = "Use the left and right arrow keys to move the striker";
			printText(strlen(mess3), mess3, width/20 + 15, length - 25 - 40);
			char mess4[] = "Use the up and down arrow keys to change the power of the striker";
			printText(strlen(mess4), mess4, width/20 + 15, length - 25 - 60);
			char mess5[] = "Use the 'a' and 'c' keys to change the direction of the striker";
			printText(strlen(mess5), mess5, width/20 + 15, length - 25 - 80);
			char mess6[] = "Mouse Controls:";
			printText(strlen(mess6), mess6, width/20 + 10, length - 25 - 100);
			char mess7[] = "Right click and drag to move the striker";
			printText(strlen(mess7), mess7, width/20 + 15, length - 25 - 120);
			char mess8[] = "Release the left mouse button to change the power and direction.";
			printText(strlen(mess8), mess8, width/20 + 15, length - 25 - 140);
			char mess9[] = "Press 'b' for black and 'w' for white";
			printText(strlen(mess9), mess9, width/20 + 10, length - 25 - 180);
			if(choice_black == true)
			{
				char mess10[] = "You chose: Black";
				printText(strlen(mess10), mess10, width/20 + 10, length - 25 - 200);
			}
			else
			{
				char mess10[] = "You chose: White";
				printText(strlen(mess10), mess10, width/20 + 10, length - 25 - 200);
			}
			char mess11[] = "Press 'f' to change the surface";
			printText(strlen(mess11), mess11, width/20 + 10, length - 25 - 220);
			if(friction == 0)
			{
				char mess10[] = "You chose: Smooth";
				printText(strlen(mess10), mess10, width/20 + 10, length - 25 - 240);
			}
			else if(friction == 1)
			{
				char mess10[] = "You chose: Normal";
				printText(strlen(mess10), mess10, width/20 + 10, length - 25 - 240);
			}
			else if(friction == 2)
			{
				char mess10[] = "You chose: Rough";
				printText(strlen(mess10), mess10, width/20 + 10, length - 25 - 240);
			}
			char mess12[] = "Press 'l' to change the difficulty";
			printText(strlen(mess12), mess12, width/20 + 10, length - 25 - 260);
			if(level == 0)
			{
				char mess10[] = "You chose: Easy";
				printText(strlen(mess10), mess10, width/20 + 10, length - 25 - 280);
			}
			else if(level == 1)
			{
				char mess10[] = "You chose: Medium";
				printText(strlen(mess10), mess10, width/20 + 10, length - 25 - 280);
			}
			else if(level == 2)
			{
				char mess10[] = "You chose: Hard";
				printText(strlen(mess10), mess10, width/20 + 10, length - 25 - 280);
			}
		}
		void displayCoinDock()
		{
			//Display the coins on the side of the board. You have to know what you hit in!;
			float y = 0;
			y = -innerBoard - 0.05;
			float coinRad = 0.035;
			float x = -0.8;
			for (int i = 0; i < blackHitIn; ++i)
			{
				glColor3ub(0,0,0);
				makeCircle(x+i*0.08,coinRad,y);
			}
			for (int i = 0; i < whiteHitIn; ++i)
			{
				glColor3ub(255,255,255);
				makeCircle(x+(i+blackHitIn)*0.08,coinRad,y);
			}
			for (int i = 0; i < redHitIn; ++i)
			{
				 glColor3ub( 105 ,38,28 );
				makeCircle(x+(i+whiteHitIn + blackHitIn)*0.08,coinRad,y);
			}
		}
		void setFriction()
		{
			// set Friction based on the surface chosen
			if(friction == 0)
				coeff_of_friction = 0.4;
			else if(friction == 1)
				coeff_of_friction = 0.6;
			else if(friction == 2)
				coeff_of_friction = 0.8;
		}
};

class Coin{
	public:
		float x;
		float y;
		float vx;
		float vy;
		float rad;
		float mass;
		bool moving;
		bool striker;
	public:
		void create(float ix, float iy, float ivx, float ivy, float irad, bool istriker){
			x = ix;
			y = iy;
			x = ix;
			y = iy;
			rad = irad;
			moving = false;
			striker = istriker;
			mass = 0.4;
		}
		void makeDisplay()
		{
			// render coin to screen
			 float circlex,circley;
			 glPushMatrix();
			 glTranslatef(x, y, 0.0f);
			 glBegin(GL_LINES);
			 for (int i = 0; i < 360; i++)
			 {
				 circlex = rad * cos(i);
				 circley = rad * sin(i);
				 glVertex3f(circlex,circley,0);
				 glVertex3f(0,0,0);
				 circlex = rad * cos(i + 0.1);
				 circley = rad * sin(i + 0.1);
				 glVertex3f(circlex,circley,0);
				 glVertex3f(0,0,0);
			 }
			 glEnd();
			 glBegin(GL_POINTS);
			 for(int i = 0; i<360; i++)
			 {	
			 	float rad1 = rad;
			 	float rad2 = (rad * 2)/3;
			 	float rad3 = (rad)/3;
			 	glColor3ub( 125, 125, 125 );
				glVertex3f(rad1*cos(i),rad1*sin(i),0);
				glVertex3f(rad2*cos(i),rad2*sin(i),0);
				glVertex3f(rad3*cos(i),rad3*sin(i),0);

			 }
			 glEnd();
			glPopMatrix();	
		}
		float speed()
		{
		    return sqrt(x*x + y*y);
		}
		void checkWalls(Board board){
			if(x + rad + 0.01 >= board.innerBoard  || x - rad - 0.01 <= -board.innerBoard)
			{
			   vx *= -1;
			   if(x > 0)
			   		x = board.innerBoard - rad - 0.01;
			   else if(x < 0)
			   		x = -board.innerBoard + rad + 0.01;
			}
			if(y + rad + 0.01 >= board.innerBoard || y - rad - 0.01 <= -board.innerBoard)
			{
				vy *= -1;
				if(y > 0)
					y = board.innerBoard - rad - 0.01;
				else if(y < 0)
					y = -board.innerBoard + rad + 0.01;
			}
		}
		void friction(Board board)
		{
			float friction_force;
			friction_force = board.coeff_of_friction * mass;
			//std::cout << friction_force << std::endl;
			float times;
			times = .001;
			x += vx;
			y += vy;
			// float theta = atanf((((vy)/(vx)) * 180)/PI);
			// printf("vx = %f ff = %f\n",vx, friction_force*times*cos(theta) );
			// printf("vy = %f ff = %f\n",vy, friction_force*times*sin(theta) );
			if(vx < 0)
			{
			    vx += friction_force*times;
			    if(vx > 0)
			        moving = false;
			}
			else if(vx > 0)
			{
			    vx -= friction_force*times;
			     if(vx < 0)
			        moving = false;
			}
			if(vy < 0)
			{
			    vy += friction_force*times;
			     if(vy > 0)
			        moving = false;
			}
			else if(vy > 0)
			{
			    vy -= friction_force*times;
			     if(vy < 0)
			        moving = false;
			}
			if((fabs(vx)) <= LESSTHAN )  
			    vx = 0;
			if((fabs(vy)) <= LESSTHAN )
			    vy = 0;

			if(vx == 0 && vy == 0)
			    moving = false;

			if(moving == false)
			{
				vx = 0;
				vy = 0;
			}

			if(moving == false && striker == true)
			{
				x =  0.4;
				y = -0.61;
			}
		}
};

class Carrom: public Coin{
	public:
		bool black;
		bool red;
		bool hitIn = false;
	public:
		void fallIn(Board board)
		{
			for(int i=0;i<4;i++)
			{
				int a,b;
				a = ((i&1)==1)? 1:-1;
				b = (((i>>1)&1)==1)? 1:-1;
				if(sqrt((x - a*board.pocketPos)*(x - a*board.pocketPos) + (y - b*board.pocketPos)*(y - b*board.pocketPos)) <= (rad))
				{
					x = -5;
					y = -5;
					vx = 0;
					vy = 0;
					moving=false;
					hitIn = true;
					if(black == true)
					{
						blackHitIn= blackHitIn + 1;
						if(choice_black == true)
						{
							score += 10;
							if(blackHitIn == number_of_coins/2 && redHitIn == 1)
							{
								gameOver = true;
								win = true;
								board.gameOver();
							}
						}
						else
							score -=5;
					}
					else if(red == true)
					{
						redHitIn=redHitIn + 1;
						score += 50;
					}
					else
					{
						whiteHitIn= whiteHitIn + 1;
						if(choice_black == false)
						{
							score += 10;
							if(whiteHitIn == number_of_coins/2 && redHitIn == 1)
							{
								gameOver = true;
								win = true;
								board.gameOver();
							}
						}
						else
							score -=5;
					}
				}
			}
		}
		Carrom collideWithCarrom(Carrom coin, int i, int j)
		{
			
			Vector join;
			join.make(x-coin.x,y-coin.y);

			if(flags[i][j] == 1 || flags[j][i] == 1)
				return coin;

			if(join.mod() <= (rad + coin.rad))
			{
				flags[i][j] = 1;
				flags[j][i] = 1;
				moving = false;
					Vector Uy, Ux, CUy, CUx, Vy, Vx, CVy, CVx;
					Vector Xaxis, Yaxis;
					Xaxis.make(1,0);
					Yaxis.make(0,1);
					Ux.make(vx,0);
					Uy.make(0,vy);
					CUx.make(coin.vx,0);
					CUy.make(0,coin.vy);
					
					Vector R,CR, Rejx, CRejx, Rejy, CRejy;
					Rejx = Ux.sub(join.projection(Ux));
					Rejy = Uy.sub(join.projection(Uy));
					CRejx = CUx.sub(join.projection(CUx));
					CRejy = CUy.sub(join.projection(CUy));
					R = join.projection(Uy).add(join.projection(Ux));
					CR = join.projection(CUy).add(join.projection(CUx));


					Vx = Xaxis.projection(CR);
					Vx = Vx.add(Xaxis.projection(Rejx));
					Vx = Vx.add(Xaxis.projection(Rejy));
					Vy = Yaxis.projection(CR);
					Vy = Vy.add(Yaxis.projection(Rejx));
					Vy = Vy.add(Yaxis.projection(Rejy));
					

					CVx = Xaxis.projection(R);
					CVx = CVx.add(Xaxis.projection(CRejx));
					CVx = CVx.add(Xaxis.projection(CRejy));
					CVy = Yaxis.projection(R);
					CVy = CVy.add(Yaxis.projection(CRejx));
					CVy = CVy.add(Yaxis.projection(CRejy));
					

					vx = Vx.x;
					vy = Vy.y;
					coin.vx = CVx.x;
					coin.vy = CVy.y;

					vx *= 1.1;
					vy *= 1.1;
					coin.vx *= 0.99;
					coin.vy *= 0.99;
					
				moving = true;
				coin.moving = true;
				flags[i][j] = 0;
				flags[j][i] = 0;

			}
			return coin;

		}
};

class Striker: public Coin{
	public:
		float theta;
		float power;
		float allow;
	public:
		void init()
		{
			theta = 45.0 + 75.0;
			power =  0.02;
			mass = 0.4;
			allow = true;
		}
		void drawGuideline(bool coor, float x1, float y1)
		{
			if(!coor)
			{
				if(power > MAXPOWER)
					power = MAXPOWER;
				//printf("power %f\n",power );
				glColor3ub( 0, 255, 0 );
				float distance = 5;
				float x2,y2;
				glPushMatrix();
				glTranslatef(x, y, 0.0f);
				y2 = sin(theta * PI / 180) * distance * power;
				x2 = cos(DEG2RAD(theta)) * distance * power;
				glBegin(GL_LINES);
			    glVertex2f( 0, 0 );
			    glVertex2f( x2, y2 );
			    glEnd();
			    glPopMatrix();
			}
			else
			{
				float distance = sqrt((x-x1)*(x-x1) + (y-y1)*(y-y1));
				float scaler = 4;
				power = distance/scaler;
				if(power > MAXPOWER)
					power = MAXPOWER;
				theta = (atanf((y - y1)/(x - x1)) * 180)/PI;
				if(x1 < x && y1 > y)
					theta += 180;
				if(x1 < x && y1 < y)
					theta += 180;
				if(x1 > x && y1 < y)
					theta += 360;
			}
		}
		void penalty(Board board)
		{
			for(int i=0;i<4;i++)
			{
				int a,b;
				a = ((i&1)==1)? 1:-1;
				b = (((i>>1)&1)==1)? 1:-1;
				Vector join;
				join.make(x-a*board.pocketPos,y-b*board.pocketPos);
			if(join.mod() <= (rad))
			{
				x = 0.4;
				y = -0.61;
				moving=false;
				score -= 5;
			}
			}
		}
		Carrom collideWithCarrom(Carrom coin, int i)
		{
			Vector join;
			join.make(x-coin.x,y-coin.y);
			// if(strikerflags[i] == 1)
			// 	return coin;

			if(join.mod() <= (rad + coin.rad))
			{
				strikerflags[i] = 1;
				moving = false;
				Vector v1,v2,x1,x2;
				Vector i1,i2;
				v1.make(vx,vy);
				v2.make(coin.vx,coin.vy);
				x1.make(x,y);
				x2.make(coin.x,coin.y);

				i1 = v1.sub(v2);
				i2 = x1.sub(x2);
				float dotproduct;
				dotproduct = i1.dotproduct(i2);
				float modsq = i2.mod();
				// if (modsq)
				// {
					dotproduct = dotproduct/(modsq*modsq);
					dotproduct = dotproduct * (2*coin.mass)/(coin.mass + mass);
					i2.scale(dotproduct);
					i1 = v1.sub(i2);
					vx = i1.x;
					vy = i1.y;
				// }
				// else
				// {
				// 	vx = 0;
				// 	vy = 0;
				// }

				i1 = v2.sub(v1);
				i2 = x2.sub(x1);
				dotproduct = i1.dotproduct(i2);
				modsq = i2.mod();
				// if (modsq)
				// {
					dotproduct = dotproduct/(modsq*modsq);
					dotproduct = dotproduct * (2*mass)/(coin.mass + mass);
					i2.scale(dotproduct);
					i1 = v2.sub(i2);
					coin.vx = i1.x;
					coin.vy = i1.y;
				// }
				// else
				// {
				// 	coin.vx = 0;
				// 	coin.vy = 0;
				// }

				coin.vx *= 1.1;
				coin.vy *= 1.1;
				vx *= 0.99;
				moving = true;
				coin.moving = true;
				strikerflags[i] = 0;
			}
			return coin;
		}

		void hoverOver(Carrom* coins)
		{
			Vector join;
			for(int i=0; i<=number_of_coins; i++)
			{
				join.make(x-coins[i].x,y-coins[i].y);
				if(join.mod() <= (rad + coins[i].rad))
					allow = allow && false;
				else
					allow = allow && true;
			}
		}
};

Striker striker;
Board board;
Carrom coins[NOC + 1];

void updateStriker(int value){
	// if the striker is moving then apply friction and check boundaries. If it falls into the hole check penalty and check any collisions
		if(striker.moving)
		{
	    	striker.friction(board);
	    	striker.checkWalls(board);
	        striker.penalty(board);
	        for(int i=0; i<=number_of_coins; i++)
	        	coins[i] = striker.collideWithCarrom(coins[i],i);
		}
		// if allow is set to true then the striker is allowed to shoot
		striker.allow =  true;
		// if the coin is moving then apply friction and check boundaries. If it falls into the hole check penalty and check any collisions
		for(int i=0; i<=number_of_coins; i++)
		{
			if(coins[i].moving && !coins[i].hitIn)
			{
				coins[i].friction(board);
				coins[i].checkWalls(board);
				coins[i].fallIn(board);
				coins[i] = striker.collideWithCarrom(coins[i],i);
			}
			for(int j=0; j<=number_of_coins && j!=i ; j++)
			{
				coins[j] = coins[i].collideWithCarrom(coins[j],i,j);
			}
		}
		// check the striker is not overlapping any coins when the user moves to shoot
		if(striker.moving == false)
			striker.hoverOver(coins);
		// if striker is placed on a coin allow to soot
		if(striker.x == 0.4 && striker.y == -0.61)
			striker.allow = true;
		// call this function in 5 miliseconds
		 glutTimerFunc(5, updateStriker, 0);
}

void handleKeypress1(unsigned char key, int x, int y){
	if(striker.moving == false && start == true)
	{
	    if(key == 32 && striker.allow == true)
	    {
	    	striker.moving = true;
	        striker.vy = striker.power * sin(striker.theta * PI / 180);
	        striker.vx = striker.power * cos(striker.theta * PI / 180);
	    }
	    if(key == 'a')
	    {
	    	striker.theta = striker.theta + 1;   
	    }
	    if(key == 'c')
	    {
	    	striker.theta = striker.theta - 1;
	    }
	}
    if(key == 27)
    	exit(0);
    if(key == 's' && start == false)
    {
    	start = true;
    }
     if(key == 'w' && start == false)
    {
    	choice_black = false;
    }
     if(key == 'b' && start == false)
    {
    	choice_black = true;
    }
     if(key == 'f' && start == false)
    {
    	board.friction = (board.friction + 1)%3;
    	board.setFriction();
    }
     if(key == 'l' && start == false)
    {
    	board.level = (board.level + 1)%3;
    	if(board.level == 2)
    		score = 30;
    	else if(board.level == 1)
    		score = 50;
    }
}

void handleKeypress2(int key, int x, int y){
	if(striker.moving == false && striker.allow == true)
	{
		 if (key == GLUT_KEY_RIGHT) {
		 	if(striker.x + striker.rad +0.01 <= board.redBoard)
		 		striker.x += 0.01;
		 }
		 else if (key == GLUT_KEY_LEFT) {
		 	if(striker.x - striker.rad - 0.01 >= -board.redBoard)
		 		striker.x -= 0.01;
		 }
		 else if (key == GLUT_KEY_UP) {
		 	if(striker.power < MAXPOWER)
	            striker.power += 0.001;
	        if(striker.power > MAXPOWER)
					striker.power = MAXPOWER;
		 }
		 else if (key == GLUT_KEY_DOWN) {
		 	if(striker.power > 0.009)
		 	    striker.power -= 0.001;
		 	if(striker.power < 0.009)
		 	    striker.power = 0.009;

		 }
	}
}

void handleMouseclick(int button, int state, int x, int y){
	if(start == false)
		return;
	if(striker.moving == false && (striker.x == 0.4 && striker.y == -0.61 || striker.allow == true))
	{
	switch(button){
	        case GLUT_LEFT_BUTTON:
	            if(state == GLUT_DOWN){
	                float ax, ay;
	                ax = x - 320;
	                ax = ax/320;
	                ay = y - 320;
	                ay = -ay/320;
	                striker.drawGuideline(true,ax,ay);
	                 glutPostRedisplay();
	            }
	            else if(state == GLUT_UP)
	            {
	            	striker.moving = true;
	            	striker.vy = striker.power * sin(striker.theta * PI / 180);
	            	striker.vx = striker.power * cos(striker.theta * PI / 180);
	            }
			case GLUT_RIGHT_BUTTON:
				if(state == GLUT_DOWN)
					right_button_down = true;
				else if(state == GLUT_UP)
					right_button_down = false;
	 		break;
	    }
	}
}

void motionCallBack(int x, int y){
	if(start == false)
		return;
	if(right_button_down && striker.moving == false)
	{
		float ax, ay;
		ax = x - 320;
		ax = ax/320;
		striker.x = ax;
		if(striker.x - striker.rad - 0.01 >= -board.redBoard && striker.x + striker.rad +0.01 <= board.redBoard )
			striker.x = ax;
		else
			striker.x = 0;
		 glutPostRedisplay();
	}
}

void initCoins(){
	    float design_rad = board.innerCircle/2;
	    if(first)
	    {
	    	float angle = 360/number_of_coins;
	    	for(int i=0; i<=number_of_coins; i++)
		    {
		    	if(i == 0)
		    	{
		    		coins[i].create(0.0 , -0.0, 0.0, 0.0, 0.035,false);
		    		coins[i].red = true;
		    		coins[i].black = false;
		    	}
		    	else
		    	{
		    		coins[i].create(design_rad*sin(angle*(i-1) * PI / 180) , design_rad*cos(angle*(i-1) * PI / 180), 0.0, 0.0, 0.035,false);
		    		coins[i].red = false;
		    		if(i%2 == 1)
		    			coins[i].black = true;
		    		else
		    			coins[i].black = false;
		    	}
		    }
		    first = false;
		}
}

void scoreMin(int value){
	if(start == true && board.level != 0)
		score -=1;
	if(score!=0)
    	glutTimerFunc(1000, scoreMin, 0);
    else
    	gameOver = true;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    board.init();
    striker.init();
    glutInitWindowSize(board.width, board.length);
    glutCreateWindow("Play Carroms!");
    striker.create(0.4, -0.61, 0.01, 0.01, 0.04,true);
    initCoins();
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutKeyboardFunc(handleKeypress1);
    glutSpecialFunc(handleKeypress2);
    glutMouseFunc(handleMouseclick);
    glutMotionFunc(motionCallBack);
    glutTimerFunc(10, updateStriker, 0);
    glutTimerFunc(1000, scoreMin, 0);
    glutMainLoop();
}


void render(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(start == true && gameOver == false)
	{
		board.makeBoard();
		if(striker.allow)
			glColor3ub( 255, 255, 200 );
		else
			glColor3ub(0,255,0);
		striker.makeDisplay();
		for(int i=0; i<=number_of_coins; i++)
	    {
	    	if(coins[i].red == true)
	    		 glColor3ub( 105 ,38,28 );
	    	else if(coins[i].black == true)
	    		glColor3ub(0,0,0);
	    	else
	    		glColor3ub(255,255,255);
	    	if(coins[i].hitIn == false)
	    		coins[i].makeDisplay();
	    }
		if(striker.moving  == false)
			striker.drawGuideline(false,0,0);
		glColor3f(1.0f, 1.0f, 1.0f);
		board.printScore();
		board.displayCoinDock();
	}
	else if(start == false && gameOver == false)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		board.startUp();
	}
	else if(gameOver == true)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		board.gameOver();

	}
    glutSwapBuffers();
}

