#include "GLUT\freeglut.h"

void render(void) // the function that will run to display items
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // wipe the drawing surface's color and depth buffer
														// GL_DEPTH_BUFFER_BIT: buffer to store how close object's pixels are releative to the viewer
														// GL_COLOR_BUFFER_BIT: buffer to store what color the object's pixels are

	//...

	glBegin(GL_QUADS); // telling opengl what object is going to be described by vertex below
					   // GL_QUADS: creats Quadrilaterals (every 4 vertex turn into one Quadrilateral)
					   // glnormal3f: sets vertex normal in the format of a 3d vector consisting of float values
					   // glColor3f: sets vertex color in the format of a (red, green, blue) consisting of float values (0 to 1)
					   // glVertex3f: sets vertex position in space in the format of a 3d vector consisting of float values

	glNormal3f(-1.0, -1.0, 1.0); glColor3f(1.0, 0.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
	glNormal3f(-1.0, 1.0, 1.0);  glColor3f(1.0, 0.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
	glNormal3f(1.0, 1.0, 1.0);   glColor3f(1.0, 0.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
	glNormal3f(1.0, -1.0, 1.0);  glColor3f(1.0, 0.0, 0.0); glVertex3f(0.5, -0.5, 0.5);

	glNormal3f(-1.0, -1.0, -1.0); glColor3f(0.0, 1.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
	glNormal3f(-1.0, 1.0, -1.0);  glColor3f(0.0, 1.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
	glNormal3f(1.0, 1.0, -1.0);   glColor3f(0.0, 1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);
	glNormal3f(1.0, -1.0, -1.0);  glColor3f(0.0, 1.0, 0.0); glVertex3f(0.5, -0.5, -0.5);

	glNormal3f(1.0, -1.0, -1.0); glColor3f(0.0, 0.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
	glNormal3f(1.0, -1.0, 1.0);  glColor3f(0.0, 0.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
	glNormal3f(1.0, 1.0, 1.0);   glColor3f(0.0, 0.0, 1.0); glVertex3f(0.5, 0.5, 0.5);
	glNormal3f(1.0, 1.0, -1.0);  glColor3f(0.0, 0.0, 1.0); glVertex3f(0.5, 0.5, -0.5);

	glNormal3f(-1.0, -1.0, -1.0); glColor3f(1.0, 0.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glNormal3f(-1.0, -1.0, 1.0);  glColor3f(1.0, 0.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
	glNormal3f(-1.0, 1.0, 1.0);   glColor3f(1.0, 0.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
	glNormal3f(-1.0, 1.0, -1.0);  glColor3f(1.0, 0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);

	glNormal3f(-1.0, 1.0, -1.0); glColor3f(1.0, 1.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
	glNormal3f(-1.0, 1.0, 1.0);  glColor3f(1.0, 1.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
	glNormal3f(1.0, 1.0, 1.0);   glColor3f(1.0, 1.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
	glNormal3f(1.0, 1.0, -1.0);  glColor3f(1.0, 1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);

	glNormal3f(-1.0, -1.0, -1.0); glColor3f(0.0, 1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glNormal3f(-1.0, -1.0, 1.0);  glColor3f(0.0, 1.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
	glNormal3f(1.0, -1.0, 1.0);   glColor3f(0.0, 1.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
	glNormal3f(1.0, -1.0, -1.0);  glColor3f(0.0, 1.0, 1.0); glVertex3f(0.5, -0.5, -0.5);

	glEnd(); // telling opengl this is the end of glBegin

	//...
	glutSwapBuffers(); // swap the back buffer (which has been prepared) with front buffer, telling gpu to write the new frame

}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void keyboard(unsigned char, int, int) {

}

void delay(int) {}
void idle(void) {
	glutPostRedisplay(); // re-calling the display (render in this case) function and remake the buffer

	glRotatef(1, 1, 1, 0); // function to rotate the matrix by 1 degree in x and y axis 

	glutTimerFunc(1000 / 60, delay, 0); // wait 1000/60 ms to (re-)call the function (limit to at most 60 fps)
}



void init() {
	// initilizing camera 
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0, 1.0, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3);
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv); // initilize OpenGL GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // enable double buffering and RGBA color spectrum
	glutInitWindowPosition(100, 100); // set the top left corner point of window that's to be displayed
	glutInitWindowSize(500, 500); // set the size for window that's to be displayed
	glutCreateWindow("app"); // set the name for window

	init(); // custome initilizing

	glutDisplayFunc(render); // set the function to be used to display items
	glutReshapeFunc(reshape); // set the fun. to be called when window size changes
	glutKeyboardFunc(keyboard); // set the fun. to be called when a key is pressed
	glutIdleFunc(idle); // sets the fun. that is called when no events are happening

	glutMainLoop(); // starts glut main loop (which responds to events and etc.)
}
