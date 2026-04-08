#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

#define L0_AMBIENT       0,			0.5	,		0.5
#define L0_DIFFUSE		0.0f,		1.0f,		1.0f
#define L0_SPECULAR      0,			0.7f,		0.7f


#define L1_AMBIENT       0.5f,		norm(32),	0.5f
#define L1_DIFFUSE		1.0f,		norm(63),	1.0f
#define L1_SPECULAR     0.7,        norm(45),   0.7


#define L2_AMBIENT       0.5f,		0,		0
#define L2_DIFFUSE		1.0f,		0.0f,		0.0f
#define L2_SPECULAR    0.7f,0,0



#define GRAY_AMBIENT	norm(100),	norm(100),	norm(100)
#define GRAY_DIFFUSE	norm(200),	norm(200),	norm(200)
#define GRAY_SPECULAR   norm(50),	norm(50),	norm(50)

#define VASE0_AMBIENT norm(121), 0.0, norm(121)
#define VASE0_DIFFUSE 1,0,1
#define VASE0_SPECULAR 0.7,0,0.7

#define VASE1_AMBIENT 0, norm(108), norm(108)
#define VASE1_DIFFUSE 0,1,1
#define VASE1_SPECULAR 0,0.7, 0.7

inline void SetColor(float arr[4], float r, float g, float b, float a)
{
	arr[0] = r;
	arr[1] = g;
	arr[2] = b;
	arr[3] = a;
}

CGLRenderer::CGLRenderer(void)
{
	//wall
	SetColor(amb, 0.2, 0.2, 0.2, 1.0);
	SetColor(diff, 0.4,0.4,0.4, 1.0);
	SetColor(spec, 0.3,0.3,0.3, 1.0);
	wall.SetAmbient(amb);
	wall.SetDiffuse(diff);
	wall.SetSpecular(spec);
	wall.SetShininess(64);


	//table;
	SetColor(amb, GRAY_AMBIENT, 1.0);
	SetColor(diff, GRAY_DIFFUSE, 1.0);
	SetColor(spec, GRAY_SPECULAR, 1.0);
	table.SetAmbient(amb);
	table.SetDiffuse(diff);
	table.SetSpecular(spec);
	//table.SetShininess(64);


	//vase1;
	SetColor(amb, VASE1_AMBIENT, 1.0);
	SetColor(diff, VASE1_DIFFUSE, 1.0);
	SetColor(spec, VASE1_SPECULAR, 1.0);
	vase1.SetAmbient(amb);
	vase1.SetDiffuse(diff);
	vase1.SetSpecular(spec);
	//vase1.SetShininess(64);


	//vase2;
	SetColor(amb, VASE0_AMBIENT, 1.0);
	SetColor(diff, VASE0_DIFFUSE, 1.0);
	SetColor(spec, VASE0_SPECULAR, 1.0);
	vase2.SetAmbient(amb);
	vase2.SetDiffuse(diff);
	vase2.SetSpecular(spec);
	//vase2.SetShininess(64);


	//l0;
	SetColor(amb, L0_AMBIENT, 1.0);
	SetColor(diff, L0_DIFFUSE, 1.0);
	SetColor(spec, L0_SPECULAR, 1.0);
	SetColor(emm, L0_SPECULAR, 1.0);
	l0.SetAmbient(amb);
	l0.SetDiffuse(diff);
	//l0.SetSpecular(spec);
	l0.SetEmission(emm);


	//l1;
	SetColor(amb, L1_AMBIENT, 1.0);
	SetColor(diff, L1_DIFFUSE, 1.0);
	SetColor(spec, L1_SPECULAR, 1.0);
	SetColor(emm, L1_SPECULAR, 1.0);

	l1.SetAmbient(amb);
	l1.SetDiffuse(diff);
	//l1.SetShininess(64);
	l1.SetEmission(emm);


	//l2;
	SetColor(amb,  L2_AMBIENT, 1.0);
	SetColor(diff, L2_DIFFUSE, 1.0);
	SetColor(spec, L2_SPECULAR, 1.0);
	SetColor(emm, L2_SPECULAR, 1.0);

	l2.SetAmbient(amb);
	l2.SetDiffuse(diff);
	//l2.SetShininess(64);
	l2.SetEmission(emm);

}

CGLRenderer::~CGLRenderer(void)
{
	
}

