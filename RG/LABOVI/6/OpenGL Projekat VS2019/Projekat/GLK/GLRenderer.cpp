#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"

//#pragma comment(lib, "GL\\glut32.lib")

UINT CGLRenderer::LoadTexture(const char* ime)
{

	UINT id;
	DImage img;
	img.Load(CString(ime));
	img.Flip();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (img.GetDIBBits() == NULL) {
		AfxMessageBox(CString("Nije ucitana slika"));
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_RGBA, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return id;
}

void CGLRenderer::DrawAxis()
{
	glBegin(GL_LINES);
	{
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(axisSize, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, axisSize, 0);

		glColor3f(0,0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, axisSize);

	}
	glEnd();
}

void CGLRenderer::CalculatePosition()
{
	eyeX = eyeR * cos(toRad(aXY)) * cos(toRad(aXZ));
	eyeY = eyeR * sin(toRad(aXY));
	eyeZ = eyeR * cos(toRad(aXY)) * sin(toRad(aXZ));
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

}

void CGLRenderer::DrawCylinder(float r1, float r2, float h, int nSeg, bool flipNorm)
{
	
	float s = 2 * PI / nSeg;

	// za sliku
	float unitS = 1.0f / 16.0f;

	// za 3D prostor
	float unitG = 2.0f;
	float sizeG = 32.0f;
	float halfG = sizeG / 2;
	float widthG = 10.0f;
	float halfW = widthG / 2;

	// donji omotac SAMO KOD NJEGA TREBA SE NALEPI TEKSTURA
	glBegin(GL_TRIANGLE_FAN);
	{
		if (flipNorm)
		{
			glNormal3f(0, 1, 0);
		}
		else
			glNormal3f(0, -1, 0);

		glTexCoord2f(6 * unitS, 14.5 * unitS); // centar tocka
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{

			glTexCoord2f(6 * unitS + 1.5 * unitS * cos(s * i), 14.5 * unitS + 1.5 * unitS * sin(s * i));
			glVertex3f(r1 * cos(s * i), 0, r1 * sin(s * i));

		}
	}
	glEnd();


	// gornji omotac
	glBegin(GL_TRIANGLE_FAN);
	{

		glNormal3f(0, 1, 0);
		//glTexCoord2f(0.5, 0.5);
		glVertex3f(0,h ,0);

		for (int i = 0; i <= nSeg; i++)
		{
			//glTexCoord2d(0.5 + 0.5 * cos(s * i), 0.5 + 0.5 * sin(s * i));
			glVertex3f(r2 * cos(s * i), h, r2 * sin(s * i));
		}


	}
	glEnd();


	float dr = r1 - r2;
	float len = sqrt(h * h + dr * dr);
	float k = h / len;
	float r = dr / len;


	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i <= nSeg; i++)
		{

			glNormal3f(k * cos(s * i), r, k * sin(s * i));


			float se = (float)i / nSeg;

			//glTexCoord2d(se, 0);
			glVertex3f(r1 * cos(s * i), 0, r1 * sin(s * i));

			//glTexCoord2d(se, 1);
			glVertex3f(r2 * cos(s * i), h, r2 * sin(s * i));




		}
	}
	glEnd();


	

}

void CGLRenderer::DrawCone(float r, float h, int nSeg)
{

	float s = 2 * PI / nSeg;
	//glBindTexture(GL_TEXTURE_2D, grass);

	// donja osovina
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);

		for (int i = 0; i <= nSeg; i++)
		{
			glTexCoord2f(0.5 + 0.5 * cos(s * i), 0.5 + 0.5 * sin(s * i));
			glVertex3f(r * cos(s * i), 0, r * sin(s * i));

		}

	}
	glEnd();


	// omotac


	float len = sqrt(h * h + r * r);
	float k = h / len;
	float dr = r / len;

	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, 1, 0); // Vrh kupe
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, h, 0);

		// OVO JE KLJUCNA PROMENA: Idemo unazad da bi Winding Order bio CCW
		//for (int i = nSeg; i >= 0; i--)
		for (int i = 0; i <= nSeg; i++)
		{
			glNormal3f(k * cos(s * i), dr, k * sin(s * i));
			glTexCoord2f(0.5 + 0.5 * cos(s * i), 0.5 + 0.5 * sin(s * i));
			glVertex3f(r * cos(s * i), 0, r * sin(s * i));
		}

	}
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, 0);

}

