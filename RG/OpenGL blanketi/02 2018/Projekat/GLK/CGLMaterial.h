#pragma once
#include "StdAfx.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

class CGLMaterial
{
	float amb[4] = { 0.4,0.4,0.4,1 };
	float diff[4] = { 0.8,0.8,0.8,1 };
	float spec[4] = { 1,1,1,1 };
	float emm[4] = { 0,0,0,1 };
	float shin = 128;
public:
	void SetAmb(float* a);
	void SetDiff(float* a);
	void SetSpec(float* a);
	void SetEmm(float* a);
	void SetShin(float s);
	void MatFace(GLenum face = GL_FRONT_AND_BACK);

};

