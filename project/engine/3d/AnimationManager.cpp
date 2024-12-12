#include "AnimationManager.h"

void AnimationManager::LoadAnimationFile(const std::string& directoryPath, const std::string& filename, Model::ModelData modelData)
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
   
    animation.modelData = modelData;

    // 解析完了
    animations[filename] = animation;
}

void AnimationManager::StartAnimation(const std::string& filename, int loopCount)
{
    auto it = animations.find(filename);

    // 見つからなかった場合止める
    assert(it == animations.end() && "file key not found");

    //　アニメーションをコピー
    Animation animation = it->second;

    // アニメーションのステータスを設定
    AnimationState animationState;
    animationState.animation = animation;
    animationState.currentLoopCount = 0;

    // 引数が0より小さい場合ループ数を無限に設定
    if (loopCount <= 0)
    {
        animationState.isInfiniteLoop = true;
    }

    // それ以外の場合ループ数をそのまま代入
    else
    {
        animationState.maxLoopCount = loopCount;
        animationState.isInfiniteLoop = false;
    }

    // アニメーションの時間を0で初期化
    animationState.animationTime = 0.0f;

    activeAnimations[filename] = animationState;
}

void AnimationManager::PlayAnimation()
{
    for (auto it = activeAnimations.begin(); it != activeAnimations.end();)
    {
        AnimationState animationState = it->second;

        animationState.animationTime += deltaTime; // 後に可変フレームに対応させる

        if (animationState.isInfiniteLoop)
        {
            animationState.animationTime = fmod(animationState.animationTime, animationState.animation.duration);
        }
        else
        {
            if (animationState.animationTime = fmod(animationState.animationTime, animationState.animation.duration))
            {
                ++animationState.currentLoopCount;

                if (animationState.maxLoopCount == animationState.currentLoopCount)
                {
                    return;
                }
            }
        }

        NodeAnimation& rootNodeAnimation = animationState.animation.nodeAnimations[animationState.animation.modelData.rootNode.name];
        


    }
}

Vector3 AnimationManager::CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time)
{
    //assert(!keyframes.empty()); // キーがないのでとめる
    //if (keyframes.size() == 1 || time <= keyframes[0].time)
    //{
    //    return keyframes[0].value;
    //}

    for (size_t index = 0; index < keyframes.size() - 1; ++index)
    {
        size_t nextIndex = index + 1;

        //indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
        if (keyframes[index].time <= time && time <= keyframes[nextIndex].time)
        {
            // 範囲内を補間する
            float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
           
            return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
        }
    }

    return (*keyframes.rbegin()).value;
}
