#pragma once
#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

#include "Structs.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp\postprocess.h>

class AnimationCommon
{
private:
	template <typename tValue>
	struct Keyframe
	{
		float time; // キーフレームの時刻
		tValue value; // キーフレームの値
	};
	using KeyframeVector3 = Keyframe<Vector3>;
	using KeyframeQuaternion = Keyframe<Quaternion>;
	
	template <typename tValue>
	struct AnimationCurve
	{
		std::vector<Keyframe<tValue>> keyframes;
	};

	struct NodeAnimation
	{
		AnimationCurve<Vector3> translate;
		AnimationCurve<Quaternion> rotate;
		AnimationCurve<Vector3> scale;
	};

	struct Animation
	{
		float duration; // アニメーション全体の尺(秒)

		// NodeAnimationの集合体
		std::unordered_map<std::string, NodeAnimation> nodeAnimations;
	};

///------------------------------
/// メンバ関数
///------------------------------
public:

   // Animation読み込み
	Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);

private:



///------------------------------
/// メンバ変数
///------------------------------
public:

private:
	
};