#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#include <stdbool.h>

//global values
float dx = 0.0;  //shift to y position
float dy = 0.0;  //shift to x position  
float rad = 5.0;  //ratio of polygon
//flags for state of the polygon animation
bool initMove = true;
bool moveToNorth = false;
bool moveToWest = false;
bool moveToSouth = false;
bool moveToEast = false;

void myIdle(){
   if(initMove){ // init movement from origin to the right side of window
     if(dx + rad >= 10.0) {
       initMove = false;
       moveToNorth = true;      
     }
     else {
       dx += 0.01;
     }
   }

   else if(moveToNorth){
     if(dy + rad < 10.0){
       dx -= 0.01;
       dy += 0.01;
     }
     else{
       moveToNorth = false;
       moveToWest = true;
     }
   }

   else if(moveToWest){ 
     if(dx - rad > -10.0){
       dx -= 0.01;
       dy -= 0.01;
     }
     else{
       moveToWest = false;
       moveToSouth = true;
     }
   }
   else if(moveToSouth){
     if(dy - rad > -10.0){
       dx += 0.01;
       dy -= 0.01;
     }
     else{
       moveToSouth = false;
       moveToEast = true;
     }
   }
   else if(moveToEast){ 
     if(dx + rad < 10.0){
       dx += 0.01;
       dy += 0.01;
     }
     else{
       moveToEast = false;
       moveToNorth = true;
     }
   }
   glutPostRedisplay();
}

void display(void){
   int i=0;
   float x=0.0, y=0.0;
   double angle = 1.05; //angle between points of polygon in radians
   glClear( GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POLYGON);
     for(i=0; i<6; i++){
       x = rad*cos(angle*i) + dx;
       y = rad*sin(angle*i) + dy;
       glVertex3f(x, y, 0.0);
     }	
   glEnd();
   glFlush();
}

int main(int argc, char **argv){
   glutInit(&argc, argv);
   glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

   glutInitWindowPosition(100,100);
   glutInitWindowSize(300,300);
   glutCreateWindow ("Grupo 04");

   glClearColor(1.0, 1.0, 1.0, 1.0);         // black background
   glMatrixMode(GL_PROJECTION);              // setup viewing projection
   glLoadIdentity();                           // start with identity matrix
   glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);   // setup a 20x20x2 viewing world

   glutDisplayFunc(display);
   glutIdleFunc(myIdle);
   glutMainLoop();

   return 0;
}
