#ifndef OBJECTS_H
#define OBJECTS_H

#include "GeometryGenerator.h"
#include "Vertex.h"
#include "Effects.h"
#include "Mechanic.h"

class Objects : public Mechanic
{
public:
	Objects(HINSTANCE hInstance);
	~Objects();

	void CreateBufferQuad(ID3D11Device* device);
	INT BronieInit(FLOAT jas_sw, FLOAT jas_diff, ID3D11Device* device);
	void Objekt(ID3D11DeviceContext* dc, FLOAT jas_sw, FLOAT jas_diff, XMMATRIX &mProj, XMMATRIX &mView, Vect S,  Vect T, ID3D11ShaderResourceView* textura, DirectionalLight DirLights[3], bool enable);

	ID3D11Buffer* vboQuad;
	ID3D11Buffer* indQuad;
	ID3D11ShaderResourceView* texture[5];
	UINT mGridIndexCount;
	UINT mGridIndexOffset;
	INT mGridVertexOffset;
	Material mQuadMat;

	XMFLOAT4X4 mQuadWorld;

	ID3D11ShaderResourceView* ogien;
	ID3D11ShaderResourceView* ziemia;
	ID3D11ShaderResourceView* GuzTwScText[11];
	ID3D11ShaderResourceView* linia_suwak;
	ID3D11ShaderResourceView* suwak;
	ID3D11ShaderResourceView* ZaznPoz;
	ID3D11ShaderResourceView* Celownik;
};

#endif