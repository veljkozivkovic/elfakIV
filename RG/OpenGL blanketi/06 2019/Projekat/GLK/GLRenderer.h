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
	float eyeY= 0;
	float eyeZ = 0;

	float cX = 0;
	float cY = 5;
	float cZ = 0;

	float upX = 0;
	float upY = 1;
	float upZ = 0;


	void CalculatePosition();


	bool axis = true;
	void DrawAxis(float as = 10);

	bool light = true;
	void SetLight();

	//   png excavator
	UINT excavator, front, left, right, back, top, bot;
	UINT LoadTexture(const char* name);
		



	float rKab = 0;
	float r1 = 0;
	float r2 = 0;
	float rVilj = 0;




	


	void DrawEnvCube(float a);
	void DrawSide(float h);




	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);
	void DrawFork();
	void DrawArm(double zh);
	void DrawBase();
	void DrawBody();
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	
	
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
