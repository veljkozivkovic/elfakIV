#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

UINT CGLRenderer::LoadTexture(const char* fn)
{

	UINT id;

	DImage img;
	img.Load(CString(fn));

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

void CGLRenderer::CalculatePosition()
{
	eyeX = eyeR * cos(toRad(aXY)) * cos(toRad(aXZ));
	eyeY = eyeR * sin(toRad(aXY));
	eyeZ = eyeR * cos(toRad(aXY)) * sin(toRad(aXZ));
	gluLookAt(eyeX, eyeY, eyeZ, cX, cY, cZ, upX, upY, upZ);

}

void CGLRenderer::DrawAxis(float aS)
{

	glLineWidth(3);
	glBegin(GL_LINES);
	{
		/*glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(aS, 0, 0);


		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, aS, 0);

		glColor3f(0,1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, aS);*/


		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(aS, 0, 0);


		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, aS, 0);

		glColor3f(0,0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, aS);

	}
	glEnd();
	glColor3f(1, 1, 1);

}

void CGLRenderer::DrawSphere(float r, int nSegA, int nSegB)
{
	float sA = PI / nSegA; // visina
	float sB = 2 * PI / nSegB; // sirina

	for (int i = 0; i < nSegA; i++)
	{
		float a1 = -PI / 2 + i * sA;
		float a2 = -PI / 2 + (i + 1) * sA;

		glBegin(GL_QUAD_STRIP);
		{
			for (int j = 0; j <= nSegB; j++)
			{
				float s = (float)j / nSegB;
				float t1 = (float)i / nSegA;
				float t2 = (float)(i + 1) / nSegA;

				float b = j * sB;

				float x1 = cos(a1) * cos(b);
				float y1 = sin(a1);
				float z1 = cos(a1) * sin(b);

				glNormal3f(x1, y1, z1);
				glTexCoord2f(s, t1);
				glVertex3f(r * x1, r * y1, r * z1);

				float x2 = cos(a2) * cos(b);
				float y2 = sin(a2);
				float z2 = cos(a2) * sin(b);

				glNormal3f(x2, y2, z2);
				glTexCoord2f(s, t2);
				glVertex3f(r * x2, r * y2, r * z2);

			}
		}
		glEnd();
	}


}

void CGLRenderer::DrawSphere(float r, int nSeg, float texU, float texV, float texR)
{
	float nSegA = nSeg;
	float nSegB = 2 * nSeg;

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

				float b = (float)j * sB;

				float x1 = cos(a1) * cos(b);
				float y1 = sin(a1);
				float z1 = cos(a1) * sin(b);



				float x2 = cos(a2) * cos(b);
				float y2 = sin(a2);
				float z2 = cos(a2) * sin(b);

				float s1 = texU + texR * x1; 
				float t1 = texV + texR * z1 ; 

				float s2 = texU + texR * x2; 
				float t2 = texV + texR * z2; 


				glNormal3f(x1, y1, z1);
				glTexCoord2f(s1, t1);
				glVertex3f(r * x1, r * y1, r * z1);


				glNormal3f(x2, y2, z2);
				glTexCoord2f(s2, t2);
				glVertex3f(r * x2, r * y2, r * z2);
			}
		}
		glEnd();
	}



}

void CGLRenderer::DrawCylinder(float r1, float r2, float h, int nSeg)
{

	float step = 2 * PI / nSeg;
	// donja 

	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);

		for (int i = 0; i <= nSeg; i++)
		{
			float x = cos(step * i);
			float z = sin(step * i);

			glTexCoord2f(0.5 + 0.5 * x, 0.5 + 0.5 * z);
			glVertex3f(x * r1, 0, z * r1);

		}
	}
	glEnd();

	// gornja
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, h, 0);

		for (int i = 0; i <= nSeg; i++)
		{
			float x = cos(step * i);
			float z = sin(step * i);

			glTexCoord2f(0.5 + 0.5 * x, 0.5 + 0.5 * z);
			glVertex3f(x * r2, h, z * r2);

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

			float x = cos(i * step);
			float z = sin(i * step);


			glNormal3f(k * x, r, k * z);

			glTexCoord2f(s, 0);
			glVertex3f(x * r1, 0, z * r1);

			glTexCoord2f(s, 1);
			glVertex3f(x * r2, h, z * r2);


		}
	}
	glEnd();


}

