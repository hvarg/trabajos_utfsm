/*
 * Tarea 5 - Grupo 04
 * Alonso Sandoval Acevedo 201073011-5
 * Hernán Vargas Leigthon 201073009-3
*/

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdio.h>

//Parámetros de las luces
const GLfloat red_light_ambient[]  = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat red_light_diffuse[]  = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat red_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat red_light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f }; 
 
const GLfloat green_light_ambient[]  = { 0.0f, 1.0f, 0.0f, 0.0f };
const GLfloat green_light_diffuse[]  = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat green_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat green_light_position[] = {-2.0f,-5.0f, 5.0f, 0.0f }; 
 
const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f }; 

 
static void resize(int width, int height){
    const float ar = (float) width / (float) height; 

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0); 
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
} 
 
//Se dibuja la esfera
static void display(void){ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(0.3,0.3,0.3); 
 
    glPushMatrix();
        glTranslated(0.0,0.0,-5);
        GLUquadric *quad;
        quad = gluNewQuadric();
        gluSphere(quad,2.0,100.0,100.0);
    glPopMatrix(); 
 
    glutSwapBuffers();
} 

//Control de las luces con las teclas
void keys(unsigned char key, int x, int y){
    if(key ==  27 || key == 'q') exit(0);
    else if(key == '0') {
        glDisable(GL_LIGHT0);
    }
    else if(key == '1') {
        glEnable(GL_LIGHT0);
    }
    else if(key == '2') {
        glDisable(GL_LIGHT1);
    }
    else if(key == '3') {
        glEnable(GL_LIGHT1);
    }
    else if(key == 'x') {
        red_light_position[0] -= 1.0;
        glLightfv(GL_LIGHT0, GL_POSITION, red_light_position);
    }
    else if(key == 'X') {
        red_light_position[0] += 1.0;
        glLightfv(GL_LIGHT0, GL_POSITION, red_light_position);
    }
    else if(key == 'y') {
        red_light_position[1] -= 1.0;
        glLightfv(GL_LIGHT0, GL_POSITION, red_light_position);
    } 
    else if(key == 'Y') {
        red_light_position[1] += 1.0;
        glLightfv(GL_LIGHT0, GL_POSITION, red_light_position);
    }
    else if(key == 'z') {
        red_light_position[2] -= 1.0;
        glLightfv(GL_LIGHT0, GL_POSITION, red_light_position);
    }
    else if(key == 'Z') {
        red_light_position[3] += 1.0;
        glLightfv(GL_LIGHT0, GL_POSITION, red_light_position);
    }
    else if(key == 'a') {
        green_light_position[0] -= 1.0;
        glLightfv(GL_LIGHT1, GL_POSITION, green_light_position);
    }
    else if(key == 'b') {
        green_light_position[1] -= 1.0;
        glLightfv(GL_LIGHT1, GL_POSITION, green_light_position);

    }
    else if(key == 'c') {
        green_light_position[2] -= 1.0;
        glLightfv(GL_LIGHT1, GL_POSITION, green_light_position);
    }
    else if(key == 'A') {
        green_light_position[0] += 1.0;
        glLightfv(GL_LIGHT1, GL_POSITION, green_light_position);
    }
    else if(key == 'B') {
        green_light_position[1] += 1.0;
        glLightfv(GL_LIGHT1, GL_POSITION, green_light_position);
    }
    else if(key == 'C') {
        green_light_position[2] += 1.0;
        glLightfv(GL_LIGHT1, GL_POSITION, green_light_position);
    }
    display();
}
 
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
    glutCreateWindow("Group 04");
 
    glutReshapeFunc(resize);
    glutDisplayFunc(display); 
    glutKeyboardFunc(keys);
 
    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 

    //Habilitando luz
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING); 
 
    //Roja
    glLightfv(GL_LIGHT0, GL_AMBIENT,  red_light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  red_light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, red_light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, red_light_position); 
 
    //Verde
    glLightfv(GL_LIGHT1, GL_AMBIENT,  green_light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  green_light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, green_light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, green_light_position); 

    //Material
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess); 
 
    glutMainLoop(); 
 
    return 0;
}
