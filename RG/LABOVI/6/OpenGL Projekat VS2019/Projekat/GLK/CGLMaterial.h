#pragma once
#include "StdAfx.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"


class CGLMaterial
{

public:

	void SetAmbient(float* a);
	void SetDiffuse(float* a);
	void SetSpecular(float* a);
	void SetEmission(float* a);
	void SetShinines(float s);

	void FaceMaterial(GLenum face = GL_FRONT_AND_BACK);



protected:

	float ambient[4] = {0,0,0,1};
	float diffuse[4] = { 0,0,0,1 };
	float specular[4] = { 0,0,0,1 };
	float shinines = 0;
	float emission[4] = { 0,0,0,1 };


};

