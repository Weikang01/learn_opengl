#include "Bone.h"
#include "AssimpGLMHelper.h"
#include "gtx/quaternion.hpp"

Bone::Bone(const string& name, int id, const aiNodeAnim* channel)
	:name(name), id(id), localTransform(1.f)
{
	numPositions = channel->mNumPositionKeys;
	for (int positionIndex = 0; positionIndex < numPositions; positionIndex++)
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = channel->mPositionKeys[positionIndex].mTime;
		keyPosition data;
		data.position = AssimpGLMHelper::getGLMVec3(aiPosition);
		data.timeStamp = timeStamp;
		positions.push_back(data);
	}

	numRotations = channel->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < numRotations; rotationIndex++)
	{
		aiQuaternion aiRotation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
		keyRotation data;
		data.orientation = AssimpGLMHelper::getGLMQuat(aiRotation);
		data.timeStamp = timeStamp;
		rotations.push_back(data);
	}

	numScales = channel->mNumScalingKeys;
	for (int scaleIndex = 0; scaleIndex < numScales; scaleIndex++)
	{
		aiVector3D aiScale = channel->mScalingKeys[scaleIndex].mValue;
		float timeStamp = channel->mScalingKeys[scaleIndex].mTime;
		keyScale data;
		data.scale = AssimpGLMHelper::getGLMVec3(aiScale);
		data.timeStamp = timeStamp;
		scales.push_back(data);
	}
}

void Bone::update(float animationTime)
{
	glm::mat4 translation = interpolatePosition(animationTime);
	glm::mat4 rotation = interpolateRotation(animationTime);
	glm::mat4 scale = interpolateScaling(animationTime);
	localTransform = translation * rotation * scale;
}

float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float framesDiff = nextTimeStamp - lastTimeStamp;
	float midWayLength = animationTime - lastTimeStamp;
	return midWayLength / framesDiff;
}

int Bone::getPositionIndex(float animationTime)
{
	for (int index = 0; index < numPositions - 1; index++)
	{
		if (animationTime < positions[index+1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::getRotationIndex(float animationTime)
{
	for (int index = 0; index < numRotations - 1; index++)
	{
		if (animationTime < rotations[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::getScaleIndex(float animationTime)
{
	for (int index = 0; index < numScales - 1; index++)
	{
		if (animationTime < scales[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

glm::mat4 Bone::interpolatePosition(float animationTime)
{
	if (1 == numPositions)
		return glm::translate(glm::mat4(1.f), positions[0].position);

	int p0index = getPositionIndex(animationTime);
	int p1index = p0index + 1;
	float scaleFactor = getScaleFactor(positions[p0index].timeStamp, positions[p1index].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(positions[p0index].position, positions[p1index].position, scaleFactor);
	return glm::translate(glm::mat4(1.f), finalPosition);
}

glm::mat4 Bone::interpolateRotation(float animationTime)
{
	if (1 == numRotations)
	{
		auto rotation = glm::normalize(rotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int p0index = getRotationIndex(animationTime);
	int p1index = p0index + 1;
	float scaleFactor = getScaleFactor(rotations[p0index].timeStamp, rotations[p1index].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(rotations[p0index].orientation, rotations[p1index].orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolateScaling(float animationTime)
{
	if (1 == numScales)
		return glm::scale(glm::mat4(1.f), scales[0].scale);

	int p0index = getRotationIndex(animationTime);
	int p1index = p0index + 1;
	float scaleFactor = getScaleFactor(scales[p0index].timeStamp, scales[p1index].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(scales[p0index].scale, scales[p1index].scale, scaleFactor);
	return glm::scale(glm::mat4(1.f), finalScale);
}
