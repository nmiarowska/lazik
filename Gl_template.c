#define  _CRT_SECURE_NO_WARNINGS
#ifdef _MSC_VER                         // Check if MS Visual C compiler
#  pragma comment(lib, "opengl32.lib")  // Compiler-specific directive to avoid manually configuration
#  pragma comment(lib, "glu32.lib")     // Link libraries
#endif
#ifdef _MSC_VER        // Check if MS Visual C compiler
#   ifndef _MBCS
#      define _MBCS    // Uses Multi-byte character set
#   endif
#   ifdef _UNICODE     // Not using Unicode character set
#      undef _UNICODE 
#   endif
#   ifdef UNICODE
#      undef UNICODE 
#   endif
#endif
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include "resource.h"           // About box resource identifiers.
#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14
int a = 0;
int b = 0;
HPALETTE hPalette = NULL;
static LPCTSTR lpszAppName = "GL Template";
static HINSTANCE hInstance;
static GLfloat xRot = 45.0f;
static GLfloat yRot = 90.0f;
static GLsizei lastHeight;
static GLsizei lastWidth;
BITMAPINFOHEADER	bitmapInfoHeader;	// nag³ówek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury
LRESULT CALLBACK WndProc(   HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam);
BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam);
void SetDCPixelFormat(HDC hDC);
int degree = 0;
void ReduceToUnit(float vector[3])
	{
	float length;
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));
	if(length == 0.0f)
		length = 1.0f;
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
	}
void calcNormal(float v[3][3], float out[3])
	{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];
	ReduceToUnit(out);
	}
void ChangeSize(GLsizei w, GLsizei h)
	{
	GLfloat nRange = 750.0f;
	GLfloat fAspect;
	if(h == 0)
		h = 1;
	lastWidth = w;
	lastHeight = h;
	fAspect=(GLfloat)w/(GLfloat)h;
    glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h) 
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else 
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	// Establish perspective: 

	//gluPerspective(90.0f,fAspect,1.0,1);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	}

void SetupRC()
	{

	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
	glColor3f(0.0,0.0,0.0);
	}

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// wskaŸnik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag³ówek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;		// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany sk³adowych

	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	fclose(filePtr);
	return bitmapImage;
}

void walec_0(void)
{
	float x, y;
	float radius = 25.0; //srednica to 25 jednostek
	float alpha;
	float PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f); //czarny srodek podstawy
	glVertex3f(100.0f, 0.0f, 10.0f); //wspolrzedne srodka pierwszej podstawy
	glColor3f(0.5f, 0.5f, 0.5f); //jasnoszary kolor
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 5.0) //kola lazika maja w podstawie dziesieciokat foremny
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(100+x, y, 10.0f);
	}
	glVertex3f(100+radius * cos(0), radius * sin(0), 10.0f);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);//czarny srodek podstawy
	glVertex3f(100.0f, 0.0f, -10.0f);//wspolrzedne srodka drugiej podstawy
	glColor3f(0.5f, 0.5f, 0.5f); //jasnoszary zewnetrzny kolor
	for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(100+x, y, -10.0f);
	}
	glVertex3f(100+radius * cos(2 * PI), radius * sin(2 * PI), -10.0f);
	glEnd();

	//wyrysowanie trojkatow wypelniajacych przestrzen miedzy dwoma podstawami walcow
	for (int i = -10; i <= 0; i += 10) {
		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0) {
			x = radius * cos(alpha);
			y = radius * sin(alpha);
			glVertex3f(100+x, y, i);
			glVertex3f(100+x, y, i + 10);
		}
		glVertex3f(100 + radius * cos(2 * PI), radius * sin(2 * PI), i);
		glVertex3f(100+radius * cos(2 * PI), radius * sin(2 * PI), i + 10);
		glEnd();
	}
}

