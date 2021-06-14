#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera()
{
	CamPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	CamTarget = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	CamUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	Forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	Yaw = 0.0f;
	Pitch = 0.0f;

	currCharDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	oldCharDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	charPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::MoveChar(double dt, XMVECTOR& destinationDirection, XMMATRIX& worldMatrix, FLOAT CameraSpeed)
{
	destinationDirection = XMVector3Normalize(destinationDirection);

	if (XMVectorGetX(XMVector3Dot(destinationDirection, oldCharDirection)) == -1)
		oldCharDirection += XMVectorSet(0.02f, 0.0f, -0.02f, 0.0f);

	charPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	charPosition = XMVector3TransformCoord(charPosition, worldMatrix);

	FLOAT destDirLength = 10.0f * dt;

	currCharDirection = (oldCharDirection)+(destinationDirection * destDirLength);
	currCharDirection = XMVector3Normalize(currCharDirection);
	charDirAngle = XMVectorGetX(XMVector3AngleBetweenNormals(XMVector3Normalize(currCharDirection), XMVector3Normalize(DefaultForward)));
	if (XMVectorGetY(XMVector3Cross(currCharDirection, DefaultForward)) > 0.0f)
		charDirAngle = -charDirAngle;

	FLOAT speed = CameraSpeed * dt;
	charPosition = charPosition + (destinationDirection * speed);

	XMMATRIX Scale = XMMatrixScaling(0.25f, 0.25f, 0.25f);
	XMMATRIX Translate = XMMatrixTranslation(XMVectorGetX(charPosition), 0.0f, XMVectorGetZ(charPosition));
	XMMATRIX RotY = XMMatrixRotationY((charDirAngle - XM_PI) / AnglePerson);

	worldMatrix = Scale * RotY * Translate;

	oldCharDirection = currCharDirection;
}

void ThirdPersonCamera::Update(FLOAT DystansKamery)
{
	CamTarget = charPosition;
	CamTarget = XMVectorSetY(CamTarget, XMVectorGetY(CamTarget) + 5.0f);
	RotationMatrix = XMMatrixRotationRollPitchYaw(-Pitch, Yaw, 0);
	CamPosition = XMVector3TransformNormal(DefaultForward, RotationMatrix);
	CamPosition = XMVector3Normalize(CamPosition);
	CamPosition = (CamPosition * DystansKamery) + CamTarget;
	Forward = XMVector3Normalize(CamTarget - CamPosition);
	Forward = XMVectorSetY(Forward, 0.0f);
	Forward = XMVector3Normalize(Forward);
	Right = XMVectorSet(-XMVectorGetZ(Forward), 0.0f, XMVectorGetX(Forward), 0.0f);
	CamUp = XMVector3Normalize(XMVector3Cross(XMVector3Normalize(CamPosition - CamTarget), Right));
	CamView = XMMatrixLookAtLH(CamPosition, CamTarget, CamUp);
}

void ThirdPersonCamera::ControllMouse(DIMOUSESTATE CurrentMouse, DIMOUSESTATE LastMouse)
{
	if ((CurrentMouse.lX != LastMouse.lX) || (CurrentMouse.lY != LastMouse.lY))
	{
		Yaw += CurrentMouse.lX * 0.002f;
		if (Yaw < 0.0f)
			Yaw = 6.27f;
		if (Yaw > 6.27f)
			Yaw = 0.0f;

		Pitch += CurrentMouse.lY * 0.002f;
		if (Pitch > 0.3f)
			Pitch = 0.3f;
		if (Pitch < -0.3f)
			Pitch = -0.3f;

		LastMouse = CurrentMouse;
	}
}

void ThirdPersonCamera::WalkForward(FLOAT dt)
{
	XMVECTOR desiredCharDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	desiredCharDir += (Forward);
	MoveChar(dt, desiredCharDir, playerCharWorld, SpeedCamera);
}

void ThirdPersonCamera::WalkBack(FLOAT dt)
{
	XMVECTOR desiredCharDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	desiredCharDir += -(Forward);
	MoveChar(dt, desiredCharDir, playerCharWorld, SpeedCamera);
}

void ThirdPersonCamera::WalkLeft(FLOAT dt)
{
	XMVECTOR desiredCharDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	desiredCharDir += (Right);
	MoveChar(dt, desiredCharDir, playerCharWorld, SpeedCamera);
}

void ThirdPersonCamera::WalkRight(FLOAT dt)
{
	XMVECTOR desiredCharDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	desiredCharDir += -(Right);
	MoveChar(dt, desiredCharDir, playerCharWorld, SpeedCamera);
}

void ThirdPersonCamera::LookUp()
{
	Pitch -= 0.01f;
	if (Pitch < -0.3f)
		Pitch = -0.3f;
}

void ThirdPersonCamera::LookDown()
{
	Pitch += 0.01f;
	if (Pitch > 0.5f)
		Pitch = 0.5f;
}

void ThirdPersonCamera::LookLeft()
{
	Yaw -= 0.01f;
	if (Yaw < 0.0f)
		Yaw = 6.27f;
}

void ThirdPersonCamera::LookRight()
{
	Yaw += 0.01f;
	if (Yaw > 6.27f)
		Yaw = 0.0f;
}
