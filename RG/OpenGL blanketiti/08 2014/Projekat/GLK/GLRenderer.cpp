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
	delete[] GL;
	delete[] GL2;
	delete[] GL3;
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
	glClearColor(0.5, 0.5, 0.5, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);

	tex = LoadTexture("OpenGL.bmp");

	


	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	SetWhiteLight();

	
	CalculatePosition();

	float l_pos[] = { 20, 20, 0, 1 };
	float amb[] = { 0,0,0,1 };
	float diff[] = { 1,1,1,1 };
	float spec[] = { 1,1,1,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	if (this->light)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

	if (this->light1)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);



	glDisable(GL_LIGHTING);
	DrawAxis();
	glEnable(GL_LIGHTING);

	DrawRubikCube(5, 3);

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

	glDeleteTextures(1, &tex);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::CalculatePosition()
{
	ex = er * cos(toRad(aXY)) * cos(toRad(aXZ));
	ey = er * sin(toRad(aXY));
	ez = er * cos(toRad(aXY)) * sin(toRad(aXZ));
	gluLookAt(ex, ey, ez, cx, cy, cz, ux, uy, uz);
}

void CGLRenderer::DrawAxis(float a)
{
	glLineWidth(3);
	glBegin(GL_LINES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(a, 0, 0);


		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, a, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, a);
	}
	glEnd();
	glColor3f(1, 1, 1);
}

UINT CGLRenderer::LoadTexture(const char* s)
{
	UINT id = 0;
	DImage img;
	img.Load(CString(s));

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

void CGLRenderer::DrawCube(float a)
{
	glBindTexture(GL_TEXTURE_2D, tex);
	float crvena[] = { 1.0f, 0.0f, 0.0f };
	float plava[] = { 0.0f, 0.0f, 1.0f };
	float narandzasta[] = { 1.0f, 0.5f, 0.0f };
	float zelena[] = { 0.0f, 1.0f, 0.0f };
	float bela[] = { 1.0f, 1.0f, 1.0f };
	float zuta[] = { 1.0f, 1.0f, 0.0f };

	float* boje[] = { crvena, plava, narandzasta, zelena, bela, zuta };

	float** pBoje = boje;

	/*0	Prednja	Crvena	
	  1	Desna	Plava	
	  2	Zadnja	Narandžasta	
	  3	Leva	Zelena	
	  4	Gornja	Bela	
	  5	Donja*/

	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		{
			glRotatef(90 * i, 0, 1, 0);
			SetMaterial(pBoje[i][0], pBoje[i][1], pBoje[i][2]);

			if (this->trenutni == i)
				DrawSide(a / 2., true, this->GL);
			else if (this->trenutni2 == i)
				DrawSide(a / 2., true, this->GL2);
			else
				DrawSide(a / 2., false, nullptr);
		}
		glPopMatrix();
	}

	// TOP:
	glPushMatrix();
	{
		glRotatef(-90 , 1, 0, 0);
		SetMaterial(pBoje[4][0], pBoje[4][1], pBoje[4][2]);
		if (this->trenutni3 == TOP)
		{
			DrawSide(a / 2., true, this->GL3);
		}
		else
			DrawSide(a / 2., false, nullptr);
	}
	glPopMatrix();

	// BOT:
	glPushMatrix();
	{
		glRotatef(90, 1, 0, 0);
		SetMaterial(pBoje[5][0], pBoje[5][1], pBoje[5][2]);
		if (this->trenutni3 == BOT)
		{
			DrawSide(a / 2., true, this->GL3);
		}
		else
			DrawSide(a / 2., false, nullptr);
	}
	glPopMatrix();


	glBindTexture(GL_TEXTURE_2D, 0);

	
	
	

}

void CGLRenderer::DrawSide(float h, bool texture, float* gl)
{
	float unit = 1.0f / (float)mCount;
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	{
		if (texture)
			glTexCoord2f(gl[0], gl[1]);
		glVertex3f(-h, h, h);


		if (texture)
			glTexCoord2f(gl[0], gl[1] + unit);
		glVertex3f(-h, -h, h);


		if (texture)
			glTexCoord2f(gl[0] + unit, gl[1] + unit);
		glVertex3f(h, -h, h);


		if (texture)
			glTexCoord2f(gl[0] + unit, gl[1]);
		glVertex3f(h, h, h);
	}
	glEnd();

}

