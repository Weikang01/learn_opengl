#pragma once

#include "Animation.h"

class Animator
{
private:
	vector<glm::mat4> transforms;
	Animation* currentAnimation;
	float currentTime;
	float deltaTime;
public:
	Animator(Animation* animation);
	void updateAnimation(float dt);
	void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
	void playAnimation(Animation* pAnimation);

	vector<glm::mat4> getFinalBoneMatrices() { return transforms; }
};