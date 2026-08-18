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

UINT CGLRenderer::LoadTexture(const char* name)
{
	UINT id;
	DImage img;
	img.Load(CString(name));
	img.Flip();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	if (img.GetDIBBits() == NULL) {
		AfxMessageBox(CString("Sliika nije ucitana! Proveri putanju."));
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());


	return id;
}

void CGLRenderer::DrawAxis(float aS)
{

	glLineWidth(3);
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

void CGLRenderer::DrawSphere(float r, int nSegA, int nSegB)
{

	float sA = PI / nSegA;
	float sB = 2 * PI / nSegB;

	for (int i = 0; i < nSegA; i++)
	{
		float a1 = -PI / 2 + i * sA;
		float a2 = -PI / 2 + (i + 1) * sA;
		glBegin(GL_QUAD_STRIP);
		{
			for (int j = 0; j <= nSegB; j++)
			{
				float b = j * sB;

				float s = (float)j / nSegB; // sirina
				float t1 = (float)i / nSegA; //visina
				float t2 = (float)(i + 1) / nSegA;


				float x = cos(a1) * cos(b);
				float y = sin(a1);
				float z = cos(a1) * sin(b);

				glNormal3f(x, y, z);
				glTexCoord2f(s, t1);
				glVertex3f(r * x, r * y, r * z);



				float x1 = cos(a2) * cos(b);
				float y1 = sin(a2);
				float z1 = cos(a2) * cos(b);
				glNormal3f(x1, y1, z1);
				glTexCoord2f(s, t2);
				glVertex3f(r * x1, r * y1, r * z1);

			}
		}
		glEnd();




	}



}

void CGLRenderer::DrawCylinder(float r1, float r2, float h, int nSeg)
{
	float step = 2 * PI / nSeg;
	
	// donji
	glNormal3f(0, -1, 0);
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glTexCoord2f(0.5 + 0.5 * cos(step * i), 0.5 + 0.5 * sin(step * i));
			glVertex3f(r1 * cos(step * i), 0, r1 * sin(step * i));
		}
	}
	glEnd();

	// gornji
	glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, h, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glTexCoord2f(0.5 + 0.5 * cos(step * i), 0.5 + 0.5 * sin(step * i));
			glVertex3f(r2 * cos(step * i), h, r2 * sin(step * i));
		}
	}
	glEnd();

	// omotac
	float dr = r1 - r2;
	float len = sqrt(dr * dr + h * h);
	float k = h / len;
	float r = dr / len;


	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i <= nSeg; i++)
		{
			float s = (float)i / nSeg;

			glNormal3f(k * cos(step * i), r, k * sin(step * i));
			glTexCoord2f(s, 0.0f);
			glVertex3f(r1 * cos(step * i), 0, r1 * sin(step * i));

			glTexCoord2f(s, 1.0f);
			glVertex3f(r2 * cos(step * i), h, r2 * sin(step * i));

		
		}
	}
	glEnd();

}

void CGLRenderer::DrawCone(float r, float h, int nSeg)
{
	float step = 2 * PI / nSeg;
	
	// donja osovina
	glNormal3f(0, -1, 0);
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(0.5, 0.5f);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glTexCoord2f(0.5 + 0.5 * cos(step * i), 0.5 + 0.5 * sin(step * i));
			glVertex3f(r * cos(step * i), 0, r * sin(step * i));

		}
	}
	glEnd();

	// omotac
	float len = sqrt(h * h + r * r);
	float k = h / len;
	float dr = r / len;

	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, h, 0);
		for (int i = nSeg; i >= 0; i--)
		{
			glNormal3f(k * cos(step * i), dr, k * sin(step * i));
			glTexCoord2f(0.5 + 0.5 * cos(step * i), 0.5 + 0.5 * sin(step * i));
			glVertex3f(r * cos(step * i), 0, r * sin(step * i));
		}
	}
	glEnd();
}

void CGLRenderer::DrawSpaceCube(double a)
{
	UINT strane[6] = { front,left,back,right,top,bot };

	double h = a / 2;



	glBindTexture(GL_TEXTURE_2D, 0);

}

void CGLRenderer::DrawTriangle(float d1, float d2, float rep)
{
	float ang = atan2(d1, d2);
	float d3 = sqrt(d1 * d1 + d2 * d2);
	float x = d1 * cos(ang) / d3;
	float y = d1 * sin(ang) / d3;


	glBegin(GL_TRIANGLES);
	{
		glNormal3f(0, 0, 1);

		glTexCoord2f(0.5 * rep, 0);
		glVertex3f(0, 0, 0);

		glTexCoord2f((0.5 + x) * rep, y * rep);
		glVertex3f(d1, 0, 0);


		glTexCoord2f(0.5 * rep, 1.0 * rep);
		glVertex3f(d1, d2, 0);


	}
	glEnd();

}

void CGLRenderer::SetLight()
{
	float l_pos[] = {0,0,1, 0};

	float l_amb[] = {0,0,0,1};
	float l_diff[] = {0.8,0.8,0.8,1};
	float l_spec[] = { 1,1,1,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);

	glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_spec);


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


	ShipT1 = LoadTexture("ShipT1.png");
	front = LoadTexture("front.jpg");
	left = LoadTexture("left.jpg");
	right = LoadTexture("right.jpg");
	back = LoadTexture("back.jpg");
	top = LoadTexture("top.jpg");
	bot = LoadTexture("bot.jpg");






	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	

	if (light)
	{
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}
	CalculatePosition();

	SetLight();

	glPushMatrix();
	{

		glTranslatef(eyeX, eyeY, eyeZ);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		DrawSpaceCube(60);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();


	if (drawAxis)
	{
		glDisable(GL_LIGHTING);
		DrawAxis();
		glEnable(GL_LIGHTING);
	}

	this->shipMat.MatFace();
	glBindTexture(GL_TEXTURE_2D, ShipT1);
	glPushMatrix();
	{
		glRotatef(angle, 1, 0, 0);
		glPushMatrix();
		{
			glTranslatef(0, 5, 0);
			glRotatef(100, 1, 0, 0);
			DrawTriangle(5.8, 2.15, 3);
			glRotatef(-100, 1, 0, 0);
			glRotatef(-100, 1, 0, 0);
			DrawTriangle(5.8, 2.15, 3);



		}
		glPopMatrix();
		{
			glTranslatef(0, 5, 0);
			glRotatef(-9, 0, 0, 1);
			glRotatef(75, 1, 0, 0);
			DrawTriangle(5.8, 2.15, 3);
			glRotatef(-75, 1, 0, 0);
			glRotatef(-75, 1, 0, 0);
			DrawTriangle(5.8, 2.15, 3);

		}
		glPopMatrix();
	}
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);



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

	glDeleteTextures(1, &ShipT1);
	glDeleteTextures(1, &front);
	glDeleteTextures(1, &left);
	glDeleteTextures(1, &right);
	glDeleteTextures(1, &back);
	glDeleteTextures(1, &top);
	glDeleteTextures(1, &bot);

		
		
		
		
		
		


	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
