#include "Animator.h"

Animator::Animator(Animation* animation)
	:deltaTime(0.f)
{
	currentTime = 0.f;
	currentAnimation = animation;
	transforms.reserve(100);
	for (int i = 0; i < 100; i++)
		transforms.push_back(glm::mat4(1.f));
}

void Animator::updateAnimation(float dt)
{
	deltaTime = dt;
	if (currentAnimation)
	{
		currentTime += currentAnimation->getTicksPerSecond() * dt;
		currentTime = fmod(currentTime, currentAnimation->getDuration());
		calculateBoneTransform(&currentAnimation->getRootNode(), glm::mat4(1.f));
	}
}

void Animator::calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;
	Bone* bone = currentAnimation->findBone(nodeName);

	if (bone)
	{
		bone->update(currentTime);
		nodeTransform = bone->getLocalTransform();
	}
	glm::mat4 globalTransform = parentTransform * nodeTransform;
	auto& boneInfoMap = currentAnimation->getBoneInfoMap();
	if (boneInfoMap.find(nodeName)!= boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		transforms[index] = globalTransform * offset;
	}
	for (int i = 0; i < node->childrenCount; i++)
		calculateBoneTransform(&node->children[i], globalTransform);
}

void Animator::playAnimation(Animation* pAnimation)
{
	currentAnimation = pAnimation;
	currentTime = 0.f;
}
