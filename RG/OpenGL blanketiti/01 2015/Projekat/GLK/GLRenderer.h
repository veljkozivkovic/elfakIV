#pragma once
#define PI 3.14159
#define toRad(x) (x * PI / 180.0f)
#define norm(x) (x) / 255.0f
#include "DImage.h"
#include "CGLMaterial.h"

class CGLRenderer
{
public:

	bool drawAxis = true;

	float aXY = 30;
	float aXZ = 90;

	float eyeR = 40;

	float eyeX = 0;
	float eyeY = 0;
	float eyeZ = 0;

	float cX = 0;
	float cY = 20;
	float cZ = 0;

	float upX = 0;
	float upY = 1;
	float upZ = 0;


	float axisSize = 50;
	void CalculatePosition();
	UINT LoadTexture(const char* fName);
	void DrawAxis(float aS = 50);

	void DrawSphere(float r, int nSegA, int nSegB);
	void DrawCylinder(float r1, float r2, float h, int nSeg);
	void DrawCone(float r, float h, int nSeg);


	CGLMaterial planetMat;

	void DrawPatch(float R, int nSeg);
	void DrawEarth(float R, int nSeg);
	void DrawMoon(float R, int nSeg);
	void DrawSpace(float R, int nSeg);

	bool light = true;
	float moonR = 0;
	void SetLight();

	UINT T[6];
	UINT M[6];
	UINT S[6];


	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
