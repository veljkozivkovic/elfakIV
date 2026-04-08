#include "stdafx.h"
#include "CGLMaterial.h"

CGLMaterial::CGLMaterial()
{
}

CGLMaterial::~CGLMaterial()
{
}

void CGLMaterial::SetFace(GLenum face)
{
	glMaterialfv(face, GL_AMBIENT, m_ambient);
	glMaterialfv(face, GL_DIFFUSE, m_diffuse);
	glMaterialfv(face, GL_SPECULAR, m_specular);
	glMaterialfv(face, GL_EMISSION, m_emission );
	glMaterialf(face, GL_SHININESS, shininess);


}

void CGLMaterial::SetAmbient(float* a)
{
	for (int i = 0; i < 4; i++)
	{
		m_ambient[i] = a[i];
	}
}

void CGLMaterial::SetDiffuse(float* d)
{
	for (int i = 0; i < 4; i++)
	{
		m_diffuse[i] = d[i];
	}
}

void CGLMaterial::SetSpecular(float* s)
{
	for (int i = 0; i < 4; i++)
	{
		m_specular[i] = s[i];
	}
}

void CGLMaterial::SetEmission(float* e)
{
	for (int i = 0; i < 4; i++)
	{
		m_emission[i] = e[i];
	}
}

void CGLMaterial::SetShininess(float s)
{
	shininess = s;
}
