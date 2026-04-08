#include "stdafx.h"
#include "CGLMaterial.h"

void CGLMaterial::SetAmb(float* a)
{
	for (int i = 0; i < 4; i++)
	{
		this->amb[i] = a[i];
	}
}

void CGLMaterial::SetDiff(float* a)
{
	for (int i = 0; i < 4; i++)
	{
		this->diff[i] = a[i];
	}
}

void CGLMaterial::SetSpec(float* a)
{
	for (int i = 0; i < 4; i++)
	{
		this->spec[i] = a[i];
	}
}

void CGLMaterial::SetEmm(float* a)
{
	for (int i = 0; i < 4; i++)
	{
		this->emm[i] = a[i];
	}
}

void CGLMaterial::SetShin(float s)
{
	this->shin = s;
}

void CGLMaterial::MatFace(GLenum face)
{

	glMaterialfv(face, GL_AMBIENT, amb);
	glMaterialfv(face, GL_DIFFUSE, diff);
	glMaterialfv(face, GL_SPECULAR, spec);
	glMaterialfv(face, GL_EMISSION, emm);
	glMaterialf(face, GL_SHININESS, shin);


}
