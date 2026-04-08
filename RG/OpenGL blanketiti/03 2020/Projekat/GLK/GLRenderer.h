#pragma once
#define PI 3.14159
#define toRad(x) (x) * PI / 180.0f
#define norm(x) (x) / 255.0f
#include "DImage.h"
#include "CGLMaterial.h"

class CGLRenderer
{
public:


	float aXY = 30;
	float aXZ = 90;

	float eyeR = 10;

	float eyeX = 0;
	float eyeY = 0;
	float eyeZ = 0;

	float cX = 0;
	float cY = 5;
	float cZ = 0;

	float upX = 0;
	float upY = 1;
	float upZ = 0;


	UINT spider, front, left, right, back, top, bot;

	UINT LoadTexture(const char* fn);
	void CalculatePosition();

	void DrawAxis(float aS = 50);
	void DrawSphere(float r, int nSegA, int nSegB);
	void DrawSphere(float r, int nSeg, float texU, float texV, float texR);
	void DrawCylinder(float r1, float r2, float h, int nSeg);
	void DrawCone(float r, float h, int nSeg);
	void DrawCone(float r, float h, int nSeg, float texU, float texV, float texR);

	void DrawLegSegment(double r, double h, int nSeg);
	void DrawLeg();
	void DrawSpiderBody();
	CGLMaterial mat;

	bool axis = true;
	bool light = true;
	void SetLight();


	CGLRenderer(void);
	virtual ~CGLRenderer(void);
	

	void DrawEnvCube(double a);


	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
