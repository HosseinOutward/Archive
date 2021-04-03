#include "GLUT\freeglut.h"
#include "math.h"
#include "cstdio"

int xrr = 0;
double rotOneDeg = 0;
bool autoAnim = true;
GLuint earthTextureData, planetTextureData, sunTextureData, moonTextureData;

GLuint LoadTexture(const char* imagepath)
{
	printf("Reading image %s\n", imagepath);
	unsigned int outWidth = -1;
	unsigned int outHeight = -1;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;

	FILE* file;
	if (errno_t err = fopen_s(&file, imagepath, "rb") != 0) {
		perror("Can't Open file");
		return 0;
	}

	// If less than 54 byes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return NULL; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return NULL; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);
	imageSize = *(int*)&(header[0x22]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = outWidth * outHeight * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Read the actual data from the file into the buffer
	unsigned char* data = new unsigned char[imageSize];
	fread(data, 3, imageSize, file);
	//unsigned* intdata = new unsigned[imageSize];
	//for (int i = 0; i < imageSize - 2; i++)
		//intdata[i] = data[i];


	// Everything is in memory now, the file wan be closed
	fclose(file);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, outWidth, outHeight, 0, GL_RGB, GL_UNSIGNED_INT, (GLvoid*)intdata);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, outWidth, outHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)data);

	return tex;
}

void sphere() {
	glutSolidSphere(1, 50, 50);
}

void sunSphere() {
	GLUquadric* q = gluNewQuadric();
	gluQuadricOrientation(q, GLU_INSIDE);
	gluSphere(q, 1, 50, 50);
}

void materialColorSet(GLfloat r, GLfloat g, GLfloat b) {
	glColor3f(r, g, b);
}

void renderObjForLink(int i) {
	switch (i) {
	case 3: materialColorSet(0.7, 0, 0);   glRectf(0, 0, 20, 40); break;
	case 2: materialColorSet(0, 0, 0.7);   glRectf(0, 0, 30, 10); break;
	case 1: materialColorSet(0, 0.7, 0);   glRectf(0, 0, 20, 10); break;
	case 0: materialColorSet(0.7, 0.7, 0); glRectf(0, 0, 10, 5); break;
	}
}

void linkObj(double m[][3], double l[][3], double o[][3], int i) {
	if (i != -1) {
		glRotatef(o[i][0], 0, 1, 0); // function to rotate the matrix by rotX degree in x axis 
		glRotatef(o[i][1], 1, 0, 0);
		glRotatef(o[i][2], 0, 0, 1);

		glTranslatef(m[i][0], m[i][1], m[i][2]);

		glRotatef(l[i][0], 0, 1, 0);
		glRotatef(l[i][1], 1, 0, 0);
		glRotatef(l[i][2], 0, 0, 1);

		renderObjForLink(i);
		linkObj(m, l, o, i - 1);

		glRotatef(-l[i][2], 0, 0, 1);
		glRotatef(-l[i][1], 1, 0, 0);
		glRotatef(-l[i][0], 0, 1, 0);

		glTranslatef(-m[i][0], -m[i][1], -m[i][2]);

		glRotatef(-o[i][2], 0, 0, 1);
		glRotatef(-o[i][1], 1, 0, 0);
		glRotatef(-o[i][0], 0, 1, 0);
	}
}

void render(void) // the function that will run to display items
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // wipe the drawing surface's color and depth buffer
														// GL_DEPTH_BUFFER_BIT: buffer to store how close object's pixels are releative to the viewer
														// GL_COLOR_BUFFER_BIT: buffer to store what color the object's pixels are
	glRotatef(xrr, 1, 0, 0);
	xrr = 0;
	double move[4][3]{
		{20,2.5,0},
		{30,0,0},
		{20,15,0},
		{0,0,0}
	};
	double objeRot[4][3]{
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0}
	};
	double localRot[4][3]{
		{0,0,rotOneDeg },
		{0,0,-rotOneDeg},
		{0,0,rotOneDeg },
		{0,0,rotOneDeg}
	};
	double ratio[4]{
		1,1,1,1
	};

	glScalef(0.1, 0.1, 0.1); // scales shapes by a horizontaly and by b verticaly
	linkObj(move, localRot, objeRot, 3);
	glScalef(10, 10, 10);
	//...

	//...
	glutSwapBuffers(); // swap the back buffer (which has been prepared) with front buffer, telling gpu to write the new frame

}

void reshape(int, int) {

}

void keyboard(unsigned char key, int, int) {
	switch (key) {
	case 32:
		autoAnim = !autoAnim;
		break;

	case 'W':
	case 'w':
		xrr -= 5;
		break;
	case 'S':
	case 's':
		xrr += 5;
		break;
	}
}

void mouseMove(int x, int y) {

}

void delay(int) {}
void idle(void) {
	if (autoAnim) {
		rotOneDeg += 2.0;
		if (rotOneDeg > 50000)
			rotOneDeg -= 50000;
	}
	glutPostRedisplay(); // re-calling the display (render in this case) function and remake the buffer
	glutTimerFunc(100.0 / 6.0, delay, 0); // wait 1000/60 ms to (re-)call the function (limit to at most 60 fps)
}

void init() {
	// changing aspect ratio and settinf matrix mode
	double width = glutGet(GLUT_WINDOW_WIDTH);
	double height = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	float aspect = (float)width / (float)height;

	// initilizing camera 
	gluPerspective(60.0, aspect, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	GLuint earthTextureData = LoadTexture("Texture/globe.bmp");

	double ratio = 0.15;
	glScalef(ratio, ratio, ratio);
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
	glutMotionFunc(mouseMove); // set the fun. to be called when mouse is clicked
	glutKeyboardFunc(keyboard); // set the fun. to be called when a key is pressed
	glutIdleFunc(idle); // sets the fun. that is called when no events are happening

	glutMainLoop(); // starts glut main loop (which responds to events and etc.)
}
