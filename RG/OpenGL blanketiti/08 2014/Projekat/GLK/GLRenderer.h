#pragma once
#define PI 3.14159
#define toRad(x) (x) * PI / 180.0f
#include "DImage.h"
enum CubeSide {
	FRONT = 0,
	RIGHT = 1,
	BACK = 2,
	LEFT = 3,
	TOP = 4,
	BOT = 5,
	NONE = 6
};

class CGLRenderer
{
public:

	float aXY = 45;
	float aXZ = 45;

	float er = 30;

	float ex = 0;
	float ey = 0;
	float ez = 0;

	float cx = 0;
	float cy = 0;
	float cz = 0;

	float ux = 0;
	float uy = 1;
	float uz = 0;

	void CalculatePosition();
	void DrawAxis(float a = 30);

	UINT tex;
	UINT LoadTexture(const char* s);


	int mCount = 3;
	void DrawCube(float a);
	void DrawSide(float h, bool texture, float* gl);

	void DrawRubikCube(float a, int count);

	void SetWhiteLight();

	void SetMaterial(float r, float g, float b);


	float rot1 = 0;
	float rot2 = 0;
	float rot3 = 0;

	float* GL = new float[2];
	float* GL2 = new float[2];
	float* GL3 = new float[2];

	CubeSide trenutni = FRONT;
	CubeSide trenutni2 = LEFT;
	CubeSide trenutni3 = NONE;

	bool light = true;
	bool light1 = true;



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
