#include "Objects.h"

Objects::Objects(HINSTANCE hInstance) : Mechanic(hInstance)
{
}

Objects::~Objects()
{
	ReleaseCOM(vboQuad);
	ReleaseCOM(indQuad);

	for (INT i = 0; i < 5; i++)
		ReleaseCOM(texture[i]);

	for (INT i = 0; i < 11; i++)
		ReleaseCOM(GuzTwScText[i]);

	ReleaseCOM(ogien);
	ReleaseCOM(ziemia);
	ReleaseCOM(linia_suwak);
	ReleaseCOM(suwak);
	ReleaseCOM(ZaznPoz);
}

void Objects::CreateBufferQuad(ID3D11Device* device)
{
	InitStructures(device);

	GeometryGenerator::MeshData grid;
	GeometryGenerator geoGen;

	geoGen.CreateGrid(1.0f, 1.0f, 2.0f, 2.0f, grid);
	mGridVertexOffset = 0;

	mGridIndexCount = grid.Indices.size();
	mGridIndexOffset = 0;

	UINT totalVertexCount = grid.Vertices.size();
	UINT totalIndexCount = mGridIndexCount;

	std::vector<Vertex::Basic32> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[k].Tex = grid.Vertices[i].TexC;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(device->CreateBuffer(&vbd, &vinitData, &vboQuad));

	std::vector<UINT> indices;
	indices.insert(indices.end(), grid.Indices.begin(), grid.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(device->CreateBuffer(&ibd, &iinitData, &indQuad));
}

INT Objects::BronieInit(FLOAT jas_sw, FLOAT jas_diff, ID3D11Device* device)
{
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/punch.png", 0, 0, &texture[0], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/luk.png", 0, 0, &texture[1], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/miecz.png", 0, 0, &texture[2], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/mieczyk.png", 0, 0, &texture[3], 0));

	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/ogien.png", 0, 0, &ogien, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/ziemia.png", 0, 0, &ziemia, 0));

	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/sciana.png", 0, 0, &GuzTwScText[0], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/luk.png", 0, 0, &GuzTwScText[1], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/miecz.png", 0, 0, &GuzTwScText[2], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/strzala.png", 0, 0, &GuzTwScText[3], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/mieczyk.png", 0, 0, &GuzTwScText[4], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/boss1.png", 0, 0, &GuzTwScText[5], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/boss2.png", 0, 0, &GuzTwScText[6], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/robot1.png", 0, 0, &GuzTwScText[7], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/robot2.png", 0, 0, &GuzTwScText[8], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/krzyzyk_lab3.png", 0, 0, &GuzTwScText[9], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/guziki/moneta.png", 0, 0, &GuzTwScText[10], 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/linia_suwak.png", 0, 0, &linia_suwak, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/suwak.png", 0, 0, &suwak, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/ZaznPoz.png", 0, 0, &ZaznPoz, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(device, L"../Release/Textures/celownik.png", 0, 0, &Celownik, 0));

	return true;
}

void Objects::Objekt(ID3D11DeviceContext* dc, FLOAT jas_sw, FLOAT jas_diff, XMMATRIX &mProj, XMMATRIX &mView, Vect S, Vect T, ID3D11ShaderResourceView* textura, DirectionalLight DirLights[3], bool enable)
{
	mQuadMat.Ambient = XMFLOAT4(jas_sw, jas_sw, jas_sw, jas_sw);
	mQuadMat.Diffuse = XMFLOAT4(jas_diff, jas_diff, jas_diff, jas_diff);
	mQuadMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	mQuadMat.Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	Effects::BasicFX->SetDirLights(DirLights);

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexTech;
	D3DX11_TECHNIQUE_DESC techDesc;

	dc->IASetVertexBuffers(0, 1, &vboQuad, &stride, &offset);
	dc->IASetIndexBuffer(indQuad, DXGI_FORMAT_R32_UINT, 0);

	dc->IASetInputLayout(InputLayouts::Basic32);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (enable)
		dc->OMSetBlendState(mAlphaEBlendState, blendFactor, 0xff00ff);
	else
		dc->OMSetBlendState(mAlphaDBlendState, blendFactor, 0xff00ff);

	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		XMMATRIX Scale = XMMatrixScaling(S.x1, S.y1, S.z1);
		XMMATRIX Translate = XMMatrixTranslation(T.x1, T.y1, T.z1);
		XMMATRIX RotateX = XMMatrixRotationX(-1.57f);

		XMMATRIX WVP = XMMatrixMultiply(Scale * Translate * RotateX, mView * mProj);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(Scale * Translate * RotateX);

		Effects::BasicFX->SetWorld(Scale * Translate * RotateX);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(WVP);
		Effects::BasicFX->SetTexTransform(XMMatrixIdentity());
		Effects::BasicFX->SetMaterial(mQuadMat);
		Effects::BasicFX->SetDiffuseMap(textura);
		activeTexTech->GetPassByIndex(p)->Apply(0, dc);
		dc->DrawIndexed(mGridIndexCount, mGridIndexOffset, mGridVertexOffset);
	}
}