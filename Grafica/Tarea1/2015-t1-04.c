#include <GL/glut.h>
#include <math.h>

void mydisplay(){
    int i=0;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
        glColor3f(1.0f, 0.0f, 0.0f);
        for(; i<6; ++i)
            glVertex2d(sin(i/6.0*2*M_PI), cos(i/6.0*2*M_PI));
    glEnd();
    glFlush();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutCreateWindow("Simple");
    glutDisplayFunc(mydisplay);
    glutMainLoop();
    return 0;
}
