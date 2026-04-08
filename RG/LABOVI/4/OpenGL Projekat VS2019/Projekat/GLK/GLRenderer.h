#pragma once
#define PI 3.14159
#define toRad(x) (x * PI / 180.0f)
#define norm(x) ((float)x / 255.0f)
#define BRAON norm(120), norm(0), norm(0)
#define KAKI norm(255), norm(153), norm(0)
#define ZUT norm(255), norm(255), norm(0)

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		

	bool drawGrid = true;
	double rotAngle = 0;
	
	double gridSize = 100;
	int gridNum = 10;

	double angleXY = 30;
	double angleXZ = 90;

	double eyeR = 120;

	double eyeX = 0;
	double eyeY = 0;
	double eyeZ = 0;

	double centerX = 0;
	double centerY = 30;
	double centerZ = 0;

	double upX = 0;
	double upY = 1;
	double upZ = 0;



	void DrawPot(double height, double R, double r);
	void DrawFigure(double height, double R, double r);

	void DrawSphere(double r, int nSegAlpha, int nSegBetha);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);

	void DrawAxis();
	void DrawGrid();
	void CalculatePosition();
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