void walec_1(void)
{
	float x, y;
	float radius = 25.0;
	float alpha;
	float PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 110.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(100+x, y, 110.0f);
	}
	glVertex3f(100+radius * cos(0), radius * sin(0), 110.0f);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 90.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(100+x, y, 90.0f);
	}
	glVertex3f(100+radius * cos(2 * PI), radius * sin(2 * PI), 90.0f);
	glEnd();

	for (int i = 100; i >= 90; i -= 10) {
		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0) {
			x = radius * cos(alpha);
			y = radius * sin(alpha);
			glVertex3f(100+x, y, i);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(100+x, y, i + 10);
		}
		glVertex3f(100+radius * cos(2 * PI), radius * sin(2 * PI), i);
		glVertex3f(100+radius * cos(2 * PI), radius * sin(2 * PI), i + 10);
		glEnd();
	}
}

void walec_3(void)
{
	float x, y;
	float radius = 25.0;
	float alpha;
	float PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 10.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 5.0) 
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(x, y, 10.0f);
	}
	glVertex3f(radius * cos(0), radius * sin(0), 10.0f);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f); 
	glVertex3f(0.0f, 0.0f, -10.0f);
	glColor3f(0.5f, 0.5f, 0.5f); 
	for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(x, y, -10.0f);
	}
	glVertex3f(radius * cos(2 * PI), radius * sin(2 * PI), -10.0f);
	glEnd();

	for (int i=-10; i <= 0; i += 10) {
		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0) {
			x = radius * cos(alpha);
			y = radius * sin(alpha);
			glVertex3f(x, y, i);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(x, y, i+10);
		}
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(radius * cos(2 * PI), radius * sin(2 * PI), i);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(radius * cos(2 * PI), radius * sin(2 * PI), i+10);
		glEnd();
	}
}

void walec_2(void)
{
	float x, y;
	float radius = 25.0;
	float alpha;
	float PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 110.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(x, y, 110.0f);
	}
	glVertex3f(radius * cos(0), radius * sin(0), 110.0f);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 90.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(x, y, 90.0f);
	}
	glVertex3f(radius * cos(2 * PI), radius * sin(2 * PI), 90.0f);
	glEnd();

	for (int i = 100; i >= 90; i -= 10) {
		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0) {
			x = radius * cos(alpha);
			y = radius * sin(alpha);
			glVertex3f(x, y, i);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(x, y, i + 10);
		}
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(radius * cos(2 * PI), radius * sin(2 * PI), i);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(radius * cos(2 * PI), radius * sin(2 * PI), i + 10);
		glEnd();
	}
}

void walec_4(void)
{
	float x, y;
	float radius = 25.0;
	float alpha;
	float PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 10.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(-100+x, y, 10.0f);
	}
	glVertex3f(-100+radius * cos(0), radius * sin(0), 10.0f);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, -10.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(-100+x, y, -10.0f);
	}
	glVertex3f(-100+radius * cos(2 * PI), radius * sin(2 * PI), -10.0f);
	glEnd();

	for (int i = -10; i <= 0; i += 10) {
		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0) {
			x = radius * cos(alpha);
			y = radius * sin(alpha);
			glVertex3f(-100+x, y, i);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(-100+x, y, i + 10);
		}
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-100+radius * cos(2 * PI), radius * sin(2 * PI), i);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-100+radius * cos(2 * PI), radius * sin(2 * PI), i + 10);
		glEnd();
	}
}

void walec_5(void)
{
	float x, y;
	float radius = 25.0;
	float alpha;
	float PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 110.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(-100+x, y, 110.0f);
	}
	glVertex3f(-100+radius * cos(0), radius * sin(0), 110.0f);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 90.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0)
	{
		x = radius * cos(alpha);
		y = radius * sin(alpha);
		glVertex3f(-100+x, y, 90.0f);
	}
	glVertex3f(-100+radius * cos(2 * PI), radius * sin(2 * PI), 90.0f);
	glEnd();

	for (int i = 100; i >= 90; i -= 10) {
		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 5.0) {
			x = radius * cos(alpha);
			y = radius * sin(alpha);
			glVertex3f(-100+x, y, i);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(-100+x, y, i + 10);
		}
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-100+radius * cos(2 * PI), radius * sin(2 * PI), i);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-100+radius * cos(2 * PI), radius * sin(2 * PI), i + 10);
		glEnd();
	}
}

