#ifndef SSAO_H
#define SSAO_H

#include "d3dUtil.h"
#include "ThirdPersonCamera.h"

class Ssao
{
public:
	Ssao(ID3D11Device* device, ID3D11DeviceContext* dc, INT width, INT height, FLOAT fovy, FLOAT farZ);
	~Ssao();

	ID3D11ShaderResourceView* NormalDepthSRV();
	ID3D11ShaderResourceView* AmbientSRV();
	
	void OnSize(INT width, INT height, FLOAT fovy, FLOAT farZ);
	void SetNormalDepthRenderTarget(ID3D11DepthStencilView* dsv);
	void ComputeSsao(ThirdPersonCamera &camera);

private:
	void BuildFrustumFarCorners(FLOAT fovy, FLOAT farZ);
	void BuildFullScreenQuad();
	void BuildTextureViews();
	void ReleaseTextureViews();
	void BuildRandomVectorTexture();
	void BuildOffsetVectors();

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* mDC;

	ID3D11Buffer* mScreenQuadVB;
	ID3D11Buffer* mScreenQuadIB;

	ID3D11ShaderResourceView* mRandomVectorSRV;

	ID3D11RenderTargetView* mNormalDepthRTV;
	ID3D11ShaderResourceView* mNormalDepthSRV;

	ID3D11RenderTargetView* mAmbientRTV0;
	ID3D11ShaderResourceView* mAmbientSRV0;

	ID3D11RenderTargetView* mAmbientRTV1;
	ID3D11ShaderResourceView* mAmbientSRV1;


	UINT mRenderTargetWidth;
	UINT mRenderTargetHeight;

	XMFLOAT4 mFrustumFarCorner[4];
	XMFLOAT4 mOffsets[14];
	D3D11_VIEWPORT mAmbientMapViewport;
};

#endif