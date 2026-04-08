#pragma once
#define PI 3.14159
#define toRad(x) (x * PI / 180)
#define norm(x) ((x)/255.0f)
#include "CGLMaterial.h"
#include <afxwin.h>


class CGLRenderer
{
public:

	float aXY = 30;
	float aXZ = 90;
	
	
	float eyeR = 30;

	float eyeX = 0;
	float eyeY = 0;
	float eyeZ = 0;

	float centerX = 0;
	float centerY = 10;
	float centerZ = 0;

	float upX = 0;
	float upY = 1;
	float upZ = 0;


	bool drawAxis = true;
	float axisSize = 60;


	float groundSize = 100;



	UINT truck;
	UINT grass;

	float amb[4] = {};
	float diff[4] = {};
	float spec[4] = {};
	float emis[4] = {};
	float shin = {};
	CGLMaterial truckMat, grassMat;



	UINT LoadTexture(const char* ime);
	void DrawAxis();
	void CalculatePosition();



	void DrawCylinder(float r1, float r2, float h, int nSeg, bool flipNorm);
	void DrawCone(float r, float h, int nSeg);
	void DrawSphere(float r, int nSegA, int nSegB);

	bool lightOn = true;

	void SetLight();
	void DrawGround();

	float truckA = 0;
	void DrawTruck();
	void DrawSide(bool front);
	void ConnectSides();
	void DrawElipsoid();







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
