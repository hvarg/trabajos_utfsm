#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#include <typeinfo>

//Data
GLfloat vertices[][3] ={{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
                        {1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, 
                        
                        {-1.0,-1.0,1.0}, {1.0,-1.0,1.0}, 
                        {1.0,1.0,1.0}, {-1.0,1.0,1.0}};

GLfloat normals[][3] = {{-1.0,-1.0,-1.0}, { 1.0,-1.0,-1.0},
                        { 1.0, 1.0,-1.0}, {-1.0, 1.0,-1.0}, {-1.0,-1.0,1.0},
                        { 1.0,-1.0, 1.0}, { 1.0, 1.0, 1.0}, {-1.0, 1.0,1.0}};

GLfloat colors[][3] =  {{0.0,0.2,0.5}, {1.0,0.0,0.0},
                        {1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0},
                        {1.0,0.0,1.0}, {0.5,0.5,0.5}, {0.0,1.0,1.0}};

static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 2;
static GLdouble viewer[]= {0.0, 0.0, 0.0};

//Polygon's points
void generateVertexData(void) {
    int i = 0, j = 0;
    float x = 0.0, y = 0.0;
    float rad = 1.0;
    float angle = 1.05;

    for (j = 0; j < 2; j++) {
        for (i = 0; i < 6; i++) {
            x = rad * cos(angle * i);
            y = rad * sin(angle * i);

            vertices[i + (j * 6)][0] = x;
            vertices[i + (j * 6)][1] = y;
            vertices[i + (j * 6)][2] = j + 1.0 * pow(-1, j + 2) -  5.0;

            normals[i + (j * 6)][0] = x;
            normals[i + (j * 6)][1] = y;
            normals[i + (j * 6)][2] = j + 1.0 * pow(-1, j + 2) - 5.0;
        }
    }
}

//Draw polygon's faces
void polygon(int a, int b, int c , int d, int e, int f){
    glBegin(GL_POLYGON);
		glNormal3fv(normals[a]);
			glVertex3fv(vertices[a]);
		glNormal3fv(normals[b]);
			glVertex3fv(vertices[b]);
		glNormal3fv(normals[c]);
			glVertex3fv(vertices[c]);
		glNormal3fv(normals[d]);
			glVertex3fv(vertices[d]);
        if (e != -1) {    
            glNormal3fv(normals[e]);
			    glVertex3fv(vertices[e]);
        }
        if (f != -1) {
		    glNormal3fv(normals[f]);
			    glVertex3fv(vertices[f]);
        }
	glEnd();
}

//Draw polygon
void polygonView() {	   
    glColor3fv(colors[0]);polygon(0, 1, 2, 3, 4, 5);
	glColor3fv(colors[1]);polygon(0, 1, 7, 6, -1, -1);
	glColor3fv(colors[2]);polygon(1, 2, 8, 7, -1, -1);
	glColor3fv(colors[3]);polygon(2, 3, 9, 8, -1, -1);
	glColor3fv(colors[4]);polygon(3, 4, 10, 9, -1, -1);
	glColor3fv(colors[5]);polygon(4, 5, 11, 10, -1, -1);
    glColor3fv(colors[6]);polygon(0, 5, 11, 6, -1, -1);
	glColor3fv(colors[7]);polygon(6, 7, 8, 9, 10, 11);
}

void display(void) {	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0],viewer[1],viewer[2], 0.0, 0.0, -10.0, 0.0, 1.0, 0.0);
	polygonView();
	glFlush();
	glutSwapBuffers();
}


//Idle callback function, the camera automatically moves
int state = 0;
int axisCamera = 0;
void myIdle() {	
    if (state == 0) {
        viewer[axisCamera] += 0.1;
        if (viewer[axisCamera] > 3.0) { 
            state = 1;
        }     
    } else if (state == 1){
        viewer[axisCamera] -= 0.1;
        if (viewer[axisCamera] < -3.0) {
            state = 2;
        }
    } else if (state == 2) {
        viewer[axisCamera] += 0.1;
        if (viewer[axisCamera] > 0.0) {
            axisCamera++;
            state = 0;
        }
    }

    if (axisCamera == 3) {
        axisCamera = 0;
    } 
    
    
    display();
}

void myReshape(int w, int h) {	
    glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w<=h) glFrustum(-2.0, 2.0, -2.0 * (GLfloat) h/ (GLfloat) w,
       2.0* (GLfloat) h / (GLfloat) w, 2.0, 20.0);
	else glFrustum(-2.0, 2.0, -2.0 * (GLfloat) w/ (GLfloat) h,
       2.0* (GLfloat) w / (GLfloat) h, 2.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {	
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Group 04");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    generateVertexData();
    
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
    glutIdleFunc(myIdle);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

    return 0;
}
