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

void CGLRenderer::DrawAxis(float as)
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
		glVertex3f(0,0 , as);
	}
	glEnd();
	glColor3f(1, 1, 1);
}

void CGLRenderer::SetLight()
{

}

UINT CGLRenderer::LoadTexture(const char* name)
{
	UINT id;
	DImage img;
	img.Load(CString(name));


	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S ,GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());



	return id;
}

void CGLRenderer::DrawEnvCube(float a)
{
	UINT sides[6] = { front, left,back, right, bot,top };

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		{
			glBindTexture(GL_TEXTURE_2D, sides[i]);

			glRotatef(i * (-90.0f), 0, 1, 0);
			DrawSide(a / 2.0f);
			glBindTexture(GL_TEXTURE_2D, 0);

		}
		glPopMatrix();
	}



	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, sides[4]);

		glRotatef(-90.0f, 1, 0, 0);
		DrawSide(a / 2.0f);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	glPopMatrix();

	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, sides[5]);

		glRotatef(90.0f, 1, 0, 0);
		DrawSide(a / 2.0f);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	glPopMatrix();

}

void CGLRenderer::DrawSide(float h)
{
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
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);


	excavator = LoadTexture("excavator.png");
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


	if (axis) {
		glDisable(GL_LIGHTING);
		DrawAxis();
		glEnable(GL_LIGHTING);
	}


	glPushMatrix();
	{
		glDisable(GL_LIGHTING);
		{
			glRotatef(this->rKab, 0, 1, 0);
			DrawBase();
			glTranslatef(0, 2, 0);
			DrawBody();

			glTranslatef(2, 0, 0);


			glTranslatef(0, 1, 0);
			glRotatef(this->r1, 0, 0, 1);
			glTranslatef(0, -1, 0);

			DrawArm(1);

			glTranslatef(0, 7, 0);

			glTranslatef(0, 1, 0);
			glRotatef(-90 + this->r2, 0, 0, 1);
			glTranslatef(0, -1, 0);

			DrawArm(1.5);


			glTranslatef(0, 7, 0);

			//float h = 4;
			//float uH = h / 6;

			//float w = 4;
			//float uW = w / 8;
			glTranslatef(1, 0, 0);
			glTranslatef(0, -4 / 3.0f, 0);





			glTranslatef(-1, 0, 0);
			glTranslatef(0, 4 / 3.0f, 0);

			glRotatef(rVilj, 0, 0, 1);

			glTranslatef(1, 0, 0);
			glTranslatef(0, -4 / 3.0f, 0);

			DrawFork();
		}
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();


	glDisable(GL_LIGHTING);
	glTranslatef(0, 50, 0);
	DrawEnvCube(100);
	glTranslatef(0, -50, 0);
	glEnable(GL_LIGHTING);



	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	glBegin(GL_TRIANGLE_FAN);
	{
		for (int i = 0; i < n; i++)
		{
			glTexCoord2f(texCoords[i].x, texCoords[i].y);
			glVertex3f(points[i].x, points[i].y, 0);
		}
	}
	glEnd();
}

