#include "Animation.h"
#include "AssimpGLMHelper.h"

Animation::Animation(const string& animationPath, Model* model)
{
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);

	auto animation = scene->mAnimations[0];
	duration = animation->mDuration;
	ticksPerSecond = animation->mTicksPerSecond;
	readHeirarchyData(rootNode, scene->mRootNode);
	readMissingBones(animation, *model);
}

Bone* Animation::findBone(const string& name)
{
	auto iter = std::find_if(bones.begin(), bones.end(),
		[&](const Bone& bone)
		{
			return bone.getName() == name;
		});
	if (iter == bones.end()) return nullptr;
	else return &(*iter);
}

void Animation::readHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);
	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelper::convertMatrix2GLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;
	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		readHeirarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}

void Animation::readMissingBones(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;

	auto& boneInfoMap = model.getBoneInfoMap();
	int& boneCount = model.getBoneCount();

	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		bones.push_back(Bone(channel->mNodeName.data, 
			boneInfoMap[channel->mNodeName.data].id, channel));
	}
	m_boneInfoMap = boneInfoMap;
}