void CGLRenderer::SetLights()
{
	float h = gridSize / 2.5;
	if (light0)
	{
		SetColor(amb, L0_AMBIENT, 1.0);
		SetColor(diff, L0_DIFFUSE, 1.0);
		SetColor(spec, L0_SPECULAR, 1.0);
		DrawLight(-h, h, 0, l0);
		SetLightPositionDirection(GL_LIGHT0, -h, h, 0, 1, 0, 0);
		glEnable(GL_LIGHT0);

	}
	if (light1)
	{

		SetColor(amb, L1_AMBIENT, 1.0);
		SetColor(diff, L1_DIFFUSE, 1.0);
		SetColor(spec, L1_SPECULAR, 1.0);
		DrawLight(h, h, 0, l1);
		SetLightPositionDirection(GL_LIGHT1, h, h, 0, -1, 0, 0);
		glEnable(GL_LIGHT1);

	}
	if (light2)
	{

		SetColor(amb, L2_AMBIENT, 1.0);
		SetColor(diff, L2_DIFFUSE, 1.0);
		SetColor(spec, L2_SPECULAR, 1.0);
		DrawLight(0, h * 2, 0, l2);
		SetLightPositionDirection(GL_LIGHT2, 0, 2 * h, 0, 0, -1, 0);
		glEnable(GL_LIGHT2);


	}
}
void CGLRenderer::SetLightPositionDirection(GLenum light, float posX, float posY, float posZ, float dirX, float dirY, float dirZ)
{
	glLightfv(light, GL_AMBIENT, amb);
	glLightfv(light, GL_DIFFUSE, diff);
	glLightfv(light, GL_SPECULAR, spec);

	float light_position[] = {posX, posY, posZ, 0.0}; // treba 0.0 na kraj jer su svi usmereni

	glLightfv(light, GL_POSITION, light_position);

	float light_direction[] = {dirX, dirY, dirZ};
	glLightfv(light, GL_SPOT_DIRECTION, light_direction);


}
void CGLRenderer::DrawLight(float x, float y, float z, CGLMaterial mat)
{

	glPushMatrix();
	glTranslatef(x, y, z);
	if(!x && !z)
	{
		glRotatef(180, 0, 0, 1);
	}
	else if (x < y)
	{
		glRotatef(-90, 0, 0, 1);

	}
	else {
		
			glRotatef(90, 0, 0, 1);
	
	}
	mat.SetFace(GL_FRONT_AND_BACK);

	DrawHalfSphere(5, 30, 30);


	glPopMatrix();


}



void CGLRenderer::CalculatePosition()
{
	eyeX = eyeR * cos(toRad(angleXY)) * cos(toRad(angleXZ));
	eyeY = eyeR * sin(toRad(angleXY));
	eyeZ = eyeR * cos(toRad(angleXY)) * sin(toRad(angleXZ));

	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

void CGLRenderer::DrawAxis()
{
	glBegin(GL_LINES);
	{
		glColor3f(1.0, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(axisSize,0,0);

		glColor3f(0, 1.0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, axisSize, 0);

		glColor3f(0, 0, 1.0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, axisSize);


	}
	glEnd();
}

void CGLRenderer::DrawGrid()
{
	float half = this->gridSize / 2.0f;
	glColor3f(1, 1, 1);

	glBegin(GL_LINES);
	{
		for (int i = 0; i <= gridNum; i++)
		{
			float pos = i * gridUnit - half;
			glVertex3f(-half, 0, pos);
			glVertex3f(half, 0, pos);


			glVertex3f(pos, 0, -half);
			glVertex3f(pos, 0, half);

		}
	}
	glEnd();



}

void CGLRenderer::DrawSphere(float r, int nSegA, int nSegB)
{
	float sA = PI * 2 / nSegA;
	float sB = PI * 2 / nSegB;

	
	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i <= nSegA; i++)
		{
			for (int j = 0; j <= nSegB; j++)
			{

				float x = cos(sA * i) * cos(sB * j);
				float y = sin(sA * i);
				float z = cos(sA * i) * sin(sB * j);


				float x1 = cos(sA * (i + 1)) * cos(sB * j);
				float y1 = sin(sA * (i + 1));
				float z1 = cos(sA * (i + 1)) * sin(sB * j);

				glNormal3f(x, y, z);
				glVertex3f(r * x, r * y, r * z);

				glNormal3f(x1, y1, z1);
				glVertex3f(r * x1, r * y1, r * z1);




			}

		}
	}
	glEnd();

}