void CGLRenderer::DrawCone(float r, float h, int nSeg)
{

	// donja osovina

	float step = 2 * PI / nSeg;

	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);

		for (int i = 0; i <= nSeg; i++)
		{
			float x = cos(step * i);
			float z = sin(step * i);

			glTexCoord2f(0.5 + 0.5 * x, 0.5 + 0.5 * z);
			glVertex3f(x * r, 0, z * r);

		}
	}
	glEnd();


	// omotac
	float len = sqrt(r * r + h * h);
	float k = h / len;
	float dr = r / len;

	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, h, 0);
		for (int i = nSeg; i >= 0; i--)
		{
			float x = cos(step * i);
			float z = sin(step * i);
			
			glNormal3f(k * x, dr, k * z);
			glTexCoord2f(0.5 + 0.5 * x, 0.5 + 0.5 * z);
			glVertex3f(x * r, 0, z * r);
		}

	}
	glEnd();


}

void CGLRenderer::DrawCone(float r, float h, int nSeg, float texU, float texV, float texR)
{
	float step = 2 * PI / nSeg;


	// omotac
	float len = sqrt(r * r + h * h);
	float k = h / len;
	float dr = r / len;

	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, 1, 0);
		glTexCoord2f(texU, texV);
		glVertex3f(0, h, 0);
		for (int i = nSeg; i >= 0; i--)
		{
			float x = cos(step * i);
			float z = sin(step * i);

			glNormal3f(k * x, dr, k * z);
			glTexCoord2f(texU + texR * x, texV + texR * z);
			glVertex3f(x * r, 0, z * r);
		}

	}
	glEnd();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	glBindTexture(GL_TEXTURE_2D, spider);
	DrawSphere(r, 2 * nSeg, 1 / 4.0f, 1 / 4.0f, 1 / 4.0f);
	//glTranslatef(0, r/ 2, 0);
	DrawCone(r, h, nSeg, 3 / 4.0f, 3 / 4.0f, 1 / 4.0f);


	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawLeg()
{
	DrawLegSegment(1, 10, 5);
	glTranslatef(0, 10, 0);
	glRotatef(60, 1, 0, 0);
	DrawLegSegment(1, 10, 5);

}

//Napisati funkciju void CGLRenderer::DrawSpiderBody(), koja iscrtava telo pauka,
//sastavljenu od tri sfere(DrawSphere) : glave, grudnog dela i stomaka, poluprečnika 2, 3
//i 5, respektivno.Broj segmenata za sve tri sfere postaviti na 10, a teksturne koordinate
//tako da se grudni deo i stomak mapiraju na gornju levu četvrtinu, a glava na gornju
//desnu četvrtinu teksture spider.Glava i grudni deo su „spljošteni“ na 50 %, a stomak
//na 80 % po jednoj od osa.[10 poena]
void CGLRenderer::DrawSpiderBody()
{
	glPushMatrix();
	{
		glTranslatef(0, 3, 0);

		glBindTexture(GL_TEXTURE_2D, spider);
		glPushMatrix();
		glScalef(1, 0.5, 1);
		glTranslatef(-4, 0, 0);

		DrawSphere(2, 10, 3 / 4.0f, 1 / 4.0f, 1 / 4.0f);
		glPopMatrix();

		glPushMatrix();
		glScalef(1, 0.5, 1);
		DrawSphere(3, 10, 1 / 4.0f, 1 / 4.0f, 1 / 4.0f);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(6, 0, 0);
		glScalef(1, 0.8, 1);
		DrawSphere(4, 10, 1 / 4.0f, 1 / 4.0f, 1 / 4.0f);
		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
}

void CGLRenderer::SetLight()
{
	float l_pos[] = {1,1,1,1};
	float amb[4] = { 0.4, 0.4, 0.4, 1 };
	float diff[4] = { 1,1,1,1 };
	float spec[4] = { 1,1,1,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);


}

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}