void cialo_cale(void)
{
	for (float param = -100; param <= 100; param += 100)
	{
		float x, y;
		float radius = 70.0; //dlugosc ramion trojkata
		float alpha;
		float PI = 3.14;
		glBegin(GL_TRIANGLE_FAN); //pierwsza podstawa
		glColor3f(0.3f, 0.3f, 0.3f); //ciemnoszary kolor
		glVertex3f(param, 30.0f, 50.0f); //wspolrzedne podstawy
		for (alpha = 0.25 * PI; alpha <= 2.25 * PI; alpha += PI / 2.0) //dzielenie przez 2 jest spowodowane checia uzyskania
			//podstawy figury jako czworokata
		{
			x = radius * sin(alpha);
			y = radius * cos(alpha);
			glVertex3f(param + x, 30.0f, y + 50);
		}
		glVertex3f(param + radius * sin(0.25 * PI), 30.0f, radius * cos(0.25 * PI) + 50);
		glEnd();

		glBegin(GL_TRIANGLE_FAN); //druga podstawa
		glColor3f(0.3f, 0.3f, 0.3f);//ciemnoszary kolor
		glVertex3f(param, 10.0f, 50.0f);//wspolrzedne podstawy
		for (alpha = 2.25 * PI; alpha >= 0.25; alpha -= PI / 2.0)//dzielenie przez 2 jest spowodowane checia uzyskania
			//podstawy figury jako czworokata
		{
			x = radius * sin(alpha);
			y = radius * cos(alpha);
			glVertex3f(param + x, 10.0f, y + 50);
		}
		glVertex3f(param + radius * sin(0.25 * PI), 10.0f, radius * cos(0.25 * PI) + 50);
		glEnd();

		//"domkniecie" przestrzeni miedzy podstawami poprzez wyrysowanie trojkatow
		for (int i = 20; i >= 10; i -= 10) {
			glBegin(GL_TRIANGLE_STRIP);
			for (alpha = 2.25 * PI; alpha >= 0.25; alpha -= PI / 2.0) {
				x = radius * sin(alpha);
				y = radius * cos(alpha);
				glVertex3f(param + x, i, y + 50);
				glVertex3f(param + x, i + 10, y + 50);
			}
			glEnd();
		}
	}
}

void ramie(void)
{
	float x, y;
	float radius = 7.0;
	float alpha;
	float PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(105.0f, 200.0f, 50.0f);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 20.0)
	{
		x = radius * sin(alpha);
		y = radius * cos(alpha);
		glVertex3f(105 + x, 200.0f, y + 50);
	}
	glVertex3f(115 + radius * sin(0), 200.0f, 50 + radius * cos(0));
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(105.0f, 20.0f, 50.0f);
	for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 20.0)
	{
		x = radius * sin(alpha);
		y = radius * cos(alpha);
		glVertex3f(105 + x, 20.0f, y + 50);
	}
	glVertex3f(105 + radius * sin(2 * PI), 20.0f, radius * cos(2 * PI) + 50);
	glEnd();

	for (int i = 200; i >= 20; i -= 10) {
		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 20.0) {
			x = radius * sin(alpha);
			y = radius * cos(alpha);
			glVertex3f(105 + x, i, y + 50);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(105 + x, i + 10, y + 50);
		}
		glVertex3f(105 + radius * sin(2 * PI), i, radius * cos(2 * PI) + 50);
		glVertex3f(105 + radius * sin(2 * PI), i + 10, radius * cos(2 * PI) + 50);
		glEnd();
	}
}

