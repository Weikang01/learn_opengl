#include "Model.h"
#include "AssimpGLMHelper.h"

Model::Model(const GLchar* filePath)
{
	customize_texture = false;
	this->loadModel(filePath);
}

Model::Model(const GLchar* filePath, const vector<Texture>& textures)
{
	customize_texture = true;
	this->textures_loaded = textures;
	this->loadModel(filePath);
}


void Model::draw(Shader& shader)
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}
}

void Model::add_texture(const Texture texture)
{
	textures_loaded.push_back(texture);
	for (Mesh& mesh:meshes)
	{
		mesh.textures.push_back(texture);
	}
}

void Model::set_texture(const Texture texture)
{
	for (Mesh& mesh : meshes)
	{
		mesh.set_texture(texture);
	}
}

void Model::set_texture(const Texture texture, const unsigned int index)
{
	for (Mesh& mesh : meshes)
	{
		mesh.set_texture(texture, index);
	}
}

void Model::set_textures(const vector<Texture> textures)
{
	for (Mesh& mesh : meshes)
	{
		mesh.set_textures(textures);
	}
}

void Model::extractBoneWeightForVertices(vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
	{
		int boneID = -1;
		string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		
		if (boneInfoMap.find(boneName)== boneInfoMap.end())
		{
			boneInfo newBoneInfo;
			newBoneInfo.id = boneCounter;
			newBoneInfo.offset = AssimpGLMHelper::convertMatrix2GLMFormat(
				mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCounter;
			boneCounter++;
		}
		else
			boneID = boneInfoMap[boneName].id;
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeight = mesh->mBones[boneIndex]->mNumWeights;
		for (size_t i = 0; i < numWeight; i++)
		{
			int vertexID = weights[i].mVertexId;
			float weight = weights[i].mWeight;
			assert(vertexID <= vertices.size());
			vertices[vertexID].setBoneData(boneID, weight);
		}
	}
}

void Model::loadModel(string const& path)
{
	Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	if (!scene||scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	this->directory = path.substr(0, path.find_last_of("/"));
	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}
int a = 0;
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex>       vertices{};
	vector<unsigned int>  indices{};
	vector<Texture>      textures{};

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.setVertexBoneDataToDefault();

		vertex.position = AssimpGLMHelper::getGLMVec3(mesh->mVertices[i]);
		if (mesh->HasNormals())
			vertex.normal = AssimpGLMHelper::getGLMVec3(mesh->mNormals[i]);
		else
			vertex.normal = glm::vec3(0.f, 1.f, 0.f);
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = vec;
		}
		else
		{
			vertex.texCoord = glm::vec2(0.f);
		}
		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tangent = AssimpGLMHelper::getGLMVec3(mesh->mTangents[i]);
			vertex.bitangent = AssimpGLMHelper::getGLMVec3(mesh->mBitangents[i]);
		}
		else
		{
			vertex.bitangent = vertex.tangent = glm::vec3(0.f);
		}
		vertices.push_back(vertex);
	}

	extractBoneWeightForVertices(vertices, mesh, scene);

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	if (customize_texture)
	{
		textures = textures_loaded;
	} else if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin() , diffuseMaps.end());
		vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		vector<Texture> reflectionMaps = this->loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflection");
		textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());
		vector<Texture> normalMaps = this->loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin()  , normalMaps.end());
		vector<Texture> heightMaps = this->loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin()  , heightMaps.end());
	}

	return Mesh(vertices, textures, indices);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		for (size_t j = 0; j < this->textures_loaded.size(); j++)
		{
			if (strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory)
{
	string fileName = string(path);
	fileName = directory + "/" + fileName;

	GLuint textureID;
	glGenTextures(1, &textureID);
	
	int t_width, t_height, t_nrChannels;
	unsigned char* data = stbi_load(fileName.c_str(), &t_width, &t_height, &t_nrChannels, 0);

	if (data)
	{
		GLenum colorChannelArray[]{ GL_FALSE, GL_R, GL_RG, GL_RGB, GL_RGBA };

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, colorChannelArray[t_nrChannels], t_width, t_height, 0,
			colorChannelArray[t_nrChannels], GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "ERROR::TEXTURE_IMAGE_IMPORT_FAILED::" << fileName << endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}
