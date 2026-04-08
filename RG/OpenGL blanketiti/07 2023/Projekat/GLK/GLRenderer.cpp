#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

void CGLRenderer::CalculatePosition()
{
	eyeX = eyeR * cos(aXY) * cos(aXZ);
	eyeY = eyeR * sin(aXY);
	eyeZ = eyeR * cos(aXY) * sin(aXZ);
	gluLookAt(eyeX, eyeY, eyeZ, cx, cy, cz, ux, uy, uz);
}

void CGLRenderer::DrawAxes(float as)
{
	glLineWidth(3);
	glBegin(GL_LINES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(as, 0, 0);


		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, as, 0);


		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, as);


	}
	glEnd();
	glColor3f(1, 1, 1);
}

UINT CGLRenderer::LoadTexture(const char* s)
{
	UINT id;
	DImage img;
	img.Load(CString(s));
	if (img.GetDIBBits() == NULL)
	{
		AfxMessageBox(CString("Greška: Tekstura je nađena, ali su bitovi prazni!"));
		return 0;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());



	return id;
}

void CGLRenderer::DrawEnvCube(float a)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, side);


	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
			glRotatef(90 * i, 0, 1, 0);
			DrawSide(a / 2.0f);

		glPopMatrix();
	}


	glBindTexture(GL_TEXTURE_2D, top);

	glPushMatrix();
		
		glRotatef(00, 1, 0, 0);
		DrawSide(a / 2.0f);

	glPopMatrix();


	glBindTexture(GL_TEXTURE_2D, bot );

	glPushMatrix();

		glRotatef(-90, 1,0, 0);
		DrawSide(a / 2.0f);

	glPopMatrix();



	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawSide(float h)
{
	//glColor3f(0, 0, 0);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-h, h, -h);

		glTexCoord2f(1, 0);
		glVertex3f(h, h, -h);


		glTexCoord2f(1, 1);
		glVertex3f(h, -h, -h);


		glTexCoord2f(0, 1);
		glVertex3f(-h, -h, -h);


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
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	lamp = LoadTexture("lamp.jpg"); 
	side = LoadTexture("front.jpg"); 
	top = LoadTexture("top.jpg"); 
	bot = LoadTexture("bot.jpg");

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

//5. Napisati funkciju void CGLRenderer::DrawCylinder(double r1, double r2, double h, int
//	nSeg, int texMode, bool bIsOpen), koja iscrtava cilindar čiji je poluprečnika jedne baze r1, a
//	druge baze r2.Visina cilindra je h, a broj segmenata kojima se aproksimira omotač nSeg.
//	Parametar texMode definiše da li se za omotač cilindra koristi gornja(texMode = 0) ili donja
//	(texMode = 1) polovina teksture lamp.jpg, a isOpen da li se crtaju baze(ako je isOpen =
//		false crtaju se; uvek se koristi gornja polovina teksture).[20 poena]


void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen)
{
	float step = 2 * PI / nSeg;

	if (!bIsOpen)
	{
		// donji
		glBegin(GL_TRIANGLE_FAN);
		{
			glTexCoord2f(0.5, 0.25);
			glVertex3f(0, 0, 0);
			for (int i = 0; i <= nSeg; i++)
			{
				float x = cos(step * i);
				float z = sin(step * i);

				glTexCoord2f(0.5 + 0.5 * x, 0.25 + 0.25 * z);
				glVertex3f(r1 * x, 0, r1 * z);

			}
		}
		glEnd();
		// gornji
		glBegin(GL_TRIANGLE_FAN);
		{
			glTexCoord2f(0.5, 0.25);
			glVertex3f(0, h, 0);
			for (int i = 0; i <= nSeg; i++)
			{
				float x = cos(step * i);
				float z = sin(step * i);

				glTexCoord2f(0.5 + 0.5 * x, 0.25 + 0.25 * z);
				glVertex3f(r2 * x, h, r2 * z);

			}
		}
		glEnd();


	}


	//omotach

	glBegin(GL_QUAD_STRIP);
	{
		for (int i = nSeg; i >= 0; i--)
		{
			float start = texMode ? 0.5f : 0.0f;

			float stepTex =(float)i /  nSeg ;
			
			float x = cos(step * i);
			float z = sin(step * i);


			glTexCoord2f(1 - stepTex, start);
			glVertex3f(x * r2, h, z * r2);

			glTexCoord2f( 1- stepTex, start + 0.5);

			glVertex3f(x * r1, 0, z * r1);

			


		}
	}
	glEnd();



}

void CGLRenderer::DrawLampBase()
{
	glBindTexture(GL_TEXTURE_2D, lamp);

	DrawCylinder(7, 8, 2, 16, 1, false);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void CGLRenderer::DrawLampArm()
{
	glBindTexture(GL_TEXTURE_2D, lamp);

	glPushMatrix();
	{
		glPushMatrix();
		{
			glRotatef(90, 1, 0, 0);
			glTranslatef(0, -1, 0);
			DrawCylinder(3, 3, 2, 32, 1, false);
		}
		glPopMatrix();
		glTranslatef(0, 3, 0);
		DrawCylinder(1, 1, 15, 32, 1, false);

	}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

}
//Napisati funkciju void CGLRenderer::DrawLampHead()
// , koja iscrtava glavu lampe sastavljenu
//od četiri cilindra.Poluprečnici imaju vrednost : 
// 2, 3 ili 6, a visine : 1, 2 ili 5, zavisno od 
// dela
//(vidi sl. 3).[15 poena]
void CGLRenderer::DrawLampHead()
{  
	glBindTexture(GL_TEXTURE_2D, lamp);

	glPushMatrix();
	{
		DrawCylinder(6, 3, 5, 32, 0, true);
		glTranslatef(0, 5, 0);
		DrawCylinder(3, 3, 5, 32, 1, true);
		glPushMatrix();
		{
			glTranslatef(-2.25, 1.8, -1); // 
			glRotatef(90, 1, 0, 0);
			DrawCylinder(2, 2, 2, 32, 1, false);

		}
		glPopMatrix();
		
		glTranslatef(0, 5, 0);
		DrawCylinder(3, 2, 1, 32, 1, false);

		
		


	}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

}

void CGLRenderer::DrawLamp()
{
	glPushMatrix();
	{
		DrawLampBase();
		
		glRotatef(45 + rot1, 0, 0, 1);
		DrawLampArm(); // 1.5 siroko ovo 15 visoko

		glTranslatef(0, 1.5 + 15, 0);
		glRotatef(-90 + rot2, 0, 0, 1);
		DrawLampArm(); // 1.5 siroko ovo 15 visoko
		glTranslatef(0, 1.5 + 15, 0);
		
		glTranslatef(2, -7, 0);
		
		
		glTranslatef(-2.25, 6.8, 0);
		glRotatef(90 + rotG, 0, 0, 1);
		glTranslatef(2.25, -6.8, 0);

		DrawLampHead();	// visina je 11
						// sirina je 6


	}
	glPopMatrix();
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	CalculatePosition();

	glTranslatef(0, 50, 0);
	DrawEnvCube(100);
	glTranslatef(0, -50, 0);

	if(this->axes)
	{
		DrawAxes();


		

	}



	DrawLamp();
	
	

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
	// ... 
	//lamp, side, top, bot;
	glDeleteTextures(1, &side);
	glDeleteTextures(1, &lamp);
	glDeleteTextures(1, &top);
	glDeleteTextures(1, &bot);



	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