void ramie2(void)
{
	float x, y;
	float radius = 5.0;
	float alpha;
	float PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(122.0f, 200.0f, 50.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 20.0)
	{
		x = radius * sin(alpha);
		y = radius * cos(alpha);
		glVertex3f(122 + x, 200.0f, y + 50);
	}
	glVertex3f(122 + radius * sin(0), 200.0f, 50 + radius * cos(0));
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(122.0f, 130.0f, 50.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 20.0)
	{
		x = radius * sin(alpha);
		y = radius * cos(alpha);
		glVertex3f(122 + x, 130.0f, y + 50);
	}
	glVertex3f(122 + radius * sin(2 * PI), 130.0f, radius * cos(2 * PI) + 50);
	glEnd();

	for (int i = 200; i >= 130; i -= 10) {
		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 20.0) {
			x = radius * sin(alpha);
			y = radius * cos(alpha);
			glVertex3f(122 + x, i, y + 50);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(122 + x, i + 10, y + 50);
		}
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(122 + radius * sin(2 * PI), i, radius * cos(2 * PI) + 50);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(122 + radius * sin(2 * PI), i + 10, radius * cos(2 * PI) + 50);
		glEnd();
	}
}

void ramie3(void)
{
	float x, y;
	float radius = 9.0;
	float alpha;
	float PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(115.0f, 200.0f, 50.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 0; alpha <= 2 * PI; alpha += PI / 20.0)
	{
		x = radius * sin(alpha);
		y = radius * cos(alpha);
		glVertex3f(115 + x, 200.0f, y + 50);
	}
	glVertex3f(115 + radius * sin(0), 200.0f, 50 + radius * cos(0));
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(115.0f, 190.0f, 50.0f);
	glColor3f(0.5f, 0.5f, 0.5f);
	for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 20.0)
	{
		x = radius * sin(alpha);
		y = radius * cos(alpha);
		glVertex3f(115 + x, 190.0f, y + 50);
	}
	glVertex3f(115 + radius * sin(2 * PI), 190.0f, radius * cos(2 * PI) + 50);
	glEnd();

	for (int i = 200; i >= 190; i -= 10) {
		glBegin(GL_TRIANGLE_STRIP);
		for (alpha = 2 * PI; alpha >= 0; alpha -= PI / 20.0) {
			x = radius * sin(alpha);
			y = radius * cos(alpha);
			glVertex3f(115 + x, i, y + 50);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(115 + x, i + 10, y + 50);
		}
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(115 + radius * sin(2 * PI), i, radius * cos(2 * PI) + 50);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(115 + radius * sin(2 * PI), i + 10, radius * cos(2 * PI) + 50);
		glEnd();
	}
}


