#pragma once
#include "Libs.h"
#include "Model.h"
#include "Bone.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	string name;
	int childrenCount;
	vector<AssimpNodeData> children;
};

class Animation
{
private:
	float duration;
	int ticksPerSecond;
	vector<Bone> bones;
	AssimpNodeData rootNode;
	map<string, boneInfo> m_boneInfoMap;
public:
	Animation() = default;
	Animation(const string& animationPath, Model* model);
	~Animation() {}

	float getDuration() { return duration; }
	int getTicksPerSecond() { return ticksPerSecond; }
	AssimpNodeData& getRootNode() { return rootNode; }
	map<string, boneInfo>& getBoneInfoMap() { return m_boneInfoMap; }

	Bone* findBone(const string& name);
private:
	void readHeirarchyData(AssimpNodeData& dest, const aiNode* src);
	void readMissingBones(const aiAnimation* animation, Model& model);
};