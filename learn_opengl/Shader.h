#pragma once
#include "Libs.h"

class Shader
{
public:
	//vector<string> texNameList;
	unsigned int texSlotCounter;
private:
	unsigned int id;
	unsigned int loadShader(const char* fileName, GLenum type);
public:
	Shader(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometricShaderFile = nullptr);
	~Shader();
	unsigned int getId()const { return id; }
	void use();
	//void bindTextures(Texture* textures);
	//void bindTexture(Texture& texture, unsigned int idx);
	int getUniformLocation(const string& name) const;
	void setBool(const string& name, bool value) const;
	void setBool(int location, bool value) const;
	void setInt(const string& name, int value) const;
	void setInt(int location, int value) const;
	void setFloat(const string& name, float value) const;
	void setFloat(int location, float value) const;
	void set2fv(const string& name, const GLfloat* vecPtr) const;
	void set2fv(int location, const GLfloat* vecPtr) const;
	void set2f(const string& name, float v1, float v2) const;
	void set2f(int location, float v1, float v2, float v3) const;
	void set3fv(const string& name, const GLfloat* vecPtr) const;
	void set3fv(int location, const GLfloat* vecPtr) const;
	void set3f(const string& name, float v1, float v2, float v3) const;
	void set3f(int location, float v1, float v2, float v3) const;
	void set4fv(const string& name, const GLfloat* vecPtr) const;
	void set4fv(int location, const GLfloat* vecPtr) const;
	void set4f(const string& name, float v1, float v2, float v3, float v4) const;
	void set4f(int location, float v1, float v2, float v3, float v4) const;
	void setMat3fv(const string& name, const GLfloat* matPtr, bool transpose = false) const;
	void setMat3fv(int location, const GLfloat* matPtr, bool transpose = false) const;
	void setMat4fv(const string& name, const GLfloat* matPtr, bool transpose = false) const;
	void setMat4fv(int location, const GLfloat* matPtr, bool transpose = false) const;
};