void CGLRenderer::DrawSphere(float r, int nSegA, int nSegB)
{
	float stepA = PI / nSegA;      // visina (samo π)
	float stepB = 2 * PI / nSegB;  // širina

	for (int i = 0; i < nSegA; i++)
	{
		float a1 = -PI / 2 + i * stepA;
		float a2 = -PI / 2 + (i + 1) * stepA;

		glBegin(GL_QUAD_STRIP);
		{
			for (int j = 0; j <= nSegB; j++)
			{
				float b = j * stepB;

				float s = (float)j / nSegB;        // cela širina
				float t1 = (float)i / nSegA * 0.5f;   // pola teksture
				float t2 = (float)(i + 1) / nSegA * 0.5f;

				// prvi red
				float x = cos(a1) * cos(b);
				float y = sin(a1);
				float z = cos(a1) * sin(b);

				glNormal3f(x, y, z);
				glTexCoord2f(s, t1);
				glVertex3f(r * x, r * y, r * z);

				// drugi red
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

void CGLRenderer::SetLight()
{
	
	glEnable(GL_LIGHT0);

	float lambient[] = { 0.3, 0.3, 0.3, 1 };
	float ldiff[] = { 0.9, 0.9, 0.9, 1 };
	float lspec[] = { 0.9, 0.9, 0.9, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lspec);



	float light_pos[] = {axisSize / 2, axisSize / 2, axisSize / 2, 1.0f};
	float light_dir[] = { -1,-1,-1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f);


}

//void CGLRenderer::DrawGround()
//{
//	float h = groundSize / 2;
//
//	grassMat.FaceMaterial();
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, grass);
//	glBegin(GL_QUADS);
//	{
//
//		glNormal3f(0, 1, 0);
//
//		glTexCoord2f(0, 0);
//		glVertex3f(-h, 0, h);
//		
//
//		glTexCoord2f(5, 0);
//		glVertex3f(h, 0, h);
//
//
//		glTexCoord2f(5, 5);
//		glVertex3f(h, 0, -h);
//		
//
//		glTexCoord2f(0,5);
//		glVertex3f(-h, 0, -h);
//
//	}
//	glEnd();
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//
//}
void CGLRenderer::DrawGround()
{
	float h = groundSize / 2;
	int n = 5; 
	float step = groundSize / n;
	glBindTexture(GL_TEXTURE_2D, grass);

	grassMat.FaceMaterial();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			float x = -h + i * step;
			float z = -h + j * step;



			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x, 0, z + step);

			
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x + step, 0, z + step);

			
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x + step, 0, z);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x, 0, z);
		}
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

}
void CGLRenderer::DrawTruck()
{
	glPushMatrix();

	glRotatef(truckA, 0, 1, 0);
	glTranslatef(0, 2, 0);
	truckMat.FaceMaterial(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, truck);

	glPushMatrix();
	{
		DrawSide(true);
		glScalef(1, 1, -1);

		glBindTexture(GL_TEXTURE_2D, truck);
		DrawSide(false);
	}
	glPopMatrix();

	ConnectSides();


	glTranslatef(1.5 * 2.0f, 6.5 * 2.0f, 0);
	DrawElipsoid();

	glTranslatef(0,5,0);
	DrawCone(5, 10, 60);
	//DrawCylinder(7, 5, 10, 60, false);

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

void CGLRenderer::DrawSide(bool front)
{
	// za sliku
	float unitS = 1.0f / 16.0f;
	
	// za 3D prostor
	float unitG = 2.0f;
	float sizeG = 32.0f;
	float halfG = sizeG / 2;
	float widthG = 10.0f;
	float halfW = widthG / 2;

	glNormal3f(0, 0, 1);
	if (!front)
	{
		
		glNormal3f(0, 0, -1);
	}


	float wheelR = 0.0f; // TODO

	glPushMatrix();

	



	glBegin(GL_QUADS); 
	{
		
		// 2 :
		glTexCoord2f(0 * unitS, 11 * unitS);
		glVertex3f(0 * unitG - halfG, 3 * unitG, halfW);

		glTexCoord2f(4 * unitS, 11 * unitS);
		glVertex3f(4 * unitG - halfG, 3 * unitG, halfW);

		glTexCoord2f(4 * unitS, 14 * unitS);
		glVertex3f(4 * unitG - halfG, 6 * unitG, halfW);

		glTexCoord2f(0 * unitS, 14 * unitS);
		glVertex3f(0 * unitG - halfG, 6 * unitG, halfW);


		// 4: 
		glTexCoord2f(4 * unitS, 9 * unitS);
		glVertex3f(4 * unitG - halfG, 1 * unitG, halfW);

		glTexCoord2f(8 * unitS, 9 * unitS);
		glVertex3f(8 * unitG - halfG, 1 * unitG, halfW);


		glTexCoord2f(8 * unitS, 12 * unitS);
		glVertex3f(8 * unitG - halfG, 4 * unitG, halfW);


		glTexCoord2f(4 * unitS, 12 * unitS);
		glVertex3f(4 * unitG - halfG, 4 * unitG, halfW);

		// 8 :

		glTexCoord2f(8 * unitS, 11 * unitS);
		glVertex3f(8 * unitG - halfG, 3 * unitG, halfW);

		glTexCoord2f(11 * unitS, 11 * unitS);
		glVertex3f(11 * unitG - halfG, 3 * unitG, halfW);

		glTexCoord2f(11 * unitS, 12 * unitS);
		glVertex3f(11 * unitG - halfG, 4 * unitG, halfW);

		glTexCoord2f(8 * unitS, 12 * unitS);
		glVertex3f(8 * unitG - halfG, 4 * unitG, halfW);

		// 9 :

		glTexCoord2f(11 * unitS, 9 * unitS);
		glVertex3f(11 * unitG - halfG, 1 * unitG, halfW);

		glTexCoord2f(15 * unitS, 9 * unitS);
		glVertex3f(15 * unitG - halfG, 1 * unitG, halfW);


		glTexCoord2f(15 * unitS, 12 * unitS);
		glVertex3f(15 * unitG - halfG, 4 * unitG, halfW);

		glTexCoord2f(11 * unitS, 12 * unitS);
		glVertex3f(11 * unitG - halfG, 4 * unitG, halfW);



	}
	glEnd();

	glBegin(GL_POLYGON);
	{
		// 1:
		glTexCoord2f(0 * unitS, 9 * unitS);  // x * unitS | x * unitG - halfG y * unitS | (y-8) * unitG
		glVertex3f(0 * unitG - halfG, 1 * unitG, halfW);

		glTexCoord2f(1 * unitS, 9 * unitS);
		glVertex3f(1 * unitG - halfG,1 * unitG, halfW);


		glTexCoord2f(1 * unitS, 10 * unitS);
		glVertex3f(1 * unitG - halfG, 2 * unitG, halfW);


		glTexCoord2f(2 * unitS, 11 * unitS);
		glVertex3f(2 * unitG - halfG, 3 * unitG, halfW);


		glTexCoord2f(0 * unitS, 11 * unitS);
		glVertex3f(0 * unitG - halfG, 3 * unitG, halfW);
	}
	glEnd();

	glBegin(GL_POLYGON);
	{
		//3 :

		glTexCoord2f(0 * unitS, 14 * unitS);
		glVertex3f(0 * unitG - halfG, 6 * unitG, halfW);

		glTexCoord2f(4 * unitS, 14 * unitS);
		glVertex3f(4 * unitG - halfG, 6 * unitG, halfW);

		glTexCoord2f(4 * unitS, 16 * unitS);
		glVertex3f(4 * unitG - halfG, 8 * unitG, halfW);

		glTexCoord2f(2 * unitS, 16 * unitS);
		glVertex3f(2 * unitG - halfG, 8 * unitG, halfW);

		// da l se treba zatvori GL_POLYGON?
	}
	glEnd();


	glBegin(GL_POLYGON);
	{
		// 5 (MALI KURAC KOJI FALI KOD 4)
		glTexCoord2f(3 * unitS, 11 * unitS);
		glVertex3f(3 * unitG - halfG, 3 * unitG, halfW);

		glTexCoord2f(4 * unitS, 10 * unitS);
		glVertex3f(4 * unitG - halfG, 2 * unitG, halfW);

		glTexCoord2f(4 * unitS, 11 * unitS);
		glVertex3f(4 * unitG - halfG, 3 * unitG, halfW);
	}
	glEnd();

	glBegin(GL_POLYGON);
	{
		// 6 : mali trouglici kurcici JEBEM IM MAMU
		glTexCoord2f(8 * unitS, 10 * unitS);
		glVertex3f(8 * unitG - halfG, 2 * unitG, halfW);


		glTexCoord2f(9 * unitS, 11 * unitS);
		glVertex3f(9 * unitG - halfG, 3 * unitG, halfW);

		
		glTexCoord2f(8 * unitS, 11 * unitS);
		glVertex3f(8 * unitG - halfG, 3 * unitG, halfW);

	}
	glEnd();

	glBegin(GL_POLYGON);
	{
		// 7 JOS JEDAN MALI U PM
		glTexCoord2f(10 * unitS, 11 * unitS);
		glVertex3f(10 * unitG - halfG, 3 * unitG, halfW);

		glTexCoord2f(11 * unitS, 10 * unitS);
		glVertex3f(11 * unitG - halfG, 2 * unitG, halfW);

		glTexCoord2f(11 * unitS, 11 * unitS);
		glVertex3f(11 * unitG - halfG, 3 * unitG, halfW);

	}
	glEnd();



	// TOCHKOVI
	

	glPushMatrix();

	glTranslatef(2.5 * unitG - halfG, 1.3 * unitG, halfW);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-20, 0, 1, 0);
	truckMat.FaceMaterial();
	glBindTexture(GL_TEXTURE_2D, truck);
	DrawCylinder(1.4 * unitG, 1.4 * unitG, widthG / 6, 10, !front);// 16 nseg

	glPopMatrix();

	glTranslatef(9.5 * unitG - halfG, 1.3 * unitG, halfW);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-20, 0, 1, 0);
	truckMat.FaceMaterial();
	glBindTexture(GL_TEXTURE_2D, truck);
	DrawCylinder(1.4 * unitG, 1.4 * unitG, widthG / 6, 10, !front);


	glPopMatrix();
}

