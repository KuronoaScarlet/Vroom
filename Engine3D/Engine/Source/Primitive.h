#pragma once

#include "MathGeoLib/src/MathGeoLib.h"
#include "Color.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

typedef unsigned int GLuint;
typedef unsigned short GLushort;
typedef float GLfloat;
typedef unsigned char GLubyte;

class Primitive
{
public:
	Primitive() : vao(0), index(0), vertex(0), showAxis(0) {}
	virtual ~Primitive() {}

	virtual void Draw() {}
	//inline GLuint GetIndexBuffer() { return indexBuffer; }
	void			SetPos(float x, float y, float z);
	void			Render() const;

	void DrawAxis();

	Color color;
protected:
	float4x4 transform;
	float3 transformG;
	float3 rotate;
	float3 scale;

	VertexBuffer* vertex;
	IndexBuffer* index;

	std::vector<float> shape;
	std::vector<unsigned int> indices;
	unsigned int my_id = 0;

	unsigned int vao;

	bool showAxis;
};

class PCube : public Primitive
{
public:
	PCube(float3 t, float3 r, float3 s);

	~PCube();

	void Draw() override;
	void CreateCheckerImage();

private:
	std::vector<GLfloat> indexVertex;
	std::vector<GLuint> indices;
	std::vector<float2> texCoords;

	GLubyte checkerImage[128][128][4];
	unsigned int texId;
	unsigned int tbo;
};

class PPlane : public Primitive
{
public:
	PPlane(float3 t, float3 r, float3 s);

	~PPlane();

	void Draw() override;

private:
};

class PCylinder : public Primitive
{
public:

	PCylinder(int sectCount, float h, float rad);
	~PCylinder();

	std::vector<float> GetUnitCircleVertices();

	void Draw() override;

	void BuildVerticesSmooth();

public:

	int sectorCount;
	float height;
	float radius;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	std::vector<GLuint> indices;
};

class PPyramid : public Primitive
{
public:
	PPyramid(float3 t, float3 r, float3 s);

	~PPyramid();

	void Draw() override;

private:
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

};

class PSphere : public Primitive
{
public:
	PSphere();
	PSphere(float radius, int rings, int sectors, vec pos);
public:
	float radius;
};

class PLine : public Primitive
{
public:
	PLine();
	PLine(float x, float y, float z, vec origin);
	void Render() const;
public:
	vec origin;
	vec destination;
};

class PGrid : public Primitive
{
public:
	PGrid(float w, float h);
	~PGrid();

	void Draw() override;

	void CreateGrid();

private:
	std::vector<float3> vertices;

	float width;
	float height;
};