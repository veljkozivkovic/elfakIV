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

UINT CGLRenderer::LoadTexture(const char* fName)
{
	UINT id;
	DImage img;
	img.Load(CString(fName));
	//img.Flip();
	if (img.GetDIBBits() == NULL) {
		AfxMessageBox(CString("Greška: Pikseli nisu uspešno pročitani iz: ") + CString(fName));
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

		for (int j = 0; j <= nSegB; j++)
		{

			float b = j * sB;
			float s = (float)j / nSegB; // sirina
			float t1 = (float)i / nSegA; //visina
			float t2 = (float)(i + 1) / nSegA; // visina


			glBegin(GL_QUAD_STRIP);
			{
				float x = cos(a1) * cos(b) ;
				float y = sin(a1);
				float z = cos(a1) * sin(b);

				glNormal3f(x, y, z);
				glTexCoord2f(s, t1);
				glVertex3f(r * x, r * y, r * z);

				float x1 = cos(a2) * cos(b);
				float y1 = sin(a2);
				float z1 = cos(a2) * sin (b);

				glNormal3f(x1, y1, z1);
				glTexCoord2f(s, t2);
				glVertex3f(r * x1, r * y1, r * z1);

			}
			glEnd();
		}


	}


}

void CGLRenderer::DrawCylinder(float r1, float r2, float h, int nSeg)
{
	float step = 2 * PI / nSeg;

	
	// donja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glTexCoord2f(0.5 + 0.5 * cos(step * i), 0.5 + 0.5 * sin(step * i));
			glVertex3f(r1 * cos(step * i), 0, r1 * sin(step * i));


		}
	}
	glEnd();
	
	// gornja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, h, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glTexCoord2f(0.5 + 0.5 * cos(step * i), 0.5 + 0.5 * sin(step * i));
			glVertex3f(r2 * cos(step * i), h, r2 * sin(step * i));
		}
	}
	glEnd();

	//omotac
	float dr = r1 - r2;
	float len = sqrt(dr * dr + h * h);
	float k = h/ len;
	float r = dr / len;

	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i <= nSeg; i++)
		{
			float se = (float)i / nSeg;

			glNormal3f(k * cos(step * i), r, k * sin(step * i));

			glTexCoord2f(se, 0);
			glVertex3f(r1 * cos(step * i), 0, r1 * sin(step * i));

			glTexCoord2f(se, 1.0f);
			glVertex3f(r2 * cos(step * i), h, r2 * sin(step * i));


		}



	}
	glEnd();


}

void CGLRenderer::DrawCone(float r, float h, int nSeg)
{

	float step = 2 * PI / nSeg;

	// donja osovina
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);

		for (int i = 0; i <= nSeg; i++)
		{
		
			glTexCoord2f(0.5 + 0.5 * cos(step * i), 0.5 + 0.5 * sin(step * i));
			glVertex3f(r * cos(step * i), 0, r * sin(step * i));
		}

	}
	glEnd();

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
			glNormal3f(k * cos(step * i), dr, k * sin(step * i));
			glTexCoord2f(0.5 + 0.5 * cos(step * i), 0.5 + 0.5 * sin(step * i));
			glVertex3f(r * cos(step * i), 0, r * sin(step * i));
		}
	}
	glEnd();


}
void inline InverseTSC(float x, float y, float& phi, float& theta)
{
	phi = atan(x);
	theta = atan(y * cos(phi));
}
void CGLRenderer::DrawPatch(float R, int nSeg)
{

	float delta = 2.0f / nSeg;

	float y = 1.0f;

	for (int i = 0; i < nSeg; i++)
	{
		float x = -1.0f;

		glBegin(GL_TRIANGLE_STRIP);
		{
			for (int j = 0; j <= nSeg; j++)
			{

				float phi, theta;
				InverseTSC(x, y, phi, theta);

				float xd, yd, zd;
				xd = cos(theta) * sin(phi);
				yd = sin(theta);
				zd = cos(theta) * cos(phi);

				glNormal3f(xd, yd, zd);
				glTexCoord2f((x + 1.0f) / 2.0f, (-y + 1.0f) / 2.0f);
				glVertex3f(R * xd, R * yd, R * zd);

				InverseTSC(x, y - delta, phi, theta);


				xd = cos(theta) * sin(phi);
				yd = sin(theta);
				zd = cos(theta) * cos(phi);

				glNormal3f(xd, yd, zd);
				glTexCoord2f((x + 1.0f) / 2.0f, (-y + delta + 1.0f) / 2.0f);
				glVertex3f(R * xd, R * yd, R * zd);

				x += delta;
			}
		}
		glEnd();
		y -= delta;


	}


}