void CGLRenderer::ConnectSides()
{
	float unitG = 2.0f;
	float sizeG = 32.0f;
	float halfG = sizeG / 2;
	float widthG = 10.0f;
	float halfW = widthG / 2;
	glBegin(GL_QUAD_STRIP);
	{

		glNormal3f(0, 1, 0);


		glVertex3f(0 - halfG, unitG, halfW);
		glVertex3f(0 - halfG, unitG, -halfW);


		glVertex3f(unitG - halfG, unitG, halfW);
		glVertex3f(unitG - halfG, unitG, -halfW);


		glVertex3f(unitG - halfG, 2 * unitG, halfW);
		glVertex3f(unitG - halfG, 2 * unitG, -halfW);


		glVertex3f(2 * unitG - halfG, 3 * unitG, halfW);
		glVertex3f(2 * unitG - halfG, 3 * unitG, -halfW);


		glVertex3f(3 * unitG - halfG, 3 * unitG, halfW);
		glVertex3f(3 * unitG - halfG, 3 * unitG, -halfW);


		glVertex3f(4 * unitG - halfG, 2 * unitG, halfW);
		glVertex3f(4 * unitG - halfG, 2 * unitG, -halfW);


		glVertex3f(4 * unitG - halfG, 1 * unitG, halfW);
		glVertex3f(4 * unitG - halfG, 1 * unitG, -halfW);


		glVertex3f(8 * unitG - halfG, 1 * unitG, halfW);
		glVertex3f(8 * unitG - halfG, 1 * unitG, -halfW);


		glVertex3f(8 * unitG - halfG, 2 * unitG, halfW);
		glVertex3f(8 * unitG - halfG, 2 * unitG, -halfW);


		glVertex3f(9 * unitG - halfG, 3 * unitG, halfW);
		glVertex3f(9 * unitG - halfG, 3 * unitG, -halfW);


		glVertex3f(10 * unitG - halfG, 3 * unitG, halfW);
		glVertex3f(10 * unitG - halfG, 3 * unitG, -halfW);


		glVertex3f(11 * unitG - halfG, 2 * unitG, halfW);
		glVertex3f(11 * unitG - halfG, 2 * unitG, -halfW);


		glVertex3f(11 * unitG - halfG, 1 * unitG, halfW);
		glVertex3f(11 * unitG - halfG, 1 * unitG, -halfW);


		glVertex3f(15 * unitG - halfG, 1 * unitG, halfW);
		glVertex3f(15 * unitG - halfG, 1 * unitG, -halfW);


		glVertex3f(15 * unitG - halfG, 4 * unitG, halfW);
		glVertex3f(15 * unitG - halfG, 4 * unitG, -halfW);


		glVertex3f(4 * unitG - halfG, 4 * unitG, halfW);
		glVertex3f(4 * unitG - halfG, 4 * unitG, -halfW);


		glVertex3f(4 * unitG - halfG, 8 * unitG, halfW);
		glVertex3f(4 * unitG - halfG, 8 * unitG, -halfW);


		glVertex3f(2 * unitG - halfG, 8 * unitG, halfW);
		glVertex3f(2 * unitG - halfG, 8 * unitG, -halfW);


		glVertex3f(0 * unitG - halfG, 6 * unitG, halfW);
		glVertex3f(0 * unitG - halfG, 6 * unitG, -halfW);


		glVertex3f(0 - halfG, unitG, halfW);
		glVertex3f(0 - halfG, unitG, -halfW);


	}
	glEnd();
}

