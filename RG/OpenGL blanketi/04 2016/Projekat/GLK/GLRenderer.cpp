#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

void CGLRenderer::CalculatePosition()
{
	eyeX = eyeR * cos(toRad(aXY)) * cos(toRad(aXZ));
	eyeY = eyeR * sin(toRad(aXY));
	eyeZ = eyeR * cos(toRad(aXY)) * sin(toRad(aXZ));
	gluLookAt(eyeX, eyeY, eyeZ, cX, cY, cZ, upX, upY, upZ);
}

void CGLRenderer::SetLight()
{
	float l_pos[] = { 1,1,-1,0 };

	float amb[4] = { 0.4, 0.4, 0.4, 1 };
	float diff[4] = { 1, 1, 1, 1 };
	float spec[4] = { 1, 1, 1, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);


}

void CGLRenderer::DrawAxis(float aS)
{
	glBegin(GL_LINES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(aS, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, aS, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, aS);

	}
	glEnd();
	glColor3f(1, 1, 1);
}

UINT CGLRenderer::LoadTexture(const char* name)
{
	UINT id;
	DImage img;
	img.Load(CString(name));


	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());





	return id;
}

void CGLRenderer::DrawCube(float a)
{
	glBindTexture(GL_TEXTURE_2D, env);
	float unitS = 0.25f;
	float unitT = 1.0f / 3;

	float gl[2];
	float gd[2];
	float dd[2];
	float dl[2];

	float h = a / 2.0f;
	

	// front 
	glPushMatrix();
	{
		glRotatef(90 * 0, 0, 1, 0);
		gl[0] = 1 * unitS;
		gl[1] = 0;

		gd[0] = 2 * unitS;
		gd[1] = 0;

		dd[0] = 2 * unitS;
		dd[1] = 1 * unitT;

		dl[0] = 1 * unitS;
		dl[1] = 1 * unitT;

		DrawSide(gl, gd, dd, dl, h);
	}
	glPopMatrix();

	
	//left
	glPushMatrix();
	{
		glRotatef(90 * 1, 0, 1, 0);
		gl[0] = 0;
		gl[1] = 2 * unitT;

		gd[0] = 0;
		gd[1] = 1 * unitT;

		dd[0] = 1* unitS;
		dd[1] = 1 * unitT;

		dl[0] = 1 * unitS;
		dl[1] = 2 * unitT;

		DrawSide(gl, gd, dd, dl, h);
	}
	glPopMatrix();


	//back
	glPushMatrix();
	{
		glRotatef(90 * 2, 0, 1, 0);
		gl[0] = 2 * unitS;
		gl[1] = 3 * unitT;

		gd[0] = 1 * unitS;
		gd[1] = 3 * unitT;

		dd[0] = 1 * unitS;
		dd[1] = 2 * unitT;

		dl[0] = 2 * unitS;
		dl[1] = 2 * unitT;

		DrawSide(gl, gd, dd, dl, h);
	}
	glPopMatrix();

	//right
	glPushMatrix();
	{
		glRotatef(90 * 3, 0, 1, 0);
		gl[0] = 3 * unitS;
		gl[1] = 1 * unitT;

		gd[0] = 3 * unitS;
		gd[1] = 2 * unitT;

		dd[0] = 2 * unitS;
		dd[1] = 2 * unitT;

		dl[0] = 2 * unitS;
		dl[1] = 1 * unitT;

		DrawSide(gl, gd, dd, dl, h);
	}
	glPopMatrix();


	//bot
	glPushMatrix();
	{
		glRotatef(-90, 1, 0, 0);
		gl[0] = 1 * unitS;
		gl[1] = 1 * unitT;

		gd[0] = 2 * unitS;
		gd[1] = 1 * unitT;

		dd[0] = 2 * unitS;
		dd[1] = 2 * unitT;

		dl[0] = 1 * unitS;
		dl[1] = 2 * unitT;

		DrawSide(gl, gd, dd, dl, h);
	}
	glPopMatrix();


	// top
	glPushMatrix();
	{
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 0, 1);


		gl[0] = 4 * unitS;
		gl[1] = 1 * unitT;

		gd[0] = 4 * unitS;
		gd[1] = 2 * unitT;

		dd[0] = 3 * unitS;
		dd[1] = 2 * unitT;

		dl[0] = 3 * unitS;
		dl[1] = 1 * unitT;

		DrawSide(gl, gd, dd, dl, h);
	}
	glPopMatrix();



	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawSide(float* gl, float* gd, float* dd, float* dl, float h)
{

	glBegin(GL_QUADS);
	{
		glTexCoord2f(gl[0], gl[1]);
		glVertex3f(-h, h, -h);

		glTexCoord2f(gd[0], gd[1]);
		glVertex3f(h, h, -h);


		glTexCoord2f(dd[0], dd[1]);
		glVertex3f(h, -h, -h);



		glTexCoord2f(dl[0], dl[1]);
		glVertex3f(-h, -h, -h);
	}
	glEnd();


}

