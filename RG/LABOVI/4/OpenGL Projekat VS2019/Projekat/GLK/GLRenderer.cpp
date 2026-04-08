#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}

void CGLRenderer::DrawPot(double height, double R, double r)
{
	double gridUnit = gridSize / gridNum;

	glColor3f(1, 1, 1);
	DrawCylinder(height / 2 , height / 2, height / 2 + gridUnit / 4, 8);
	glTranslatef(0, height / 2  , 0);
	DrawCylinder(height / 3, height / 2 + gridUnit / 2, height / 2 + gridUnit / 2, 8);
	glTranslatef(0, height / 3, 0);


}

void CGLRenderer::DrawFigure(double height, double R, double r)
{
	double gridUnit = gridSize / gridNum;

	glPushMatrix();
	DrawPot(height, R , r);

	height = height / 2;
	R -= r/2;

	glColor3f(KAKI);
	DrawCylinder(height, R, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);

	// raskrsnica
	glPushMatrix();

	// dole levo
	glRotatef(45, 0, 0, 1); 
	glTranslatef(0, r, 0);
	glColor3f(KAKI);
	DrawCone(height, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);

	// srednje levo
	glPushMatrix();
	glRotatef(-45, 0, 0, 1);
	glTranslatef(0, r, 0);
	glColor3f(KAKI);
	DrawCone(height, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);
	glPopMatrix();

	// srednje levo levo
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, r, 0);
	glColor3f(KAKI);
	DrawCylinder(height, R, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);

	glPushMatrix();// srednje levo levo gore
	glRotatef(-45, 0, 0, 1);
	glTranslatef(0, r, 0);
	glColor3f(KAKI);
	DrawCylinder(height, R, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);

	glPopMatrix();
	// srednje levo levo dole
	glRotatef(45, 0, 0, 1);
	glTranslatef(0, r, 0);
	glColor3f(KAKI);
	DrawCylinder(height, R, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);

	glPopMatrix();
	glPushMatrix(); // raskrsnica

	// gore 
	glTranslatef(0, r, 0);
	glColor3f(KAKI);
	DrawCylinder(height, R, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);
	glColor3f(ZUT);
	glRotatef(rotAngle, 0, 0, 1);
	glTranslatef(0, r, 0);
	DrawCone(height, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);
	glTranslatef(0, r, 0);
	glColor3f(KAKI);
	DrawCone(height, R, 60);
	glPopMatrix();
	//desno
	glRotatef(-45, 0, 0, 1);
	glTranslatef(0, r, 0);
	DrawCylinder(height, R, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);
	glTranslatef(0, r, 0);
	glColor3f(KAKI);
	DrawCylinder(height, R, R, 60);
	glTranslatef(0, height, 0);
	glTranslatef(0, r, 0);
	glColor3f(BRAON);
	DrawSphere(r, 60, 60);

	glPopMatrix();
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBetha)
{
	double stepA = 2 * PI / nSegAlpha;
	double stepB = 2 * PI / nSegBetha;
	
	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i <= nSegAlpha; i++)
		{

			for (int j = 0; j <= nSegBetha; j++)
			{
				glVertex3f(r * cos(stepA * i) * cos(stepB * j), r * sin(stepA * i), r * cos(stepA * i) * sin(stepB * j));
				glVertex3f(r * cos(stepA * (i + 1)) * cos(stepB * j), r * sin(stepA * (i + 1)), r * cos(stepA * (i + 1)) * sin(stepB * j));

			}
		}
	}
	glEnd();


}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{

	double step = 2 * PI / nSeg;
	
	
	// donja osovina
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glVertex3f(r1 * cos(step * i), 0, r1 * sin(step * i));
		}
	}
	glEnd();



	// gornja osovina
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f(0, h, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glVertex3f(r2 * cos(step * i), h, r2 * sin(step * i));
		}
	}
	glEnd();

	// omotac
	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i <= nSeg; i++)
		{
			glVertex3f(r1 * cos(step * i), 0, r1 * sin(step * i));
			glVertex3f(r2 * cos(step * i), h, r2 * sin(step * i));
		}
		

	}
	glEnd();

}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{

	double step = PI * 2 / nSeg;

	glBegin(GL_TRIANGLE_FAN); // osovina
	{
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glVertex3f(r * cos(step * i), 0, r * sin(step * i)); // gledas kao obican krug da crtas na XZ osi
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN); // omotach
	{
		glVertex3f(0, h, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glVertex3f(r * cos(step * i), 0, r * sin(step * i));
		}
	}
	glEnd();



}

void CGLRenderer::DrawAxis()
{
	glLineWidth(5.0f);
	glPointSize(5.0f);

	double axisSize = 200;

	glBegin(GL_LINES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(axisSize, 0, 0);


		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, axisSize, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, axisSize);

	}
	glEnd();
}

void CGLRenderer::DrawGrid()
{
	double gridUnit = gridSize / gridNum;

	double half = gridSize / 2;

	glLineWidth(2);

	glColor3f(0.3, 0.3, 0.3);
	for (int i = 0; i <= gridNum; i++)
	{
		glBegin(GL_LINES);
		{
			double pos = i * gridUnit - half;
			
			glVertex3f(-half, 0, pos);
			glVertex3f(half, 0, pos);

			glVertex3f(pos, 0, -half);
			glVertex3f(pos, 0, half);


		}
		glEnd();
	}
}

void CGLRenderer::CalculatePosition()
{
	eyeX = eyeR * cos(toRad(angleXY)) * cos(toRad(angleXZ));
	eyeY = eyeR * sin(toRad(angleXY));
	eyeZ = eyeR * cos(toRad(angleXY)) * sin(toRad(angleXZ));
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
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
	
	glClearColor(0.5, 0.5, 1, 1);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_NORMALIZE);


	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	this->CalculatePosition();

	if (this->drawGrid)
	{
		DrawAxis();
		DrawGrid();
	}

	double gridUnit = gridSize / gridNum;

	double height = gridUnit * 2;
	double R = gridUnit / 2;
	double r = gridUnit / 4;


	DrawFigure(height, R, r);

	

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
	gluPerspective(40, aspect, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);


	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