void podloze(void)
{

	// Parametry wierzcholkow

	GLfloat sa[3] = { -500.0f,-100.0f,700.0f };
	GLfloat sb[3] = { 500.0f,-100.0f,700.0f };
	GLfloat sc[3] = { 500.0f,-25.0f,700.0f };
	GLfloat sd[3] = { -500.0f,-25.0f,700.0f };
	GLfloat se[3] = { -500.0f,-100.0f,-700.0f };
	GLfloat sf[3] = { 500.0f,-100.0f,-700.0f };
	GLfloat sg[3] = { 500.0f,-25.0f,-700.0f };
	GLfloat sh[3] = { -500.0f,-25.0f,-700.0f };


	// Sciany skladowe
	glColor3f(0.7f, 0.3f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sc);
	glVertex3fv(sd);
	glEnd();

	glColor3f(0.7f, 0.3f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(sg);
	glVertex3fv(sc);
	glEnd();

	glColor3f(0.7f, 0.3f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	glColor3f(0.7f, 0.3f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(se);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.3f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.3f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(se);
	glEnd();
	
}

void podloze_obiekt(void)
{

	// Parametry wierzcholkow
	GLfloat sa[3] = { 300.0f,-25.0f,100.0f };
	GLfloat sb[3] = { 400.0f,-25.0f,100.0f };
	GLfloat sc[3] = { 400.0f,75.0f,100.0f };
	GLfloat sd[3] = { 300.0f,75.0f,100.0f };
	GLfloat se[3] = { 300.0f,-25.0f,-100.0f };
	GLfloat sf[3] = { 400.0f,-25.0f,-100.0f };
	GLfloat sg[3] = { 400.0f,75.0f,-100.0f };
	GLfloat sh[3] = { 300.0f,75.0f,-100.0f };


	// Sciany skladowe
	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);	
	glVertex3fv(sb);	
	glVertex3fv(sc);
	glVertex3fv(sd);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(sg);	
	glVertex3fv(sc);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	glColor3f(0.7f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(se);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(se);
	glEnd();
}

void podloze_obiekt2(void)
{

	// Parametry wierzcholkow
	GLfloat sa[3] = { 0.0f,-25.0f,-100.0f };
	GLfloat sb[3] = { 400.0f,-25.0f,-100.0f };
	GLfloat sc[3] = { 400.0f,75.0f,-100.0f };
	GLfloat sd[3] = { 0.0f,75.0f,-100.0f };
	GLfloat se[3] = { 0.0f,-25.0f,-200.0f };
	GLfloat sf[3] = { 400.0f,-25.0f,-200.0f };
	GLfloat sg[3] = { 400.0f,75.0f,-200.0f };
	GLfloat sh[3] = { 000.0f,75.0f,-200.0f };


	// Sciany skladowe
	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sc);
	glVertex3fv(sd);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(sg);
	glVertex3fv(sc);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	glColor3f(0.7f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(se);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(se);
	glEnd();
}

void podloze_obiekt3(void)
{

	// Parametry wierzcholkow
	GLfloat sa[3] = { -100.0f,-25.0f,-100.0f };
	GLfloat sb[3] = { 0.0f,-25.0f,-100.0f };
	GLfloat sc[3] = { 0.0f,75.0f,-100.0f };
	GLfloat sd[3] = { -100.0f,75.0f,-100.0f };
	GLfloat se[3] = { -100.0f,-25.0f,-325.0f };
	GLfloat sf[3] = { 0.0f,-25.0f,-325.0f };
	GLfloat sg[3] = { 0.0f,75.0f,-325.0f };
	GLfloat sh[3] = { -100.0f,75.0f,-325.0f };


	// Sciany skladowe
	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sc);
	glVertex3fv(sd);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(sg);
	glVertex3fv(sc);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	glColor3f(0.7f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(se);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(se);
	glEnd();
}

void podloze_obiekt4(void)
{

	// Parametry wierzcholkow
	GLfloat sa[3] = { -100.0f,-25.0f,-400.0f };
	GLfloat sb[3] = { 300.0f,-25.0f,-400.0f };
	GLfloat sc[3] = { 300.0f,75.0f,-400.0f };
	GLfloat sd[3] = { -100.0f,75.0f,-400.0f };
	GLfloat se[3] = { -100.0f,-25.0f,-500.0f };
	GLfloat sf[3] = { 300.0f,-25.0f,-500.0f };
	GLfloat sg[3] = { 300.0f,75.0f,-500.0f };
	GLfloat sh[3] = { -100.0f,75.0f,-500.0f };


	// Sciany skladowe
	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sc);
	glVertex3fv(sd);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(sg);
	glVertex3fv(sc);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	glColor3f(0.7f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(se);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(se);
	glEnd();
}

void podloze_obiekt5(void)
{

	// Parametry wierzcholkow
	GLfloat sa[3] = { 200.0f,-25.0f,-400.0f };
	GLfloat sb[3] = { 300.0f,-25.0f,-400.0f };
	GLfloat sc[3] = { 300.0f,75.0f,-400.0f };
	GLfloat sd[3] = { 200.0f,75.0f,-400.0f };
	GLfloat se[3] = { 200.0f,-25.0f,-700.0f };
	GLfloat sf[3] = { 300.0f,-25.0f,-700.0f };
	GLfloat sg[3] = { 300.0f,75.0f,-700.0f };
	GLfloat sh[3] = { 200.0f,75.0f,-700.0f };


	// Sciany skladowe
	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sc);
	glVertex3fv(sd);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(sg);
	glVertex3fv(sc);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	glColor3f(0.7f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(se);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(se);
	glEnd();
}

void podloze_obiekt6(void)
{

	// Parametry wierzcholkow
	GLfloat sa[3] = { -400.0f,-25.0f,-100.0f };
	GLfloat sb[3] = { -300.0f,-25.0f,-100.0f };
	GLfloat sc[3] = { -300.0f,75.0f,-100.0f };
	GLfloat sd[3] = { -400.0f,75.0f,-100.0f };
	GLfloat se[3] = { -400.0f,-25.0f,-500.0f };
	GLfloat sf[3] = { -300.0f,-25.0f,-500.0f };
	GLfloat sg[3] = { -300.0f,75.0f,-500.0f };
	GLfloat sh[3] = { -400.0f,75.0f,-500.0f };


	// Sciany skladowe
	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sc);
	glVertex3fv(sd);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(sg);
	glVertex3fv(sc);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	glColor3f(0.7f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(se);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(se);
	glEnd();
}

void podloze_obiekt7(void)
{

	// Parametry wierzcholkow
	GLfloat sa[3] = { 0.0f,-25.0f,250.0f };
	GLfloat sb[3] = { 400.0f,-25.0f,250.0f };
	GLfloat sc[3] = { 400.0f,75.0f,250.0f };
	GLfloat sd[3] = { 0.0f,75.0f,250.0f };
	GLfloat se[3] = { 0.0f,-25.0f,150.0f };
	GLfloat sf[3] = { 400.0f,-25.0f,150.0f };
	GLfloat sg[3] = { 400.0f,75.0f,150.0f };
	GLfloat sh[3] = { 0.0f,75.0f,150.0f };


	// Sciany skladowe
	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sc);
	glVertex3fv(sd);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(sg);
	glVertex3fv(sc);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	glColor3f(0.7f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(se);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.7f, 0.2f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(se);
	glEnd();
}

void podloze_cel(void)
{

	// Parametry wierzcholkow
	GLfloat sa[3] = { 400.0f,-25.0f,-600.0f };
	GLfloat sb[3] = { 500.0f,-25.0f,-600.0f };
	GLfloat sc[3] = { 500.0f,75.0f,-600.0f };
	GLfloat sd[3] = { 400.0f,75.0f,-600.0f };
	GLfloat se[3] = { 400.0f,-25.0f,-700.0f };
	GLfloat sf[3] = { 500.0f,-25.0f,-700.0f };
	GLfloat sg[3] = { 500.0f,75.0f,-700.0f };
	GLfloat sh[3] = { 400.0f,75.0f,-700.0f };


	// Sciany skladowe
	glColor3f(0.0f, 1.0f, 0.7f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sc);
	glVertex3fv(sd);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.7f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(sg);
	glVertex3fv(sc);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.7f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sf);
	glVertex3fv(se);
	glVertex3fv(sh);
	glVertex3fv(sg);
	glEnd();

	glColor3f(0.0f, 0.46f, 0.33f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(se);
	glVertex3fv(sa);
	glVertex3fv(sd);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.7f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sd);
	glVertex3fv(sc);
	glVertex3fv(sg);
	glVertex3fv(sh);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.7f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3fv(sa);
	glVertex3fv(sb);
	glVertex3fv(sf);
	glVertex3fv(se);
	glEnd();
}


void lazik(void) {
	walec_0();
	walec_1();
	walec_2();
	walec_3();
	walec_4();
	walec_5();
	cialo_cale();
	ramie();
	ramie2();
	ramie3();
}

void podloze_obiekty(void) {
	podloze_obiekt();
	podloze_obiekt2();
	podloze_obiekt3();
	podloze_obiekt4();
	podloze_obiekt5();
	podloze_obiekt6();
	podloze_obiekt7();
	podloze_cel();
}


float pos_z2 = 0.0;
float pos_z = 0.1;
float phrase = 1;
float temp = 0;
GLfloat deg = 3.14f / 180.0f;

void RenderScene(void)
	{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glPolygonMode(GL_BACK,GL_LINE);
	podloze();
	podloze_obiekty();

		glPushMatrix();
			if ((pos_z >= 700.0 || pos_z <= -700.0) || (pos_z2 >= 500.0 || pos_z2 <= -500.0))
			{
				if ((pos_z >= -600.0 || pos_z <= -700.0) && (pos_z2 >= 500.0 || pos_z2 <= 400.0))
				{
					return 0;
				}
				phrase = phrase * (-1);
			}
			pos_z += phrase;
			glTranslatef(pos_z2, 0.0, pos_z);
			glRotatef(temp, 0, 20.0f, 0);
			lazik();
		glPopMatrix();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush;
}



void SetDCPixelFormat(HDC hDC)
	{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                                              // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0,0,0,0,0,0,                            // Not used to select mode
		0,0,                                    // Not used to select mode
		0,0,0,0,0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0,0,0 };                                // Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
	}



// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
	{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange,GreenRange,BlueRange;

	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	if(!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	nColors = 1 << pfd.cColorBits;	

	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +nColors*sizeof(PALETTEENTRY));


	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	RedRange = (1 << pfd.cRedBits) -1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) -1;

	for(i = 0; i < nColors; i++)
		{

		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double) pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char) NULL;
		}
		
	hRetPal = CreatePalette(pPal);

	SelectPalette(hDC,hRetPal,FALSE);
	RealizePalette(hDC);

	free(pPal);

	return hRetPal;
	}


