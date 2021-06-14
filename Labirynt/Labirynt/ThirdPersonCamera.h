#ifndef THIRDPERSONCAMERA_H
#define THIRDPERSONCAMERA_H

#include "d3dUtil.h"
#include <dinput.h>

class ThirdPersonCamera
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();

	void MoveChar(double dt, XMVECTOR& destinationDirection, XMMATRIX& worldMatrix, FLOAT CameraSpeed);
	void Update(FLOAT DystansKamery);

	void ControllMouse(DIMOUSESTATE CurrentMouse, DIMOUSESTATE LastMouse);

	void WalkForward(FLOAT dt);
	void WalkBack(FLOAT dt);
	void WalkLeft(FLOAT dt);
	void WalkRight(FLOAT dt);
	void LookUp();
	void LookDown();
	void LookLeft();
	void LookRight();

	XMMATRIX CamView;
	XMMATRIX CamProjection;

	XMVECTOR CamPosition;
	XMVECTOR CamTarget;
	XMVECTOR CamUp;
	XMVECTOR DefaultForward;
	XMVECTOR DefaultRight;
	XMVECTOR Forward;
	XMVECTOR Right;

	XMMATRIX RotationMatrix;
	XMMATRIX playerCharWorld;

	FLOAT moveLeftRight;
	FLOAT moveBackForward;

	FLOAT Yaw;
	FLOAT Pitch;

	XMVECTOR currCharDirection;
	XMVECTOR oldCharDirection;
	XMVECTOR charPosition;

	FLOAT charCamDist = 15.0f;
	FLOAT charDirAngle;

	FLOAT SpeedCamera = 15.0f;
	FLOAT AnglePerson = 3.49f;
};

#endif