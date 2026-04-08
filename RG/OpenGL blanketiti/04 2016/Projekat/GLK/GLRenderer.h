#pragma once
#define PI 3.14159
#define toRad(x) (x) * PI / 180.0f
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

	void CalculatePosition();

	bool light = true;
	void SetLight();



	bool axis = true;
	void DrawAxis(float aS = 10);


	UINT brick, env;
	UINT LoadTexture(const char* name);

	void DrawCube(float a);
	void DrawSide(float* gl, float* gd, float* dd, float* dl, float h);



	float rotV = 0;

	CGLMaterial mat;
	void DrawTube(double r1, double r2, double h, int n);
	void DrawCone(double r, double h, int n);
	void DrawPaddle(double length, double width);



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
