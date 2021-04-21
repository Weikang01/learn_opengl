#pragma once
#include "Libs.h"

struct keyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct keyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct keyScale
{
	glm::vec3 scale;
	float timeStamp;
};


class Bone
{
private:
	vector<keyPosition> positions;
	vector<keyRotation> rotations;
	vector<keyScale> scales;
	int numPositions;
	int numRotations;
	int numScales;
	
	glm::mat4 localTransform;
	string name;
	int id;
public:
	Bone(const string& name, int id, const aiNodeAnim* channel);
	string getName() const { return name; }
	glm::mat4 getLocalTransform() const { return localTransform; }
	void update(float animationTime);

private:
	float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	int getPositionIndex(float animationTime);
	int getRotationIndex(float animationTime);
	int getScaleIndex(float animationTime);

	glm::mat4 interpolatePosition(float animationTime);
	glm::mat4 interpolateRotation(float animationTime);
	glm::mat4 interpolateScaling(float animationTime);
};