void CGLRenderer::DrawEnvCube(double a)
{
	glEnable(GL_CULL_FACE);
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	float h = a / 2.0f;

	glPushMatrix();
	{
		UINT sides[6] = { front, left, back, right, top, bot };

		for (int i = 0; i < 4; i++)
		{
			glPushMatrix();

			glRotatef(90 * i, 0, 1, 0);
			glBindTexture(GL_TEXTURE_2D, sides[i]);

			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0); glVertex3f(-h, h, -h);
				glTexCoord2f(0, 1); glVertex3f(-h, -h, -h);
				glTexCoord2f(1, 1); glVertex3f(h, -h, -h);
				glTexCoord2f(1, 0); glVertex3f(h, h, -h);
			}
			glEnd();

			glPopMatrix();

		}

		
		

		glRotatef(-90, 1, 0, 0);

		glBindTexture(GL_TEXTURE_2D, sides[4]);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 0);
			glVertex3f(-h, h, h);

			glTexCoord2d(0, 1);
			glVertex3f(h, h, h);


			glTexCoord2d(1, 1);
			glVertex3f(h, -h, h);


			glTexCoord2d(1, 0);
			glVertex3f(-h, -h, h);
		}
		glEnd();

		glRotatef(-180, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, sides[5]);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 0);
			glVertex3f(-h, h, h);

			glTexCoord2d(0, 1);
			glVertex3f(h, h, h);


			glTexCoord2d(1, 1);
			glVertex3f(h, -h, h);


			glTexCoord2d(1, 0);
			glVertex3f(-h, -h, h);
		}
		glEnd();

	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

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

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	spider = LoadTexture("spider.png");
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

	CalculatePosition();


	
	SetLight();

	glTranslatef(0, 50, 0);
	DrawEnvCube(100);
	glTranslatef(0, -50, 0);

	if(this->axis)
	{
		glDisable(GL_LIGHTING);
		DrawAxis();
		glEnable(GL_LIGHTING);
	}
	if (this->light)
	{
		glEnable(GL_LIGHT0);
	}
	else
		glDisable(GL_LIGHT0);

	
	mat.MatFace();
	
	DrawSpiderBody();

	// legs
	glTranslatef(0, 3, 0);
	glRotatef(60, 1, 0, 0);


	glPushMatrix();
	{
		glRotatef(15, 0, 0, 1);
		DrawLeg();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(-15, 0, 0, 1);
		DrawLeg();
	}
	glPopMatrix();

	
	glPushMatrix();
	{
		glRotatef(45, 0, 0, 1);
		DrawLeg();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(-45, 0, 0, 1);
		DrawLeg();
	}
	glPopMatrix();
	
	// druga strana

	glRotatef(60, 1, 0, 0);


	glScalef(1, -1, 1);

	glPushMatrix();
	{
		glRotatef(15, 0, 0, 1);
		DrawLeg();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(-15, 0, 0, 1);
		DrawLeg();
	}
	glPopMatrix();


	glPushMatrix();
	{
		glRotatef(45, 0, 0, 1);
		DrawLeg();
	}
	glPopMatrix();
	glPushMatrix();
	{
		glRotatef(-45, 0, 0, 1);
		DrawLeg();
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
	gluPerspective(50, aspect, 0.1, 3000);
	glMatrixMode(GL_MODELVIEW);



	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
		
	
	glDeleteTextures(1, &spider);
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
