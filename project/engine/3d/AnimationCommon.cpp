#include "AnimationCommon.h"

AnimationCommon::Animation AnimationCommon::LoadAnimationFile(const std::string& directoryPath, const std::string& filename)
{
    Animation animation; // 今回作るアニメーション
    Assimp::Importer importer;
    std::string filePath = directoryPath + "/" + filename;
    const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
    assert(scene->mNumAnimations != 0); // アニメーションがない
    aiAnimation* animationAssimp = scene->mAnimations[0]; //　最初のアニメーションだけ採用
    animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond); // 時間の単位を秒に変換

    // assimpでは個々のNodeのAnimationをchannelと呼んでいるのでchannelを回してNodeAnimationの情報をとってくる
    for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) 
    {
        aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
        NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
        for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex)
        {
            //----------------------
            // translate
            aiVectorKey& posKeyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
            KeyframeVector3 posKeyframe;
            posKeyframe.time = float(posKeyAssimp.mTime / animationAssimp->mTicksPerSecond); // 時間の単位を変換(秒)
            posKeyframe.value = { -posKeyAssimp.mValue.x,posKeyAssimp.mValue.y,posKeyAssimp.mValue.z }; // 右手->左手
            nodeAnimation.translate.keyframes.push_back(posKeyframe);

            //----------------------
            // rotate
            aiQuatKey& rotateKeyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
            KeyframeQuaternion rotateKeyframe;
            rotateKeyframe.time = float(rotateKeyAssimp.mTime / animationAssimp->mTicksPerSecond); // 時間の単位を変換(秒)
            rotateKeyframe.value = { rotateKeyAssimp.mValue.x, -rotateKeyAssimp.mValue.y ,-rotateKeyAssimp.mValue.z }; // 右手->左手
            nodeAnimation.rotate.keyframes.push_back(rotateKeyframe);

            //----------------------
            // scale
            aiVectorKey& scaleKeyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
            KeyframeVector3 scaleKeyframe;
            scaleKeyframe.time = float(scaleKeyAssimp.mTime / animationAssimp->mTicksPerSecond); // 時間の単位を変換(秒)
            scaleKeyframe.value = { scaleKeyAssimp.mValue.x,scaleKeyAssimp.mValue.y ,scaleKeyAssimp.mValue.z }; // 変わらず
            nodeAnimation.scale.keyframes.push_back(scaleKeyframe);
        }
    }
   
    // 解析完了
    return animation;
}
