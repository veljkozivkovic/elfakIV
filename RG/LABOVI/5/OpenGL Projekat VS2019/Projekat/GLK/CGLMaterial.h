#pragma once
#include "StdAfx.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

class CGLMaterial
{
public:
	CGLMaterial();
	~CGLMaterial();

	void SetFace(GLenum face = GL_FRONT);
	void SetAmbient(float* a);
	void SetDiffuse(float* d);
	void SetSpecular(float* s);
	void SetEmission(float* e);
	void SetShininess(float s);

protected:
	float m_ambient[4] = {};
	float m_diffuse[4] = {};
	float m_specular[4] = {};
	float m_emission[4] = {};
	float shininess = {};



};