void CGLRenderer::DrawHalfSphere(float r, int nSegA, int nSegB)
{
	float sA = PI  / nSegA; // SAMO JE OVDE BEZ 2 *
	float sB = 2 * PI / nSegB; 
	glEnable(GL_CULL_FACE);

	glBegin(GL_QUAD_STRIP);
	{
		for (int i = 0; i < nSegA; i++) // NE UKLJUCUJE nSegA
		{
			for (int j = 0; j <= nSegB; j++)
			{

				float x = cos(sA * i) * cos(sB * j);
				float y = sin(sA * i);
				float z = cos(sA * i) * sin(sB * j);


				float x1 = cos(sA * (i + 1)) * cos(sB * j);
				float y1 = sin(sA * (i + 1));
				float z1 = cos(sA * (i + 1)) * sin(sB * j);

				glNormal3f(x / r, y / r, z / r);// NORMALE DRUGACIJE OD OBICNE SFERE
				glVertex3f(r * x, r * y, r * z);

				glNormal3f(x1 / r , y1 / r, z1 / r); // NORMALE DRUGACIJE OD OBICNE SFERE
				glVertex3f(r * x1, r * y1, r * z1);

				


			}

		}
	}
	glEnd();

	if(drawNorms)
	{
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		{
			for (int i = 0; i < nSegA; i++) // NE UKLJUCUJE nSegA
			{
				for (int j = 0; j <= nSegB; j++)
				{

					float x = cos(sA * i) * cos(sB * j);
					float y = sin(sA * i);
					float z = cos(sA * i) * sin(sB * j);

					glVertex3f(r * x, r * y, r * z);
					glVertex3f(r * x + (x / r) * norm_len, r * y + (y / r) * norm_len, r * z + (z / r) * norm_len);// NORMALE DRUGACIJE OD OBICNE SFERE

				}

			}
		}
		glEnd();
	}
	
	glDisable(GL_CULL_FACE);
}

void CGLRenderer::DrawCone(float r, float h, int nSeg)
{
	
	float s = PI * 2 / nSeg;
	
	// donja osovina
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
		
			glVertex3f(r * cos(s * i), 0, r * sin(s * i));


		}
	}
	glEnd();


	// omotac
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f(0, h, 0);
		for (int i = 0; i <= nSeg; i++)
		{

			glNormal3f(cos(s * i), r / h, sin(s * i));
			glVertex3f(r * cos(s * i), 0, r * sin(s * i));

		}
	}
	glEnd();


}

void CGLRenderer::DrawCylinder(float r1, float r2, float h, int nSeg)
{
	float s = 2 * PI / nSeg;

	// donja osovina 
	glBegin(GL_TRIANGLE_FAN);
	{
		glNormal3f(0, -1, 0);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glVertex3f(r1 * cos(s * i), 0, r1 * sin(s * i));
		}
	}
	glEnd();


	// gornja osovina
	glBegin(GL_TRIANGLE_FAN);
	{

		glNormal3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			glVertex3f(r2 * cos(s * i), h, r2 * sin(s * i));
		}
	}
	glEnd();


	float dr = r1 - r2;
	float len = sqrt(dr * dr + h * h);
	float k = h / len;
	float r = dr / len;


	// omotach
	glBegin(GL_QUAD_STRIP);
	{

		for (int i = 0; i <= nSeg; i++)
		{
			glNormal3f(k * cos(s * i), r, k * sin(s * i));
			glVertex3f(r1 * cos(s * i), 0, r1 * sin(s * i));
			glVertex3f(r2 * cos(s * i), h, r2 * sin(s * i));

		}

	}
	glEnd();

	glColor3f(1, 0, 0);
	if (drawNorms) {
		glBegin(GL_LINES);
		{
			glVertex3f(0, -1 * norm_len, 0);
			glVertex3f(0, 0, 0);
			
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex3f(0, h + norm_len, 0);
			glVertex3f(0, h, 0);

		}
		glEnd();
		glBegin(GL_LINES);
		{
			for (int i = 0; i <= nSeg; i++)
			{
				glVertex3f(r1 * cos(s * i) + (k * cos(s * i)) * norm_len, r * norm_len, r1 * sin(s * i) + (k * sin(s * i)) * norm_len);
				glVertex3f(r1 * cos(s * i), 0, r1 * sin(s * i));

				glVertex3f(r2 * cos(s * i), h, r2 * sin(s * i));
				glVertex3f(r2 * cos(s * i) + (k * cos(s * i)) * norm_len, h + r * norm_len , r2 * sin(s * i) + (k * sin(s * i)) * norm_len);

			}
		}
		glEnd();
	}

	


}