void CGLRenderer::DrawFork()
{
	glBindTexture(GL_TEXTURE_2D, excavator);
	POINTF* p = new POINTF[6];
	float h = 4;
	float uH = h / 6;

	float w = 4;
	float uW = w / 8;

	p[0].x = -w / 2; p[0].y = h - uH;

	p[1].x = -w / 2 + uW; p[1].y = h;

	p[2].x = w / 2 - uW; p[2].y = h;

	p[3].x = w / 2; p[3].y = h - uH;

	p[4].x = w / 2; p[4].y = 0;

	p[5].x = -w / 2; p[5].y = 0;



	POINTF* t = new POINTF[6];
	float tUnit = 1.0f / 16.0f;

	t[0].x = 0; t[0].y = 1 * tUnit;
	t[1].x = 1 * tUnit; t[1].y = 0 * tUnit;

	t[2].x = 7 * tUnit; t[2].y = 0 * tUnit;


	t[3].x = 8 * tUnit; t[3].y = 1 * tUnit;
	t[4].x = 8 * tUnit; t[4].y = 6 * tUnit;
	t[5].x = 0 * tUnit; t[5].y = 6 * tUnit;





	DrawExtrudedPolygon(p, t, 6, 1, 0.7, 0.7, 0.7);

	delete[] p;
	delete[] t;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawArm(double zh)
{
	glBindTexture(GL_TEXTURE_2D, excavator);
	POINTF* p = new POINTF[6];
	float h = 8;
	float uH = h / 16;

	float w = 2;
	float uW = w / 4;

	p[0].x = -w / 2 + uW; p[0].y = 0;

	p[1].x = -w / 2 ; p[1].y = uH;

	p[2].x = -w / 2 + uW; p[2].y = h;

	p[3].x = w / 2 - uW; p[3].y = h ;

	p[4].x = w / 2; p[4].y = uH;

	p[5].x = w / 2 - uW; p[5].y = 0;



	POINTF* t = new POINTF[6];
	float tUnit = 1.0f / 16.0f;

	t[0].x = 0; t[0].y = 9 * tUnit;
	t[1].x = 1 * tUnit; t[1].y = 8 * tUnit;
	t[2].x = 16 * tUnit; t[2].y = 9 * tUnit;
	t[3].x = 16 * tUnit; t[3].y = 11 * tUnit;
	t[4].x = 1 * tUnit; t[4].y = 12 * tUnit;
	t[5].x = 0 * tUnit; t[5].y = 11 * tUnit;
	




	DrawExtrudedPolygon(p, t, 6, zh, 0.96, 0.5, 0.12);

	delete[] p;
	delete[] t;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawBase()
{
	glBindTexture(GL_TEXTURE_2D, excavator);
	POINTF* p = new POINTF[8];
	float h = 2;
	float uH = h / 4;
	
	float w = 6;
	float uW = w / 16;

	p[0].x = -w / 2 ; p[0].y = h - uH;
	p[1].x = -w / 2 + uW; p[1].y = h;

	p[2].x = w / 2 - uW ; p[2].y = h ;
	p[3].x = w / 2; p[3].y = h - uH;


	p[4].x = w / 2; p[4].y =uH;

	p[5].x = w / 2 - uW; p[5].y = 0;

	p[6].x = -w / 2 + uW; p[6].y = 0;

	p[7].x = -w / 2; p[7].y = uH;
	
	POINTF* t = new POINTF[8];
	float tUnit = 1.0f / 16.0f;

	t[0].x = 0; t[0].y = 13 * tUnit;//
	t[1].x = 1 * tUnit; t[1].y = 12 * tUnit;//
	t[2].x = 15 * tUnit; t[2].y = 12 * tUnit;//
	t[3].x = 16 * tUnit; t[3].y = 13 * tUnit;//
	t[4].x = 16 * tUnit; t[4].y = 15 * tUnit;//
	t[5].x = 15 * tUnit; t[5].y = 16 * tUnit;//
	t[6].x = 1 * tUnit; t[6].y = 16 * tUnit;
	t[7].x = 0; t[7].y = 15 * tUnit;




	DrawExtrudedPolygon(p, t, 8, 5, 0, 0, 0);

	delete[] p;
	delete[] t;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawBody()
{
	glBindTexture(GL_TEXTURE_2D, excavator);
	POINTF* p = new POINTF[5];
	float h = 4;
	float w = 4;
	p[0].x = - w / 2; p[0].y = h;
	p[1].x = 0; p[1].y = h ;
	p[2].x = w / 2; p[2].y = h/2;
	p[3].x = w / 2; p[3].y = 0;
	p[4].x = -w/2; p[4].y = 0;

	POINTF* t = new POINTF[5];
	float tUnit = 1.0f / 16.0f;

	t[0].x = 8 * tUnit; t[0].y = 0;
	t[1].x = 12 * tUnit; t[1].y = 0;
	t[2].x = 16 * tUnit; t[2].y = 4 * tUnit;
	t[3].x = 16 * tUnit; t[3].y = 8 * tUnit;
	t[4].x = 8 * tUnit; t[4].y = 8 * tUnit;

	DrawExtrudedPolygon(p, t, 5, 4, 0.96, 0.5, 0.12);

	delete[] p;
	delete[] t;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	glColor3f(1,1, 1);

	glPushMatrix();
	{
		glTranslatef(0, 0, zh / 2);
		DrawPolygon(points, texCoords, n);

	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0, 0, -zh / 2);
		DrawPolygon(points, texCoords, n);

	}
	glPopMatrix();

	glColor3f(r, g, b);

	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i < n; i++)
		{
			glVertex3f(points[i].x, points[i].y, -zh /2 );
			glVertex3f(points[i].x, points[i].y, zh / 2);

		}
		glVertex3f(points[0].x, points[0].y, -zh / 2);
		glVertex3f(points[0].x, points[0].y, zh / 2);
	}
	glEnd();



}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	double aspect = (double)w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, aspect, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	//excavator, front, left, right, back, top, bot;
	glDeleteTextures(1,&excavator);
	glDeleteTextures(1,&front);
	glDeleteTextures(1,&left);
	glDeleteTextures(1,&right);
	glDeleteTextures(1,&back);
	glDeleteTextures(1,&top);
	glDeleteTextures(1,&bot);



	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
