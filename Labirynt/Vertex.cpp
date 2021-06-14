#include "Vertex.h"
#include "Effects.h"

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Pos[1] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Basic32[3] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormalTexTan[4] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosNormalTexTanSkinned[6] =
{
	{ "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "WEIGHTS",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,   0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

ID3D11InputLayout* InputLayouts::Pos = 0;
ID3D11InputLayout* InputLayouts::Basic32 = 0;
ID3D11InputLayout* InputLayouts::PosNormalTexTan = 0;
ID3D11InputLayout* InputLayouts::PosNormalTexTanSkinned = 0;

void InputLayouts::InitAll(ID3D11Device* device)
{
	D3DX11_PASS_DESC passDesc;

	// Pos
	Effects::SkyFX->SkyTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::Pos, 1, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &Pos));

	// Basic32
	Effects::BasicFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::Basic32, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &Basic32));

	// NormalMap
	Effects::NormalMapFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::PosNormalTexTan, 4, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &PosNormalTexTan));

	// NormalMapSkinned
	Effects::NormalMapFX->Light1SkinnedTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::PosNormalTexTanSkinned, 6, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &PosNormalTexTanSkinned));
}

void InputLayouts::DestroyAll()
{
	ReleaseCOM(Pos);
	ReleaseCOM(Basic32);
	ReleaseCOM(PosNormalTexTan);
	ReleaseCOM(PosNormalTexTanSkinned);
}