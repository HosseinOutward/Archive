#include "GLUT\freeglut.h"
#include "math.h"
#include "cstdio"

int xrr = 0;
double rotOneDeg = 0;
bool autoAnim = true;
GLuint earthTextureData, planetTextureData, sunTextureData, moonTextureData, starTextureData;

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
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose(file);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, outWidth, outHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, (GLvoid*)data);

	return tex;
}

void sphere() {
	GLUquadric* q = gluNewQuadric();
	gluQuadricTexture(q, true);
	gluSphere(q, 1, 50, 50);
}

void sunSphere() {
	GLUquadric* q = gluNewQuadric();
	gluQuadricTexture(q, true);
	gluQuadricOrientation(q, GLU_INSIDE);
	gluSphere(q, 1, 50, 50);
}

void background(void) {
	glBindTexture(GL_TEXTURE_2D, starTextureData);
	GLUquadric* q = gluNewQuadric();
	gluQuadricOrientation(q, GLU_INSIDE);
	gluQuadricTexture(q, true);
	gluSphere(q, 30, 50, 50);
}

void transform(void(*f)(), double move[], double localRot[], double objeRot[2][3], double ratio) {
	glTranslatef(objeRot[0][0], objeRot[0][1], objeRot[0][2]); // moves shapes by x horizontaly and by y verticaly
	glRotatef(objeRot[1][0], 0, 1, 0); // function to rotate the matrix by rotX degree in x axis 
	glRotatef(objeRot[1][1], 1, 0, 0);
	glRotatef(objeRot[1][2], 0, 0, 1);
	glTranslatef(-objeRot[0][0], -objeRot[0][1], -objeRot[0][2]);

	glTranslatef(move[0], move[1], move[2]);

	glScalef(ratio, ratio, ratio); // scales shapes by a horizontaly and by b verticaly

	glRotatef(localRot[0], 0, 1, 0);
	glRotatef(localRot[1], 1, 0, 0);
	glRotatef(localRot[2], 0, 0, 1);

	(*f)();

	glRotatef(-localRot[2], 0, 0, 1);
	glRotatef(-localRot[1], 1, 0, 0);
	glRotatef(-localRot[0], 0, 1, 0);

	glScalef(1.0 / ratio, 1.0 / ratio, 1.0 / ratio);

	glTranslatef(-move[0], -move[1], -move[2]);

	glTranslatef(objeRot[0][0], objeRot[0][1], objeRot[0][2]);
	glRotatef(-objeRot[1][2], 0, 0, 1);
	glRotatef(-objeRot[1][1], 1, 0, 0);
	glRotatef(-objeRot[1][0], 0, 1, 0);
	glTranslatef(-objeRot[0][0], -objeRot[0][1], -objeRot[0][2]);
}

void materialColorSet(GLfloat r, GLfloat g, GLfloat b) {

	GLfloat a[] = { r, g, b, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, a);
	GLfloat d[] = { 0.2f, 0.3f, 0.5f, 0.5f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, d);
	GLfloat s[] = { r, g, b, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, s);
}

void linkEarthAndMoon() {
	glRotatef(-90, 1, 0, 0);
	sphere();
	glRotatef(90, 1, 0, 0);
	double move[3]; double localRot[3]; double objeRot[2][3];
	glBindTexture(GL_TEXTURE_2D, moonTextureData);
	move[0] = 3; move[1] = 0; move[2] = 0;
	localRot[0] = rotOneDeg / 3; localRot[1] = 0; localRot[2] = 0;
	objeRot[0][0] = 0; objeRot[0][1] = 0; objeRot[0][2] = 0;
	objeRot[1][0] = rotOneDeg / 5; objeRot[1][1] = 0; objeRot[1][2] = 0;
	transform(*sphere, move, localRot, objeRot, 0.5);
}

void render(void) // the function that will run to display items
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // wipe the drawing surface's color and depth buffer
														// GL_DEPTH_BUFFER_BIT: buffer to store how close object's pixels are releative to the viewer
														// GL_COLOR_BUFFER_BIT: buffer to store what color the object's pixels are
	background();
	double move[3]; double localRot[3]; double objeRot[2][3];

	//...
	//sun
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sunTextureData);
	move[0] = 0; move[1] = 0; move[2] = 0;
	localRot[0] = rotOneDeg / 4; localRot[1] = 0; localRot[2] = 0;
	objeRot[0][0] = 0; objeRot[0][1] = 0; objeRot[0][2] = 0;
	objeRot[1][0] = 0; objeRot[1][1] = 0; objeRot[1][2] = 0;
	transform(*sunSphere, move, localRot, objeRot, 2.5);

	//earth and moon
	glBindTexture(GL_TEXTURE_2D, earthTextureData);
	move[0] = 10; move[1] = 0; move[2] = 0;
	localRot[0] = rotOneDeg / 2; localRot[1] = 0; localRot[2] = 0;
	objeRot[0][0] = 0; objeRot[0][1] = 0; objeRot[0][2] = 0;
	objeRot[1][0] = rotOneDeg / 4; objeRot[1][1] = 0; objeRot[1][2] = 0;
	transform(*linkEarthAndMoon, move, localRot, objeRot, 1);

	//planet
	glBindTexture(GL_TEXTURE_2D, planetTextureData);
	move[0] = 5; move[1] = 0; move[2] = 0;
	localRot[0] = rotOneDeg; localRot[1] = 0; localRot[2] = 0;
	objeRot[0][0] = 0; objeRot[0][1] = 0; objeRot[0][2] = 0;
	objeRot[1][0] = rotOneDeg / 3; objeRot[1][1] = 0; objeRot[1][2] = 0;
	transform(*sphere, move, localRot, objeRot, 1);
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
		glRotatef(-5, 1, 0, 0);
		break;
	case 'S':
	case 's':
		glRotatef(5, 1, 0, 0);
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

	earthTextureData = LoadTexture("Texture/globe.bmp");
	sunTextureData = LoadTexture("Texture/sun.bmp");
	moonTextureData = LoadTexture("Texture/moon.bmp");
	planetTextureData = LoadTexture("Texture/kerbal.bmp");
	starTextureData = LoadTexture("Texture/stars.bmp");

	double ratio = 0.15;
	glScalef(ratio, ratio, ratio);
	glRotatef(20, 1, 0, 0);
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
	GLfloat lightPosition[] = { 0.0, 0.0, 0, 1 };
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
