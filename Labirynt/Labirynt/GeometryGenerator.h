#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include "d3dUtil.h"

class GeometryGenerator
{
public:
	struct Vertex
	{
		Vertex(){}
		Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv) : Position(p), Normal(n), TangentU(t), TexC(uv){}
		Vertex(FLOAT px, FLOAT py, FLOAT pz, FLOAT nx, FLOAT ny, FLOAT nz, FLOAT tx, FLOAT ty, FLOAT tz, FLOAT u, FLOAT v) : Position(px,py,pz), Normal(nx,ny,nz), TangentU(tx, ty, tz), TexC(u,v){}

		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT3 TangentU;
		XMFLOAT2 TexC;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
	};

	void CreateBox(FLOAT width, FLOAT height, FLOAT depth, MeshData& meshData);
	void CreateSphere(FLOAT radius, UINT sliceCount, UINT stackCount, MeshData& meshData);
	void CreateGrid(FLOAT width, FLOAT depth, UINT m, UINT n, MeshData& meshData);
};

#endif