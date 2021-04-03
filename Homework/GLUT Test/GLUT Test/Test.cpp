#include <GLUT\freeglut.h>
#include <iostream>
#include "math.h"
#include "cstdio"
#include <iostream>

//variables go here
float angle = 0.0;
float moonAroundEarth = 0.0;
float moonitself = 0.0;

GLuint earthTexture;
GLuint planetTexture;
GLuint sunTexture;
GLuint moonTexture;

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

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_DEPTH_TEST);


	moonTexture = LoadTexture("Texture/moon.bmp");
	planetTexture = LoadTexture("Texture/neptune.bmp");
	earthTexture = LoadTexture("Texture/earth.bmp");
	sunTexture = LoadTexture("Texture/sun.bmp");

}


void display(void)
{
	//cleaning up the before coloring or depths
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0, 1.0);
	glEnable(GL_TEXTURE_2D);





	//calling timer for rotation
	void timer();

	//sun
	glPushMatrix();
	//texture

	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glTranslatef(0.0, 0.0, 0.0);
	//dorost krdn mnba nur
	GLUquadric* q = gluNewQuadric();
	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals(q, GLU_SMOOTH);
	gluQuadricTexture(q, GL_TRUE);
	gluQuadricOrientation(q, GLU_INSIDE);
	//rsm kore
	gluSphere(q, 5, 20, 20);
	glPopMatrix();


	q = gluNewQuadric();
	gluQuadricTexture(q, GL_TRUE);

	//earth
	glPushMatrix();
	//texture
	glBindTexture(GL_TEXTURE_2D, earthTexture);
	glRotatef(angle * 30, 0.0, 1.0, 0.0);
	glTranslatef(10.0, 0.0, 0.0);
	gluSphere(q, 3, 20, 20);
	glPopMatrix();


	//moon
	glPushMatrix();
	//texture
	glBindTexture(GL_TEXTURE_2D, moonTexture);
	// rotation for moon.
	glRotatef(angle * 30, 0.0, 1.0, 0.0);
	// translation for moon.
	glTranslatef(15, 0.0, 0.0);
	// around earth rotation.
	glRotatef(moonAroundEarth * 60, 0.0, 1.0, 0.0);
	// moon rotation about it self.
	glRotatef(moonitself, 0.0, 1.0, 0.0);

	gluSphere(q, 0.5, 20, 20);
	glPopMatrix();


	//otherPlanet
	glPushMatrix();
	//texture
	glBindTexture(GL_TEXTURE_2D, planetTexture);
	glRotatef(angle * 10, 0, 1, 0);
	glTranslatef(-25.0, 0.0, 0.0);
	gluSphere(q, 2, 20, 20);
	glPopMatrix();


	glutSwapBuffers();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-30, 30, -25, 25, -25, 25);
	//  your camera (projection settings go here)
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	// your camera model can be placed here
}

void timer(int v) {
	angle += 1;



	//changing the variables by time for rotation of the shape
	if (angle > 360)
		angle = angle - 360;

	if (moonAroundEarth > 360)
		moonAroundEarth = moonAroundEarth - 360;

	if (moonitself > 360)
		moonitself = moonitself - 360;

	// for stack to not overflow
	// write your own code for updating animation variables if required 
	// updata your movement parameters here...
	glutPostRedisplay();
	glutTimerFunc(100, timer, 0);
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
	GLfloat lightPosition[] = { 0, 0.0, 0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	initLighting();
	glutDisplayFunc(display);    glutReshapeFunc(reshape);
	glutTimerFunc(100, timer, 0); // use if you want to create animation
	glutMainLoop();
	//return 0;
}
