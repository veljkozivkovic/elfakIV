#pragma once
#define PI 3.14159
#define toRad(x) ((x) * PI / 180.0f)
#include "DImage.h"

class CGLRenderer
{
public:





	float aXY = toRad(30);
	float aXZ = toRad(90);


	float eyeR = 90;

	float eyeX = 0;
	float eyeY = 0;
	float eyeZ = 0;

	float cx = 0;
	float cy = 5;
	float cz = 0;

	float ux = 0;
	float uy = 1;
	float uz = 0;

	void CalculatePosition();


	bool axes = true;
	void DrawAxes(float as = 10);


	UINT lamp, side, top, bot;
	UINT LoadTexture(const char* s);






	void DrawEnvCube(float a);
	void DrawSide(float h);

	float rot1 = 0;
	float rot2 = 0;
	float rotG = 0;





	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	
	
	void DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool bIsOpen);
	
	void DrawLampBase();
	void DrawLampArm();
	void DrawLampHead();

	void DrawLamp();

	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