int APIENTRY WinMain(   HINSTANCE       hInst,
						HINSTANCE       hPrevInstance,
						LPSTR           lpCmdLine,
						int                     nCmdShow)
	{
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	hInstance = hInst;

	wc.style                        = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc          = (WNDPROC) WndProc;
	wc.cbClsExtra           = 0;
	wc.cbWndExtra           = 0;
	wc.hInstance            = hInstance;
	wc.hIcon                        = NULL;
	wc.hCursor                      = LoadCursor(NULL, IDC_ARROW);

	wc.hbrBackground        = NULL;         
	
	wc.lpszMenuName         = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName        = lpszAppName;

	if(RegisterClass(&wc) == 0)
		return FALSE;

	hWnd = CreateWindow(
				lpszAppName,
				lpszAppName,

				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,

				50, 50,
				400, 400,
				NULL,
				NULL,
				hInstance,
				NULL);

	if(hWnd == NULL)
		return FALSE;

	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);

	while( GetMessage(&msg, NULL, 0, 0))
		{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}

	return msg.wParam;
	}

LRESULT CALLBACK WndProc(       HWND    hWnd,
							UINT    message,
							WPARAM  wParam,
							LPARAM  lParam)
	{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	switch (message)
		{

		case WM_CREATE:
			hDC = GetDC(hWnd);              

			SetDCPixelFormat(hDC);          

			hPalette = GetOpenGLPalette(hDC);

			hRC = wglCreateContext(hDC);
			wglMakeCurrent(hDC, hRC);
			SetupRC();
			glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			

			bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);
			
			glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
						 bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
			
			if(bitmapData)
			free(bitmapData);

			bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
			glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
						 bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
			
			if(bitmapData)
			free(bitmapData);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
			break;

		case WM_DESTROY:

			wglMakeCurrent(hDC,NULL);
			wglDeleteContext(hRC);

			if(hPalette != NULL)
				DeleteObject(hPalette);

			PostQuitMessage(0);
			break;

		case WM_SIZE:

			ChangeSize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_PAINT:
			{
			RenderScene();

			SwapBuffers(hDC);

			ValidateRect(hWnd,NULL);
			}
			break;

		case WM_QUERYNEWPALETTE:

			if(hPalette)
				{
				int nRet;

				SelectPalette(hDC, hPalette, FALSE);

				nRet = RealizePalette(hDC);

				InvalidateRect(hWnd,NULL,FALSE);

				return nRet;
				}
			break;

		case WM_PALETTECHANGED:

			if((hPalette != NULL) && ((HWND)wParam != hWnd))
				{

				SelectPalette(hDC,hPalette,FALSE);

				// Map entries to system palette
				RealizePalette(hDC);
				
				// Remap the current colors to the newly realized palette
				UpdateColors(hDC);
				return 0;
				}
			break;

		// Key press, check for arrow keys to do cube rotation.
		case WM_KEYDOWN:
		{
			if (wParam == VK_DOWN)
			{
				pos_z2 -= cos(temp * deg) * 5.0f;
				pos_z += sin(temp * deg) * 5.0f;
			}
			else if (wParam == VK_UP)
			{
				pos_z2 += cos(temp * deg) * 5.0f;
				pos_z -= sin(temp * deg) * 5.0f;
			}
			if (wParam == VK_RIGHT)
			{
				temp -= 5.0f;
			}
			if (wParam == VK_LEFT)
			{
				temp += 5.0f;
			}
			xRot = (const int)xRot % 360;
			yRot = (const int)yRot % 360;

			InvalidateRect(hWnd, NULL, FALSE);
		}
			break;

		// A menu command
		case WM_COMMAND:
			{
			switch(LOWORD(wParam))
				{
				// Exit the program
				case ID_FILE_EXIT:
					DestroyWindow(hWnd);
					break;

				// Display the about box
				case ID_HELP_ABOUT:
					DialogBox (hInstance,
						MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
						hWnd,
						AboutDlgProc);
					break;
				}
			}
			break;


	default:   // Passes it on if unproccessed
	    return (DefWindowProc(hWnd, message, wParam, lParam));

	}

    return (0L);
	}

