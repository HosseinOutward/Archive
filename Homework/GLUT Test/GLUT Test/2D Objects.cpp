#include "GLUT\freeglut.h"
#include "math.h"

#define PI 3.14159265
double r = 0;
bool smooth = true;

/************************** Functions for ease of access ****************************/
void triangle() {
	glBegin(GL_TRIANGLES); // telling opengl what object is going to be described by vertex below
					   // GL_TRIANGLES: every 3 vertex into one triangle
					   // glColor3f: sets vertex color in the format of a (red, green, blue) consisting of float values (0 to 1)
					   // glVertex2f: sets vertex position in space in the format of a 2d vector consisting of float values

	glColor3f(0, 0.0, 0.0); glVertex2f(-1, -1);
	glColor3f(0, 1.0, 0.0); glVertex2f(-1, 1);
	glColor3f(0.5, 0.0, 0.0); glVertex2f(1, 1);

	glEnd(); // telling opengl this is the end of glBegin
}

void star() {
	glBegin(GL_POLYGON); // GL_POLYGON: all vertexs become 1 shape (like a star)

	for (double i = r; i < 360 + r; i += 2 * PI / 5) {
		double x = cos(i) / 2, y = sin(i) / 2;
		glColor3f(fabs(y), fabs(x), 0);
		glVertex2f(x, y);

		x = cos(i + PI / 5), y = sin(i + PI / 5);
		glVertex2f(x, y);
		glColor3f(fabs(y), fabs(x), 0);
	}

	glEnd();
}

void circle() {
	glBegin(GL_POLYGON); // GL_POLYGON: all vertexs become 1 shape (like an unperfect circle)


	for (double i = r; i < 2 * PI + r; i += 2 * PI / 100) {
		double x = cos(i), y = sin(i);
		glColor3f(fabs(x), fabs(y), fabs(sin(r) + x) / 2);
		glVertex2f(x, y);
	}

	glEnd();
}

void heart() {
	double y = 0.4, a = 0.55, b = 0.55;
	glTranslatef(0, y, 0); // explainded later
	glScalef(a, b, 0); // explainded later

	glBegin(GL_POLYGON); // GL_POLYGON: all vertexs become 1 shape (like an unperfect circle)

	for (double i = -PI * 0.17; i < PI; i += PI / 100) {
		double x = cos(i), y = sin(i);
		glVertex2f(x + 0.7, y);
	}
	for (double i = 0; i < PI + PI * 0.17; i += PI / 100) {
		double x = cos(i), y = sin(i);
		glVertex2f(x - 0.7, y);
	}

	glColor3f(cos(r), 0, 1);
	glVertex2f(0, -2.5);

	glEnd();

	glScalef(1.0 / a, 1.0 / b, 0);
	glTranslatef(-0, -y, 0);
}

void rect() {
	glBegin(GL_QUADS); // GL_QUADS: creats Quadrilaterals (every 4 vertex turn into one Quadrilateral)

	float t = sin(r);
	glColor3f(1.0 + t, 0.0, 0.0); glVertex3f(-1, -1, 1);
	glColor3f(1.0 + t, 1.0, 0.0); glVertex3f(-1, 1, 1);
	glColor3f(1.0 + t, 0.0, 1.0); glVertex3f(1, 1, 1);
	glColor3f(0.0 + t, 0.0, 1.0); glVertex3f(1, -1, 1);

	glEnd(); // telling opengl this is the end of glBegin
}

void transform(void(*f)(), double x, double y, double a, double b) {

	glTranslatef(x, y, 0); // moves shapes by x horizontaly and by y verticaly
	glScalef(a, b, 0); // scales shapes by a horizontaly and by b verticaly

	(*f)();

	glScalef(1.0 / a, 1.0 / b, 0);
	glTranslatef(-x, -y, 0);
}
/************************** END ****************************/

void render(void) // the function that will run to display items
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // wipe the drawing surface's color and depth buffer
														// GL_DEPTH_BUFFER_BIT: buffer to store how close object's pixels are releative to the viewer
														// GL_COLOR_BUFFER_BIT: buffer to store what color the object's pixels are

	//...
	float ratio = 0.45;
	transform(*heart, -1, -0.5, ratio, ratio);
	transform(*triangle, 1, 0.5, ratio, ratio);
	transform(*circle, 0, 0.5, ratio, ratio);
	transform(*star, 1, -0.5, ratio, ratio);
	transform(*rect, -1, 0.5, ratio, ratio);


	//...
	glutSwapBuffers(); // swap the back buffer (which has been prepared) with front buffer, telling gpu to write the new frame

}

void reshape(int, int) {

}
void keyboard(unsigned char key, int, int) {
	switch (key) {
	case 32:
		if (smooth) {
			glShadeModel(GL_FLAT);  // sets shading mod (coloring for object) to GL_FLAT (instead of GL_SMOOTH)
						// GL_FLAT: the color for the last vertex is set for the whole object (no interplation)
			smooth = false;
		}
		else {
			glShadeModel(GL_SMOOTH);
			// GL_SMOOTH: the default way, object is interpolated
			smooth = true;
		}
	}
}

void delay(int) {}

void idle(void) {
	glutPostRedisplay(); // re-calling the display (render in this case) function and remake the buffer

	r += 0.01;
	r = remainder(r, 2 * PI);

	glutTimerFunc(1000 / 60, delay, 0); // wait 1000/60 ms to (re-)call the function (limit to at most 60 fps)
}


void init() {
	// changing aspect ratio and settinf matrix mode
	double width = glutGet(GLUT_WINDOW_WIDTH);
	double height = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	float aspect = (float)width / (float)height;
	glOrtho(-aspect, aspect, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv); // initilize OpenGL GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // enable double buffering and RGBA color spectrum
	glutInitWindowPosition(100, 100); // set the top left corner point of window that's to be displayed
	glutInitWindowSize(1200, 800); // set the size for window that's to be displayed
	glutCreateWindow("app"); // set the name for window

	init(); // custome initilizing

	glutDisplayFunc(render); // set the function to be used to display items
	glutReshapeFunc(reshape); // set the fun. to be called when window size changes
	glutKeyboardFunc(keyboard); // set the fun. to be called when a key is pressed
	glutIdleFunc(idle); // sets the fun. that is called when no events are happening

	glutMainLoop(); // starts glut main loop (which responds to events and etc.)
}