void CGLRenderer::DrawRubikCube(float a, int count)
{
	float unit = 1.0f / (float)mCount;
	
	// LEVO 
	glPushMatrix();
	{

		glTranslatef(-a * 0.05, 0, 0);
		glTranslatef(-a, 0, 0);
		glRotatef(rot1, 1, 0, 0);

		glPushMatrix();
		{
			// PREDNJA strana
			this->trenutni = FRONT;
			this->trenutni2 = LEFT;

			glTranslatef(0, 0, a);
			glTranslatef(0, 0, a * 0.05f);



			glTranslatef(0, a, 0);
			glTranslatef(0, a * 0.05f, 0);
			// prednje gore 
			this->trenutni3 = TOP;
			this->GL3[0] = 0;
			this->GL3[1] = 2 * unit;

			this->GL[0] = 0;
			this->GL[1] = 0;

			this->GL2[0] = 2 * unit;
			this->GL2[1] = 0;

			DrawCube(5);
			this->trenutni3 = NONE;


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			// prednje srednje
			this->GL[0] = 0;
			this->GL[1] = unit;

			this->GL2[0] = 2 * unit;
			this->GL2[1] = unit;

			DrawCube(5);


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);

			// prednje donje
			this->trenutni3 = BOT;
			this->GL3[0] = 0;
			this->GL3[1] = 0;

			this->GL[0] = 0;
			this->GL[1] = 2 * unit;

			this->GL2[0] = 2 * unit;
			this->GL2[1] = 2 * unit;

			DrawCube(5);
			this->trenutni3 = NONE;

		}
		glPopMatrix();


		glPushMatrix();
		{
			// SREDNJA strana


			this->trenutni = NONE;
			this->trenutni2 = LEFT;
			this->GL[0] = -1;
			this->GL[1] = -1;

			glTranslatef(0, a, 0);
			glTranslatef(0, a * 0.05f, 0);
			// srednje gore
			this->trenutni3 = TOP;
			this->GL3[0] = 0;
			this->GL3[1] =  unit;

			this->GL2[0] = unit;
			this->GL2[1] = 0;
			DrawCube(5);
			this->trenutni3 = NONE;


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			//srednje srednje
			this->GL2[0] = unit;
			this->GL2[1] = unit;
			DrawCube(5);


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			// srednje dole
			this->trenutni3 = BOT;
			this->GL3[0] = 0;
			this->GL3[1] = unit;

			this->GL2[0] = unit;
			this->GL2[1] = 2 * unit;
			DrawCube(5);
			this->trenutni3 = NONE;

		}
		glPopMatrix();


		glPushMatrix();
		{
			// ZADNJA strana
			glTranslatef(0, 0, -a);
			glTranslatef(0, 0, -a * 0.05f);
			this->trenutni = BACK;
			this->trenutni2 = LEFT;


			glTranslatef(0, a, 0);
			glTranslatef(0, a * 0.05f, 0);
			// zadnje gore
			this->trenutni3 = TOP;
			this->GL3[0] = 0;
			this->GL3[1] = 0;

			this->GL[0] = 2 * unit;
			this->GL[1] = 0;

			this->GL2[0] = 0;
			this->GL2[1] = 0;
			DrawCube(5);
			this->trenutni3 = NONE;


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			//zadnje srednje
			this->GL[0] = 2 * unit;
			this->GL[1] = unit;

			this->GL2[0] = 0;
			this->GL2[1] = unit;
			DrawCube(5);



			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			//zadnje dole
			this->trenutni3 = BOT;
			this->GL3[0] = 0;
			this->GL3[1] = 2 * unit;

			this->GL[0] = 2 * unit;
			this->GL[1] = 2 * unit;

			this->GL2[0] = 0;
			this->GL2[1] = 2 * unit;
			DrawCube(5);
			this->trenutni3 = NONE;



		}
		glPopMatrix();
	}
	glPopMatrix();



	// SREDNJE
	glPushMatrix();
	{
		glRotatef(rot2, 1, 0, 0);

		// PREDNJA strana
		glPushMatrix();
		{
			this->trenutni = FRONT;
			this->trenutni2 = NONE;
			this->GL2[0] = -1;
			this->GL2[1] = -1;

			glTranslatef(0, 0, a);
			glTranslatef(0, 0, a * 0.05f);

			glTranslatef(0, a, 0);
			glTranslatef(0, a * 0.05f, 0);
			// prednje gornje
			this->trenutni3 = TOP;
			this->GL3[0] = unit;
			this->GL3[1] = 2 * unit;

			this->GL[0] =  unit;
			this->GL[1] = 0;

			DrawCube(5);
			this->trenutni3 = NONE;


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			// prednje srednje
			this->GL[0] =  unit;
			this->GL[1] = unit;



			DrawCube(5);


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			// prednje donje
			this->trenutni3 = BOT;
			this->GL3[0] = unit;
			this->GL3[1] = 0;

			this->GL[0] =  unit;
			this->GL[1] = 2 * unit;


			DrawCube(5);
			this->trenutni3 = NONE;


		}
		glPopMatrix();


		//SREDNJA strana
		glPushMatrix();
		{
			this->trenutni = NONE;
			this->trenutni2 = NONE;
			this->GL[0] = -1;
			this->GL[1] = -1;
			this->GL2[0] = -1;
			this->GL2[1] = -1;

			glTranslatef(0, a, 0);
			glTranslatef(0, a * 0.05f, 0);
			// srednje gore
			this->trenutni3 = TOP;
			this->GL3[0] = unit;
			this->GL3[1] =  unit;

			DrawCube(5);
			this->trenutni3 = NONE;


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			//srednje srednje

			DrawCube(5);


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			// srednje dole
			this->trenutni3 = BOT;
			this->GL3[0] = unit;
			this->GL3[1] = unit;

			DrawCube(5);
			this->trenutni3 = NONE;

		}
		glPopMatrix();


		//ZADNJA strana
		glPushMatrix();
		{
			// ZADNJA strana
			glTranslatef(0, 0, -a);
			glTranslatef(0, 0, -a * 0.05f);
			this->trenutni = BACK;
			this->trenutni2 = NONE;
			this->GL2[0] = -1;
			this->GL2[1] = -1;


			glTranslatef(0, a, 0);
			glTranslatef(0, a * 0.05f, 0);
			// zadnje gore
			this->trenutni3 = TOP;
			this->GL3[0] = unit;
			this->GL3[1] = 0;

			this->GL[0] = unit;
			this->GL[1] = 0;

			DrawCube(5);
			this->trenutni3 = NONE;


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			//zadnje srednje
			this->GL[0] =  unit;
			this->GL[1] = unit;

			DrawCube(5);



			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			//zadnje dole
			this->trenutni3 = BOT;
			this->GL3[0] = unit;
			this->GL3[1] = 2 * unit;

			this->GL[0] =  unit;
			this->GL[1] = 2 * unit;

			DrawCube(5);
			this->trenutni3 = NONE;
		}
		glPopMatrix();
	}
	glPopMatrix();




	//DESNO
	glPushMatrix();
	{
		glTranslatef(a * 0.05, 0, 0);
		glTranslatef(a, 0, 0);
		glRotatef(rot3, 1, 0, 0);

		// PREDNJA strana
		glPushMatrix();
		{
			this->trenutni = FRONT;
			this->trenutni2 = RIGHT;

			glTranslatef(0, 0, a);
			glTranslatef(0, 0, a * 0.05f);


			glTranslatef(0, a, 0);
			glTranslatef(0, a * 0.05f, 0);
			// prednje gore
			this->trenutni3 = TOP;
			this->GL3[0] = 2 * unit;
			this->GL3[1] = 2 * unit;

			this->GL[0] = 2 * unit;
			this->GL[1] = 0;

			this->GL2[0] = 0;
			this->GL2[1] = 0;

			DrawCube(5);
			this->trenutni3 = NONE;


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			// prednje srednje
			this->GL[0] = 2 * unit;
			this->GL[1] = unit;

			this->GL2[0] = 0;
			this->GL2[1] = unit;

			DrawCube(5);


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			// prednje donje
			this->trenutni3 = BOT;
			this->GL3[0] = 2 * unit;
			this->GL3[1] = 0;

			this->GL[0] = 2 * unit;
			this->GL[1] = 2 * unit;

			this->GL2[0] = 0;
			this->GL2[1] = 2 * unit;

			DrawCube(5);
			this->trenutni3 = NONE;

		}
		glPopMatrix();




		//SREDNJA strana
		glPushMatrix();
		{
			this->trenutni = NONE;
			this->trenutni2 = RIGHT;
			this->GL[0] = -1;
			this->GL[1] = -1;
			

			glTranslatef(0, a, 0);
			glTranslatef(0, a * 0.05f, 0);
			// srednje gore
			this->trenutni3 = TOP;
			this->GL3[0] = 2 * unit;
			this->GL3[1] =  unit;

			this->GL2[0] = unit;
			this->GL2[1] = 0;

			DrawCube(5);
			this->trenutni3 = NONE;


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			//srednje srednje
			this->GL2[0] = unit ;
			this->GL2[1] =  unit;

			DrawCube(5);


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			// srednje dole
			this->trenutni3 = BOT;
			this->GL3[0] = 2 * unit;
			this->GL3[1] = unit;

			this->GL2[0] = unit;
			this->GL2[1] = 2 * unit;

			DrawCube(5);
			this->trenutni3 = NONE;

		}
		glPopMatrix();




		//ZADNJA strana
		glPushMatrix();
		{
			glTranslatef(0, 0, -a);
			glTranslatef(0, 0, -a * 0.05f);
			this->trenutni = BACK;
			this->trenutni2 = RIGHT;
			


			glTranslatef(0, a, 0);
			glTranslatef(0, a * 0.05f, 0);
			// zadnje gore
			this->trenutni3 = TOP;
			this->GL3[0] = 2 * unit;
			this->GL3[1] = 0;

			this->GL[0] = 0;
			this->GL[1] = 0;

			this->GL2[0] = 2 *unit;
			this->GL2[1] = 0;

			DrawCube(5);
			this->trenutni3 = NONE;


			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			//zadnje srednje
			this->GL[0] = 0;
			this->GL[1] = unit;

			this->GL2[0] = 2 * unit;
			this->GL2[1] = unit;

			DrawCube(5);



			glTranslatef(0, -a, 0);
			glTranslatef(0, -a * 0.05f, 0);
			//zadnje dole
			this->trenutni3 = BOT;
			this->GL3[0] = 2 * unit;
			this->GL3[1] = 2 * unit;

			this->GL[0] = 0;
			this->GL[1] = 2 * unit;

			this->GL2[0] = 2 * unit;
			this->GL2[1] = 2 * unit;

			DrawCube(5);
			this->trenutni3 = NONE;

		}
		glPopMatrix();



	}
	glPopMatrix();

}

void CGLRenderer::SetWhiteLight()
{


	float l_pos1[] = { 0, 0, 0, 1 };
	float amb1[] = { 0,0,0,1 };
	float diff1[] = { 1,1,0,1 };
	float spec1[] = { 1,1,0,1 };

	float l_dir[] = {0,0,-1 };


	glLightfv(GL_LIGHT1, GL_POSITION, l_pos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l_dir);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 13);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);



}

void CGLRenderer::SetMaterial(float r, float g, float b)
{
	float a[4] = { r * 0.2f,  g * 0.2f , b * 0.2f , 1 };
	float d[4] = { r,g,b,1 };
	float s[4] = { r,g,b,1 };
	float e[4] = { 0,0,0,1 };
	float sh = 64;

	GLenum face = GL_FRONT_AND_BACK;

	glMaterialfv(face, GL_AMBIENT, a);
	glMaterialfv(face, GL_DIFFUSE, d);
	glMaterialfv(face, GL_SPECULAR, s);
	glMaterialfv(face, GL_EMISSION, e);
	
	glMaterialf(face, GL_SHININESS, sh);


}
