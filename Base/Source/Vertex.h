#ifndef VERTEX_H
#define VERTEX_H
#include "Vector3.h"

/***************************************************************************************************************
Vertex info class

Author: Tan Yie Cher
Date: 27/12/2016

Note:
-Order is important
/***************************************************************************************************************/
struct Color
{
	float r, g, b;
	Color(float r = 255.f, float g = 255.f, float b = 255.f)
	{
		Set(r, g, b);
	}
	void Set(float r, float g, float b) { this->r = r / 255.f; this->g = g / 255.f; this->b = b / 255.f; }

	Color& operator= (Color& copyMe){
		r = copyMe.r; g = copyMe.g; b = copyMe.b;
		return *this;
	}
};

struct Color4
{
	float r, g, b, a;
	Color4(float r = 255, float g = 255, float b = 255, float a = 1) { Set(r, g, b, a); }
	void Set(float r, float g, float b, float a) { this->r = r / 255.f; this->g = g / 255.f; this->b = b; this->a = a; }
};


struct TexCoord
{
	float u, v;
	TexCoord(float u = 0, float v = 0) { Set(u, v); }
	void Set(float u, float v) { this->u = u; this->v = v; }
};


class Vertex
{
public:
	Vector3 position;
	Color color;
	TexCoord tex;

	Vertex(){}
	~Vertex(){}
};

#endif