void CGLRenderer::DrawElipsoid()
{
	glPushMatrix();
	{
		truckMat.FaceMaterial();
		glBindTexture(GL_TEXTURE_2D, truck);
		glScalef(2, 1, 1);
		DrawSphere(5, 30, 30);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();

}

CGLRenderer::CGLRenderer(void)
{



	amb[0] = 0.3; amb[1] = 0.3; amb[2] = 0.3;
	diff[0] = 0.7; diff[1] = 0.7; diff[2] = 0.7;
	spec[0] = 0.9; spec[1] = 0.9; spec[2] = 0.9;
	shin = 128;




	truckMat.SetAmbient(amb);
	truckMat.SetDiffuse(diff);
	truckMat.SetSpecular(spec);
	truckMat.SetEmission(emis);
	truckMat.SetShinines(shin);


	amb[0] = 0   ; amb[1] = 0.2   ; amb[2] =  0;
	diff[0] = 0; diff[1] = 0.5; diff[2] = 0;
	spec[0] = 0; spec[1] = 0.8; spec[2] = 0;
	shin = 32;

	grassMat.SetAmbient(amb);
	grassMat.SetDiffuse(diff);
	grassMat.SetSpecular(spec);
	grassMat.SetEmission(emis);
	grassMat.SetShinines(shin);
}

CGLRenderer::~CGLRenderer(void)
{
	glDeleteTextures(1, &truck);
	glDeleteTextures(1, &grass);
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
	glClearColor(0.7, 0.7, 0.7, 1);

	truck = LoadTexture("truck.png");
	
	grass = LoadTexture("grass.jpg");
	
	GLfloat lmodel_ambient[] = { 0.9, 0.9, 0.9, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	CalculatePosition();

	if (this->lightOn)
		SetLight();
	else
		glDisable(GL_LIGHT0);


	if (this->drawAxis)
	{
		glDisable(GL_LIGHTING);
		DrawAxis();
		glEnable(GL_LIGHTING);
	}
	DrawGround();

	DrawTruck();


	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	double aspect = (double)w / h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
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