void CGLRenderer::DrawTableTop()
{
	float height = 4;
	float width = 45;

	float h = height / 2;
	float w = width / 2;

	table.SetFace(GL_FRONT_AND_BACK);

	glBegin(GL_QUAD_STRIP);
	{
		// Z = 1
		glNormal3f(0, 0, 1);
		glVertex3f(-w, -h, w);
		glVertex3f(-w, h, w);


		glNormal3f(0, 0, 1);
		glVertex3f(w,-h,w);
		glVertex3f(w, h, w);

		// X = 1
		glNormal3f(1, 0, 0);
		glVertex3f(w, -h, -w);
		glVertex3f(w, h, -w);

		// Z = -1
		glNormal3f(0, 0, -1);
		glVertex3f(-w, -h, -w);
		glVertex3f(-w, h, -w);

		// X = -1
		glNormal3f(-1, 0, 0);
		glVertex3f(-w, -h, w);
		glVertex3f(-w, h, w);

	}
	glEnd();

	glBegin(GL_QUADS); {

		glNormal3f(0, 1, 0);
		glVertex3f(-w, h, w);
		glVertex3f(w, h, w);
		glVertex3f(w, h, -w);
		glVertex3f(-w, h, -w);



	}
	glEnd();

	glBegin(GL_QUADS); {

		glNormal3f(0, -1, 0);
		glVertex3f(-w, -h, w);
		glVertex3f(w, -h, w);
		glVertex3f(w, -h, -w);
		glVertex3f(-w, -h, -w);
	}
	glEnd();

	if (drawNorms) {
		glColor3f(1, 0, 0);

		glBegin(GL_LINES);
		{

			// prednja strana:
			glVertex3f(0, 0, w);
			glVertex3f(0, 0, w + norm_len);

			// desna
			glVertex3f(w, 0, 0);
			glVertex3f(w + norm_len, 0, 0);

			// zadnja strana:
			glVertex3f(0, 0, -w);
			glVertex3f(0, 0, -w - norm_len);

			//leva
			glVertex3f(-w, 0, 0);
			glVertex3f(-w - norm_len, 0, 0);

			//gore
			glVertex3f(0, h, 0);
			glVertex3f(0, h + norm_len, 0);

			//dole
			glVertex3f(0, -h, 0);
			glVertex3f(0, -h - norm_len, 0);

			
		}
		glEnd();


	}


}

void CGLRenderer::DrawTable()
{
	

	table.SetFace(GL_FRONT_AND_BACK);
	DrawHalfSphere(8, 60, 60);
	glTranslatef(0, 8, 0);
	table.SetFace(GL_FRONT_AND_BACK);

	DrawCylinder(3, 3, 8, 60);
	glTranslatef(0, 8, 0);
	glTranslatef(0, 2, 0);
	table.SetFace(GL_FRONT_AND_BACK);

	DrawTableTop();
	glTranslatef(0, 2, 0);

	
}

void CGLRenderer::DrawVase(float r1, float r2, float h, int nSeg)
{
	float s = 2 * PI / nSeg;
	float dr = r1 - r2;
	float len = sqrt(dr * dr + h * h);
	float k = h / len;
	float r = dr / len;


	// omotach
	glBegin(GL_QUAD_STRIP);
	{

		for (int i = 0; i <= nSeg; i++)
		{
			glNormal3f(k * cos(s * i), r, k * sin(s * i));
			glVertex3f(r1 * cos(s * i), 0, r1 * sin(s * i));
			glVertex3f(r2 * cos(s * i), h, r2 * sin(s * i));

		}

	}
	glEnd();

	glColor3f(1, 0, 0);
	if (drawNorms) {
		glBegin(GL_LINES);
		{
			glVertex3f(0, -1 * norm_len, 0);
			glVertex3f(0, 0, 0);

		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex3f(0, h + norm_len, 0);
			glVertex3f(0, h, 0);

		}
		glEnd();
		glBegin(GL_LINES);
		{
			for (int i = 0; i <= nSeg; i++)
			{
				glVertex3f(r1 * cos(s * i) + (k * cos(s * i)) * norm_len, r * norm_len, r1 * sin(s * i) + (k * sin(s * i)) * norm_len);
				glVertex3f(r1 * cos(s * i), 0, r1 * sin(s * i));

				glVertex3f(r2 * cos(s * i), h, r2 * sin(s * i));
				glVertex3f(r2 * cos(s * i) + (k * cos(s * i)) * norm_len, h + r * norm_len, r2 * sin(s * i) + (k * sin(s * i)) * norm_len);

			}
		}
		glEnd();
	}
}