void CGLRenderer::DrawTube(double r1, double r2, double h, int n)
{
	glBindTexture(GL_TEXTURE_2D, brick);
	float step = 2 * PI / (float)n;

	// donji
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= n; i++)
		{
			float x = cos(step * i);
			float z = sin(step * i);
			glTexCoord2f(0.5 + 0.5 * x, 0.5 + 0.5 * z);
			glVertex3f(r1 * x, 0, r1 * z);
		}
	}
	glEnd();

	// gornji
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0,h, 0);
		for (int i = 0; i <= n; i++)
		{
			float x = cos(step * i);
			float z = sin(step * i);
			glTexCoord2f(0.5 + 0.5 * x, 0.5 + 0.5 * z);
			glVertex3f(r2 * x, h, r2 * z);
		}
	}
	glEnd();

	//omotac

	double dr = r1 - r2;
	double len = sqrt(h * h + dr * dr);
	double k = h / len;
	double r = dr / len;

	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i <= n; i++)
		{
			float x = cos(step * i);
			float z = sin(step * i);
			
			float s = (float)i / n;
			
			glNormal3f(k * x, dr, k * z);

			glTexCoord2f(s, 0);
			glVertex3f(r1 * x, 0, r1 * z);

			glTexCoord2f(s, 1);
			glVertex3f(r2 * x, h, r2 * z);
		}
	}
	glEnd();


	glBindTexture(GL_TEXTURE_2D, 0);

}

void CGLRenderer::DrawCone(double r, double h, int n)
{
	float step = 2 * PI / (float)n;

	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= n; i++)
		{
			float x = cos(step * i);
			float z = sin(step * i);
			glVertex3f(r * x, 0, r * z);
		}
	}
	glEnd();

	float len = sqrt(r * r + h * h);
	float k = h / len;
	float dr = r / len;

	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, 1, 0);
		glVertex3f(0, h, 0);
		for (int i = n; i >= 0; i--)
		{
			float x = cos(step * i);
			float z = sin(step * i);

			glNormal3f(k * x, dr, k * z);
			glVertex3f(x * r, 0, z * r);


		}
	}
	glEnd();

}
//Napisati funkciju void CGLRenderer::DrawPaddle(double length, double width), koja
//iscrtava jedno krilo vetrenjače dužine length i širine width.Prva 1 / 8 dužine je širine 1 / 8 *
//width, a zatim se za 1 / 8 dužine širi do pune širine.U temenima definisati i normale.[5
//poena]
void CGLRenderer::DrawPaddle(double length, double width)
{
	// Koristimo QUAD_STRIP da izbegnemo problem sa konkavnim poligonom.
	// Krilo delimo na 3 spojena dela (sprata).
	glBegin(GL_QUAD_STRIP);
	{
		// BAZA (y = 0)
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);                    // Dole-Levo
		glVertex3f(width / 8.0, 0.0f, 0.0f);             // Dole-Desno

		// KRAJ 1. DELA (y = 1/8L) -> širina je i dalje 1/8
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, length / 8.0, 0.0f);            // Levo
		glVertex3f(width / 8.0, length / 8.0, 0.0f);     // Desno

		// KRAJ 2. DELA (y = 2/8L) -> širina se proširila na punu
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, length * 2.0 / 8.0, 0.0f);      // Levo
		glVertex3f(width, length * 2.0 / 8.0, 0.0f);     // Desno

		// VRH KRILA (y = L) -> širina ostaje puna
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, length, 0.0f);                  // Gore-Levo
		glVertex3f(width, length, 0.0f);                 // Gore-Desno
	}
	glEnd();
}

CGLRenderer::CGLRenderer(void)
{

}

CGLRenderer::~CGLRenderer(void)
{

}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	brick = LoadTexture("brick.png");
	env = LoadTexture("env.png");

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	CalculatePosition();

	SetLight();

	if (axis)
	{
		glDisable(GL_LIGHTING);
		DrawAxis();
		glEnable(GL_LIGHTING);
	}
	if (!light)
	{
		glDisable(GL_LIGHT0);
	}
	else {
		glEnable(GL_LIGHT0);
	}

	glDisable(GL_LIGHTING);
	DrawCube(100);
	glEnable(GL_LIGHTING);



	mat.MatFace();
	glPushMatrix();
	{
		glTranslatef(0, 0, -5);
		DrawTube(2.5, 3.5, 10, 32);
		glTranslatef(0, 10, 0);
		DrawCone(3.8, 2, 32);
		glTranslatef(0, 0, 5);
	}
	glPopMatrix();
	
	glPushMatrix();
	{
		glTranslatef(0, 10, -1.2);
		glRotatef(rotV, 0, 0, 1);
		DrawPaddle(8, 1.5);
		glRotatef(90, 0, 0, 1);
		DrawPaddle(8, 1.5);
		glRotatef(90, 0, 0, 1);
		DrawPaddle(8, 1.5);
		glRotatef(90, 0, 0, 1);
		DrawPaddle(8, 1.5);


	}
	glPopMatrix();
	glFlush();
	SwapBuffers(pDC->m_hDC);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	double aspect = (double)w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, aspect, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glDeleteTextures(1, &brick);
	glDeleteTextures(1, &env);


	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