void CGLRenderer::DrawEarth(float R, int nSeg)
{
	glPushMatrix();
	{

		glPushMatrix();
		for (int i = 0; i < 4; i++)
		{
			glRotatef(i * (90), 0, 1, 0);
			glBindTexture(GL_TEXTURE_2D, T[i]);
			DrawPatch(R, 20);
			glBindTexture(GL_TEXTURE_2D, 0);

		}
		glPopMatrix();

		glRotatef(-90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, T[4]);
		DrawPatch(R, 20);
		glBindTexture(GL_TEXTURE_2D, 0);

		glRotatef(180, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, T[5]);
		DrawPatch(R, 20);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();

}

void CGLRenderer::DrawMoon(float R, int nSeg)
{
	glPushMatrix();
	{

		glPushMatrix();
		for (int i = 0; i < 4; i++)
		{
			glRotatef(i * (90), 0, 1, 0);
			glBindTexture(GL_TEXTURE_2D, M[i]);
			DrawPatch(R, 20);
			glBindTexture(GL_TEXTURE_2D, 0);

		}
		glPopMatrix();

		glRotatef(-90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, M[4]);
		DrawPatch(R, 20);
		glBindTexture(GL_TEXTURE_2D, 0);

		glRotatef(180, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, M[5]);
		DrawPatch(R, 20);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpace(float R, int nSeg)
{
	glCullFace(GL_FRONT);
	glPushMatrix();
	{

		glPushMatrix();
		for (int i = 0; i < 4; i++)
		{
			glRotatef(i * (90), 0, 1, 0);
			glBindTexture(GL_TEXTURE_2D, S[i]);
			DrawPatch(R, 20);
			glBindTexture(GL_TEXTURE_2D, 0);

		}
		glPopMatrix();

		glRotatef(-90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, S[4]);
		DrawPatch(R, 20);
		glBindTexture(GL_TEXTURE_2D, 0);

		glRotatef(180, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, S[5]);
		DrawPatch(R, 20);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();
	glCullFace(GL_BACK);


}

void CGLRenderer::SetLight()
{

	float light_pos[] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	float l_amb[] = { 0.4, 0.4, 0.4, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb);

	float l_diff[] = { 0.8, 0.8, 0.8, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diff);

	float l_spec[] = { 1,1,1,1 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_spec);

}



CGLRenderer::CGLRenderer(void)
{
	float a[] = { 0.4, 0.4, 0.4, 1 };
	float d[] = { 0.8, 0.8, 0.8, 1 };
	float s[] = { 1, 1, 1, 1 };


	planetMat.SetAmb(a);
	planetMat.SetDiff(d);
	planetMat.SetSpec(s);
	


}

CGLRenderer::~CGLRenderer(void)
{

	// izbrisi ovde teksture glDeleteTexture()
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

	// ucitaj ovde texture

	char t[] = "TSCn.jpg";
	char m[] = "Mn.jpg";
	char s[] = "Sn.jpg";

	for (int i = 0; i < 6; i++)
	{
		t[3] = '0' + i;
		T[i] = LoadTexture(t);

		m[1] = '0' + i;
		M[i] = LoadTexture(m);

		s[1] = '0' + i;
		S[i] = LoadTexture(s);

	}


	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);



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


	glDisable(GL_LIGHTING);
	DrawSpace(200, 20);
	glEnable(GL_LIGHTING);

	if(this->drawAxis)
	{
		glDisable(GL_LIGHTING);
		DrawAxis();
		glEnable(GL_LIGHTING);
	}

	planetMat.MatFace();
	DrawEarth(30, 20);



	if (!light)
	{
		glDisable(GL_LIGHT0);
	}
	else {
		glEnable(GL_LIGHT0);
	}

	glPushMatrix();
	{
		glTranslatef(50, 50, 50);
		glRotatef(moonR, 0, 1, 0);
		DrawMoon(15, 20);
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
	gluPerspective(40, aspect, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	
	glDeleteTextures(6, M);
	glDeleteTextures(6, T);
	glDeleteTextures(6, S);

	
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
