#include "stdafx.h"
#include "CGLMaterial.h"

void CGLMaterial::SetAmbient(float* a)
{
	for (int i = 0; i < 4; i++)
	{
		ambient[i] = a[i];
	}
}

void CGLMaterial::SetDiffuse(float* a)
{
	for (int i = 0; i < 4; i++)
	{
		diffuse[i] = a[i];
	}
}

void CGLMaterial::SetSpecular(float* a)
{
	for (int i = 0; i < 4; i++)
	{
		specular[i] = a[i];
	}
}

void CGLMaterial::SetEmission(float* a)
{
	for (int i = 0; i < 4; i++)
	{
		emission[i] = a[i];
	}
}

void CGLMaterial::SetShinines(float s)
{
	shinines = s;
}

void CGLMaterial::FaceMaterial(GLenum face)
{
	glMaterialfv(face, GL_AMBIENT, ambient);
	glMaterialfv(face, GL_DIFFUSE, diffuse);
	glMaterialfv(face, GL_SPECULAR, specular);
	glMaterialfv(face, GL_EMISSION, emission);
	glMaterialf(face, GL_SHININESS, shinines);
}