#include "GLUT\freeglut.h"
#include "math.h"
#include "cstdio"

int xrr = 0, yrr = 0;
bool autoAnim = true;
GLuint earthTextureData;

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, outWidth, outHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	return tex;
}

void pyramids() {
	glBegin(GL_QUADS);
	glTexCoord3f(0.0, 0.0, 0.0);
	glVertex3f(-0.5, -0.5, 0);

	glTexCoord3f(0.0, 1.0, 0.0);
	glVertex3f(-0.5, 0.5, 0);

	glTexCoord3f(1.0, 1.0, 0.0);
	glVertex3f(0.5, 0.5, 0);

	glTexCoord3f(1.0, 0.0, 0);
	glVertex3f(0.5, -0.5, 0);
	glEnd();
}

void cube() {
	glBegin(GL_QUADS); // telling opengl what object is going to be described by vertex below
					   // GL_QUADS: creats Quadrilaterals (every 4 vertex turn into one Quadrilateral)

	glNormal3f(-1.0, -1.0, 1.6); glColor3f(1.0, 0.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
	glNormal3f(-1.0, 1.0, 1.6);  glColor3f(1.0, 0.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
	glNormal3f(1.0, 1.0, 1.6);   glColor3f(1.0, 0.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
	glNormal3f(1.0, -1.0, 1.6);  glColor3f(1.0, 0.0, 0.0); glVertex3f(0.5, -0.5, 0.5);

	glNormal3f(-1.0, -1.0, -1.6); glColor3f(0.0, 1.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
	glNormal3f(-1.0, 1.0, -1.6);  glColor3f(0.0, 1.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
	glNormal3f(1.0, 1.0, -1.6);   glColor3f(0.0, 1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);
	glNormal3f(1.0, -1.0, -1.6);  glColor3f(0.0, 1.0, 0.0); glVertex3f(0.5, -0.5, -0.5);

	glNormal3f(1.6, -1.0, -1.0); glColor3f(0.0, 0.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
	glNormal3f(1.6, -1.0, 1.0);  glColor3f(0.0, 0.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
	glNormal3f(1.6, 1.0, 1.0);   glColor3f(0.0, 0.0, 1.0); glVertex3f(0.5, 0.5, 0.5);
	glNormal3f(1.6, 1.0, -1.0);  glColor3f(0.0, 0.0, 1.0); glVertex3f(0.5, 0.5, -0.5);

	glNormal3f(-1.6, -1.0, -1.0); glColor3f(1.0, 0.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glNormal3f(-1.6, -1.0, 1.0);  glColor3f(1.0, 0.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
	glNormal3f(-1.6, 1.0, 1.0);   glColor3f(1.0, 0.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
	glNormal3f(-1.6, 1.0, -1.0);  glColor3f(1.0, 0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);

	glNormal3f(-1.0, 1.6, -1.0); glColor3f(1.0, 1.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
	glNormal3f(-1.0, 1.6, 1.0);  glColor3f(1.0, 1.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
	glNormal3f(1.0, 1.6, 1.0);   glColor3f(1.0, 1.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
	glNormal3f(1.0, 1.6, -1.0);  glColor3f(1.0, 1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);

	glNormal3f(-1.0, -1.6, -1.0); glColor3f(0.0, 1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glNormal3f(-1.0, -1.6, 1.0);  glColor3f(0.0, 1.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
	glNormal3f(1.0, -1.6, 1.0);   glColor3f(0.0, 1.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
	glNormal3f(1.0, -1.6, -1.0);  glColor3f(0.0, 1.0, 1.0); glVertex3f(0.5, -0.5, -0.5);

	glEnd(); // telling opengl this is the end of glBegin
}

void sphere() {
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(1, 50, 50);
}

void dodecahedron() {
	glColor3f(1.0, 0.0, 0.0);
	glutSolidDodecahedron();
}

void transform(void(*f)(), double x, double y, double z, double ratio) {


	glTranslatef(x, y, z); // moves shapes by x horizontaly and by y verticaly
	glScalef(ratio, ratio, ratio); // scales shapes by a horizontaly and by b verticaly

	glRotatef(xrr, 1, 0, 0); // function to rotate the matrix by xrr degree in x axis 
	glRotatef(yrr, 0, 1, 0);

	(*f)();

	glRotatef(-yrr, 0, 1, 0);
	glRotatef(-xrr, 1, 0, 0);

	glScalef(1.0 / ratio, 1.0 / ratio, 1.0 / ratio);
	glTranslatef(-x, -y, -z);
}

void materialColorSet(GLfloat r, GLfloat g, GLfloat b) {

	GLfloat a[] = { r, g, b, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a);
	GLfloat d[] = { 0.2f, 0.3f, 0.5f, 0.5f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, d);
	GLfloat s[] = { r, g, b, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s);
}

void render(void) // the function that will run to display items
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // wipe the drawing surface's color and depth buffer
														// GL_DEPTH_BUFFER_BIT: buffer to store how close object's pixels are releative to the viewer
														// GL_COLOR_BUFFER_BIT: buffer to store what color the object's pixels are

	//...
	printf("Reading image %s\n", "Texture/globe.bmp");
	unsigned int outWidth = -1;
	unsigned int outHeight = -1;
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;

	FILE* file;
	if (errno_t err = fopen_s(&file, "Texture/globe.bmp", "rb") != 0) {
		perror("Can't Open file");
	}

	// If less than 54 byes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n"); }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n"); }

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
	fread(data, 1, imageSize, file);


	// Everything is in memory now, the file wan be closed
	fclose(file);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, outWidth, outHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)data);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBindTexture(GL_TEXTURE_2D, tex);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, -0.5, 0);

	glTexCoord2f(0.0, 1);
	glVertex3f(-0.5, 0.5, 0);

	glTexCoord2f(1, 1);
	glVertex3f(0.5, 0.5, 0);

	glTexCoord2f(1, 0.0);
	glVertex3f(0.5, -0.5, 0);

	glEnd();

	//...
	glutSwapBuffers(); // swap the back buffer (which has been prepared) with front buffer, telling gpu to write the new frame

}

void reshape(int, int) {

}

void keyboard(unsigned char key, int, int) {
	switch (key) {
	case 32:
		if (autoAnim) { autoAnim = false; }
		else { autoAnim = true; }
		break;
	case 'W':
	case 'w':
		xrr -= 5;
		autoAnim = false;
		break;

	case 'A':
	case 'a':
		yrr -= 5;
		autoAnim = false;
		break;

	case 'S':
	case 's':
		xrr += 5;
		autoAnim = false;
		break;

	case 'D':
	case 'd':
		yrr += 5;
		autoAnim = false;
		break;
	}
	xrr %= 360, yrr %= 360;
}

void mouseMove(int x, int y) {
	double width = glutGet(GLUT_WINDOW_WIDTH);
	double height = glutGet(GLUT_WINDOW_HEIGHT);
	float fx = (x - width / 2)*2.0 / width, fy = (-y + height / 2)*2.0 / height;
	GLfloat lightPosition[] = { fx * 2, fy * 2, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

}

void delay(int) {}

void idle(void) {
	if (autoAnim) {
		xrr += 1, yrr += 1;
	}
	glutPostRedisplay(); // re-calling the display (render in this case) function and remake the buffer
	glutTimerFunc(1000 / 60, delay, 0); // wait 1000/60 ms to (re-)call the function (limit to at most 60 fps)
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

	GLuint earthTextureData = LoadTexture("Texture/globe.bmp");
}

void initLighting()
{
	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Set lighting intensity and color
	GLfloat ambientLight[] = { 0.3, 0.2, 0.1, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	GLfloat diffuseLight[] = { 0.5, 0.5, 0.5, 0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	GLfloat specularLight[] = { 0.3, 0.2, 0.1, 1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glMaterialf(GL_FRONT, GL_SHININESS, 1);

	// Set the light position
	GLfloat lightPosition[] = { 0.0, 0.0, 1.0, 0.5 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv); // initilize OpenGL GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // enable double buffering and RGBA color spectrum
	glutInitWindowPosition(100, 100); // set the top left corner point of window that's to be displayed
	glutInitWindowSize(1200, 800); // set the size for window that's to be displayed
	glutCreateWindow("app"); // set the name for window

	init(); // custome initilizing
	initLighting();

	glutDisplayFunc(render); // set the function to be used to display items
	glutReshapeFunc(reshape); // set the fun. to be called when window size changes
	glutMotionFunc(mouseMove); // set the fun. to be called when mouse is clicked
	glutKeyboardFunc(keyboard); // set the fun. to be called when a key is pressed
	glutIdleFunc(idle); // sets the fun. that is called when no events are happening

	glutMainLoop(); // starts glut main loop (which responds to events and etc.)
}