BOOL APIENTRY AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
	{
	
    switch (message)
	{
		// Initialize the dialog box
	    case WM_INITDIALOG:
			{
			int i;
			GLenum glError;

			SetDlgItemText(hDlg,IDC_OPENGL_VENDOR,glGetString(GL_VENDOR));
			SetDlgItemText(hDlg,IDC_OPENGL_RENDERER,glGetString(GL_RENDERER));
			SetDlgItemText(hDlg,IDC_OPENGL_VERSION,glGetString(GL_VERSION));
			SetDlgItemText(hDlg,IDC_OPENGL_EXTENSIONS,glGetString(GL_EXTENSIONS));

			SetDlgItemText(hDlg,IDC_GLU_VERSION,gluGetString(GLU_VERSION));
			SetDlgItemText(hDlg,IDC_GLU_EXTENSIONS,gluGetString(GLU_EXTENSIONS));


			i = 0;
			do {
				glError = glGetError();
				SetDlgItemText(hDlg,IDC_ERROR1+i,gluErrorString(glError));
				i++;
				}
			while(i < 6 && glError != GL_NO_ERROR);


			return (TRUE);
			}
			break;

	    case WM_COMMAND:      
			{
			if(LOWORD(wParam) == IDOK)
				EndDialog(hDlg,TRUE);
		    }
			break;

		case WM_CLOSE:
			EndDialog(hDlg,TRUE);
			break;
		}

	return FALSE;
	}

int moving()
{
	switch (degree) {
	case 0: {
		a = 0;
		b = 0;
		break;
	}
	case 1: {
		a = 10 - 10 * cos(30 * GL_PI / 180);
		b = 10 * sin(30 * GL_PI / 180);
		break;
	}
	case 2: {
		a = 10 - 10 * sin(45 * GL_PI / 180);
		b = 10 * sin(45 * GL_PI / 180);
		break;
	}
	case 3: {
		a = 10 - 10 * cos(60 * GL_PI / 180);
		b = 10 * sin(60 * GL_PI / 180);
		break;
	}
	case 4: {
		a = 10;
		b = 10;
		break;
	}
	}
}
