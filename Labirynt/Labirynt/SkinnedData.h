#ifndef SKINNEDDATA_H
#define SKINNEDDATA_H

#include "d3dUtil.h"
#include <map>

struct Keyframe
{
	Keyframe();
	~Keyframe();

	FLOAT TimePos;
	XMFLOAT3 Translation;
	XMFLOAT3 Scale;
	XMFLOAT4 RotationQuat;
};

struct BoneAnimation
{
	FLOAT GetStartTime()const;
	FLOAT GetEndTime()const;

	void Interpolate(FLOAT t, XMFLOAT4X4& M)const;

	std::vector<Keyframe> Keyframes;
};

struct AnimationClip
{
	FLOAT GetClipStartTime()const;
	FLOAT GetClipEndTime()const;

	void Interpolate(FLOAT t, std::vector<XMFLOAT4X4>& boneTransforms)const;

	std::vector<BoneAnimation> BoneAnimations;
};

class SkinnedData
{
public:
	UINT BoneCount()const;

	FLOAT GetClipStartTime(const std::string& clipName)const;
	FLOAT GetClipEndTime(const std::string& clipName)const;

	void Set(std::vector<INT>& boneHierarchy, std::vector<XMFLOAT4X4>& boneOffsets, std::map<std::string, AnimationClip>& animations);
	void GetFinalTransforms(const std::string& clipName, FLOAT timePos, std::vector<XMFLOAT4X4>& finalTransforms)const;

private:
	std::vector<INT> mBoneHierarchy;
	std::vector<XMFLOAT4X4> mBoneOffsets;
	std::map<std::string, AnimationClip> mAnimations;
};

#endif