void CGLRenderer::DrawVases()
{
	

	vase1.SetFace(GL_FRONT_AND_BACK);
	DrawVase(15, 10, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase2.SetFace(GL_FRONT_AND_BACK);
	DrawVase(10, 5, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase1.SetFace(GL_FRONT_AND_BACK);
	DrawVase(5, 5, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase2.SetFace(GL_FRONT_AND_BACK);
	DrawVase(5, 5, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase1.SetFace(GL_FRONT_AND_BACK);
	DrawVase(5, 10, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase2.SetFace(GL_FRONT_AND_BACK);
	DrawVase(10, 5, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase1.SetFace(GL_FRONT_AND_BACK);
	DrawVase(5, 10, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase2.SetFace(GL_FRONT_AND_BACK);
	DrawVase(10, 15, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase1.SetFace(GL_FRONT_AND_BACK);
	DrawVase(15, 10, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase2.SetFace(GL_FRONT_AND_BACK);
	DrawVase(10, 5, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase1.SetFace(GL_FRONT_AND_BACK);
	DrawVase(5, 10, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase2.SetFace(GL_FRONT_AND_BACK);
	DrawVase(10, 5, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase1.SetFace(GL_FRONT_AND_BACK);
	DrawVase(5, 10, 2.5, 60);
	glTranslatef(0, 2.5, 0);
	vase2.SetFace(GL_FRONT_AND_BACK);
	DrawVase(10, 15, 2.5, 60);

	
}

void CGLRenderer::DrawRoom()
{
	glPushMatrix();
	float h = gridSize/ 2.5;
	glTranslatef(0, h, 0);

	wall.SetFace(GL_FRONT);

	glBegin(GL_QUADS);
	{
		// FRONT (Z = 1)
		glNormal3f(0, 0, -1);
		glVertex3f(-h, -h, h);
		glVertex3f(h, -h, h);
		glVertex3f(h, h, h);
		glVertex3f(-h, h, h);

		// BACK (Z = -1) - Gledano od pozadi
		glNormal3f(0, 0, 1);
		glVertex3f(h, -h, -h);
		glVertex3f(-h, -h, -h);
		glVertex3f(-h, h, -h);
		glVertex3f(h, h, -h);

		// LEFT (X = -1)
		glNormal3f(1, 0, 0);
		glVertex3f(-h, -h, -h);
		glVertex3f(-h, -h, h);
		glVertex3f(-h, h, h);
		glVertex3f(-h, h, -h);

		// RIGHT (X = 1)
		glNormal3f(-1, 0, 0);
		glVertex3f(h, -h, h);
		glVertex3f(h, -h, -h);
		glVertex3f(h, h, -h);
		glVertex3f(h, h, h);

		// TOP (Y = 1)
		glNormal3f(0, -1, 0);
		glVertex3f(-h, h, h);
		glVertex3f(h, h, h);
		glVertex3f(h, h, -h);
		glVertex3f(-h, h, -h);

		// BOTTOM (Y = -1) - Orijentisano odozdo CCW
		glNormal3f(0, 1, 0);
		glVertex3f(-h, -h, -h);
		glVertex3f(h, -h, -h);
		glVertex3f(h, -h, h);
		glVertex3f(-h, -h, h);
	}
	glEnd();



	glBegin(GL_QUADS);
	{
		glNormal3f(0, 1, 0);
		glVertex3f(-h, -h, h);
		glVertex3f(h, -h, h);
		glVertex3f(h, -h, -h);
		glVertex3f(-h, -h, -h);
	}
	glEnd();

	glPopMatrix();
	
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
	
	glClearColor(1,1, 1, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	float model_ambient[] = { 0.7, 0.7, 0.7, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);


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

	if (this->drawAxis) {
		glDisable(GL_LIGHTING);

		this->DrawAxis();
		this->DrawGrid();
		glEnable(GL_LIGHTING);
	}
	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	SetLights();

	
	

	glPushMatrix();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	DrawRoom();
	glDisable(GL_CULL_FACE);

	DrawTable();
	DrawVases();
	




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
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
