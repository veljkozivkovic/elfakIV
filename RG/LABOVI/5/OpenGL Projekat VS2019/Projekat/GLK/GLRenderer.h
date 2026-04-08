#pragma once
#include "CGLMaterial.h"
#define PI 3.14159
#define toRad(x) (x * PI / 180.f)
#define norm(x) (x/255.0f)



class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
	

	bool light0 = true;
	bool light1 = true;
	bool light2 = true;

	float norm_len = 2.0f;


	bool drawNorms = true;

	CGLMaterial wall ;
	CGLMaterial table;
	CGLMaterial vase1;
	CGLMaterial vase2;
	CGLMaterial l0   ;
	CGLMaterial l1   ;
	CGLMaterial l2   ;

	float amb  [4] = {};// inicijalizuje sa nule sve memset(амб, 0, 4 * sizeof(float));
	float diff [4] = {};// inicijalizuje sa nule sve memset(дифф, 0, 4 * sizeof(float));
	float spec [4] = {};// inicijalizuje sa nule sve memset(спец, 0, 4 * sizeof(float));
	float emm  [4] = {};// inicijalizuje sa nule sve memset(емм, 0, 4 * sizeof(float));
	float shin  = 128.0f;

	bool drawAxis = true;

	float angleXY = 30;
	float angleXZ = 90;

	float eyeR = 100;
	float eyeX = 0;
	float eyeY = 0;
	float eyeZ = 0;

	float centerX = 0;
	float centerY = 30;
	float centerZ = 0;

	float upX = 0;
	float upY = 1;
	float upZ = 0;



	float gridSize = 135;
	float gridNum = 10;
	float gridUnit = gridSize / gridNum;
	float axisSize = gridSize / 2 + 10.0f;

	void SetLights();
	void DrawLight(float x, float y, float z, CGLMaterial mat); // kad zoves ovo ispitaj bool da li treba se crta
	void SetLightPositionDirection(GLenum light, float posX, float posY, float posZ, float dirX, float dirY, float dirZ);


	void CalculatePosition();
	void DrawAxis();
	void DrawGrid();

	void DrawSphere(float r, int nSegA, int nSegB);
	void DrawHalfSphere(float r, int nSegA, int nSegB);
	void DrawCone(float r, float h, int nSeg);
	void DrawCylinder(float r1, float r2, float h, int nSeg);

	void DrawTableTop();
	void DrawTable();

	void DrawVase(float r1, float r2, float h, int nSeg);
	void DrawVases();


	void DrawRoom();


	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
