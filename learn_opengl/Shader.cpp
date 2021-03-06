#include "Shader.h"


unsigned int Shader::loadShader(const char* code, GLenum type, int dummy)
{
	unsigned int shaderId;
	shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &code, NULL);
	glCompileShader(shaderId);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		cout << "SHADER_COMPILATION_FAILED::" << infoLog << code << "::TYPE::" << type << endl;
	}

	return shaderId;
}

unsigned int Shader::loadShader(const char* fileName, GLenum type)
{
	string codeString;
	ifstream file;
	file.exceptions(ifstream::failbit | ifstream::badbit);
	stringstream fileStream;
	try
	{
		file.open(fileName);
		fileStream << file.rdbuf();
		codeString = fileStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "FILE_OPEN_FAILED::" << fileName << endl;
	}
	const char* code = codeString.c_str();

	return loadShader(code, type, 0);
}

void Shader::loadProgram(unsigned int vertexId, unsigned int fragmentId, unsigned int geometryId)
{
	id = glCreateProgram();
	glAttachShader(id, vertexId);
	glAttachShader(id, fragmentId);
	if (geometryId) { glAttachShader(id, geometryId); }
	glLinkProgram(id);

	int success;
	char infoLog[512];
	glGetProgramiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		cout << "SHADER_PROGRAM_COMPILATION_FAILED::" << infoLog << endl;
	}

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
	if (geometryId) { glDeleteShader(geometryId); }
}

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile)
	:texSlotCounter(0)
{
	unsigned int vertexId = loadShader(vertexShaderFile, GL_VERTEX_SHADER);
	unsigned int fragmentId = loadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	unsigned int geometryId = geometryShaderFile ? loadShader(geometryShaderFile, GL_GEOMETRY_SHADER) : 0;
	loadProgram(vertexId, fragmentId, geometryId);
}

Shader::Shader(int dummy, const char* vertexShaderCode, const char* fragmentShaderCode, const char* geometricShaderCode)
	:texSlotCounter(0)
{
	unsigned int vertexId = loadShader(vertexShaderCode, GL_VERTEX_SHADER, 0);
	unsigned int fragmentId = loadShader(fragmentShaderCode, GL_FRAGMENT_SHADER, 0);
	unsigned int geometryId = geometricShaderCode ? loadShader(geometricShaderCode, GL_GEOMETRY_SHADER, 0) : 0;
	loadProgram(vertexId, fragmentId, geometryId);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::use()
{
	glUseProgram(id);
}

int Shader::getUniformLocation(const string& name) const
{
	return glGetUniformLocation(id, name.c_str());
}

GLuint Shader::getUniformBlockIndex(const string& name) const
{
	return glGetUniformBlockIndex(id, name.c_str());
}

GLuint Shader::getUniformBlockIndex(const string& listName, const string& memberName, const unsigned int& index) const
{
	return glGetUniformBlockIndex(id, (listName + "[" + std::to_string(index) + "]." + memberName).c_str());
}

void Shader::uniformBlockBinding(GLuint uniformBlockIndex, int bindingPoint)
{
	glUniformBlockBinding(id, uniformBlockIndex, bindingPoint);
}

void Shader::setBool(const string& name, bool value) const
{
	glUseProgram(id);
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setBool(int location, bool value) const
{
	glUseProgram(id);
	glUniform1i(location, (int)value);
}

void Shader::setInt(const string& name, int value) const
{
	glUseProgram(id);
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setInt(const string& listName, const string& memberName, const int& value) const
{
	glUseProgram(id);
	glUniform1i(glGetUniformLocation(id, (listName + "." + memberName).c_str()), value);
}

void Shader::setInt(const string& listName, const string& memberName, int value, const unsigned int& idx) const
{
	glUseProgram(id);
	glUniform1i(getUniformBlockIndex(listName, memberName, idx), value);
}

void Shader::setInt(int location, int value) const
{
	glUseProgram(id);
	glUniform1i(location, value);
}

void Shader::setInt_vector(const string& name, const vector<int> vec) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		setInt(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec[i]);
}

void Shader::setInt_vector(const string& name, const int& value, const unsigned int& size) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		setInt(getUniformLocation(name + "[" + std::to_string(i) + "]"), value);
}

void Shader::setInt_vector(const string& listName, const string& memberName, const vector<int>& vec) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		setInt(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), vec[i]);
}

void Shader::setInt_vector(const string& listName, const string& memberName, const int& value, const unsigned int& size) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		setInt(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), value);
}

void Shader::setFloat(const string& name, float value) const
{
	glUseProgram(id);
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const string& listName, const string& memberName, const float& value) const
{
	glUseProgram(id);
	glUniform1f(glGetUniformLocation(id, (listName + "." + memberName).c_str()), value);
}

void Shader::setFloat(const string& listName, const string& memberName, float value, const unsigned int& idx) const
{
	glUseProgram(id);
	glUniform1f(getUniformBlockIndex(listName, memberName, idx), value);
}

void Shader::setFloat(int location, float value) const
{
	glUseProgram(id);
	glUniform1f(location, value);
}

void Shader::setFloat_vector(const string& name, const vector<float>& vec) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		setFloat(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec[i]);
}

void Shader::setFloat_vector(const string& name, const float& value, const unsigned int& size) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		setFloat(getUniformLocation(name + "[" + std::to_string(i) + "]"), value);
}

void Shader::setFloat_vector(const string& listName, const string& memberName, const vector<float>& vec) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		setFloat(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), vec[i]);
}

void Shader::setFloat_vector(const string& listName, const string& memberName, const float& value, const unsigned int& size) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		setFloat(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), value);
}

void Shader::set2fv(const string& name, const glm::vec2& vec) const
{
	glUseProgram(id);
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::set2fv(const string& listName, const string& memberName, const glm::vec2& vec) const
{
	glUseProgram(id);
	glUniform2fv(glGetUniformLocation(id, (listName + "." + memberName).c_str()), 1, glm::value_ptr(vec));
}

void Shader::set2fv(const string& listName, const string& memberName, const glm::vec2& vec, const unsigned int& idx) const
{
	glUseProgram(id);
	glUniform2fv(getUniformBlockIndex(listName, memberName, idx), 1, glm::value_ptr(vec));
}

void Shader::set2fv(int location, const glm::vec2& vec) const
{
	glUseProgram(id);
	glUniform2fv(location, 1, glm::value_ptr(vec));
}

void Shader::set2f(const string& name, float v1, float v2) const
{
	glUseProgram(id);
	glUniform2f(glGetUniformLocation(id, name.c_str()), v1, v2);
}

void Shader::set2f(int location, float v1, float v2) const
{
	glUseProgram(id);
	glUniform2f(location, v1, v2);
}

void Shader::set2fv_vector(const string& name, const vector<glm::vec2>& vec) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		set2fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec[i]);
}

void Shader::set2fv_vector(const string& name, const glm::vec2& vec, const unsigned int& size) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		set2fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec);
}

void Shader::set2fv_vector(const string& listName, const string& memberName, const vector<glm::vec2>& vec) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		set2fv(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), vec[i]);
}

void Shader::set2fv_vector(const string& listName, const string& memberName, const glm::vec2& vec, const unsigned int& size) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		set2fv(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), vec);
}

void Shader::set3fv(const string& name, const glm::vec3& vec) const
{
	glUseProgram(id);
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::set3fv(const string& listName, const string& memberName, const glm::vec3& vec) const
{
	glUseProgram(id);
	glUniform3fv(glGetUniformLocation(id, (listName + "." + memberName).c_str()), 1, glm::value_ptr(vec));
}

void Shader::set3fv(const string& listName, const string& memberName, const glm::vec3& vec, const unsigned int& idx) const
{
	glUseProgram(id);
	glUniform3fv(getUniformBlockIndex(listName, memberName, idx), 1, glm::value_ptr(vec));
}

void Shader::set3fv(int location, const glm::vec3& vec) const
{
	glUseProgram(id);
	glUniform3fv(location, 1, glm::value_ptr(vec));
}

void Shader::set3f(const string& name, float v1, float v2, float v3) const
{
	glUseProgram(id);
	glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
}

void Shader::set3f(int location, float v1, float v2, float v3) const
{
	glUseProgram(id);
	glUniform3f(location, v1, v2, v3);
}

void Shader::set3fv_vector(const string& name, const vector<glm::vec3>& vec) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		set3fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec[i]);
}

void Shader::set3fv_vector(const string& name, const glm::vec3& vec, const unsigned int& size) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		set3fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec);
}

void Shader::set3fv_vector(const string& listName, const string& memberName, const vector<glm::vec3>& vec) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		set3fv(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), vec[i]);
}

void Shader::set3fv_vector(const string& listName, const string& memberName, const glm::vec3& vec, const unsigned int& size) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		set3fv(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), vec);
}

void Shader::set4f(const string& name, float v1, float v2, float v3, float v4) const
{
	glUseProgram(id);
	glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}

void Shader::set4fv(const string& name, const glm::vec4& vec) const
{
	glUseProgram(id);
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::set4fv(const string& listName, const string& memberName, const glm::vec4& vec) const
{
	glUseProgram(id);
	glUniform4fv(glGetUniformLocation(id, (listName + "." + memberName).c_str()), 1, glm::value_ptr(vec));
}

void Shader::set4fv(const string& listName, const string& memberName, const glm::vec4& vec, const unsigned int& idx) const
{
	glUseProgram(id);
	glUniform4fv(getUniformBlockIndex(listName, memberName, idx), 1, glm::value_ptr(vec));
}

void Shader::set4fv(int location, const glm::vec4& vec) const
{
	glUseProgram(id);
	glUniform4fv(location, 1, glm::value_ptr(vec));
}

void Shader::set4f(int location, float v1, float v2, float v3, float v4) const
{
	glUseProgram(id);
	glUniform4f(location, v1, v2, v3, v4);
}

void Shader::set4fv_vector(const string& name, const vector<glm::vec4>& vec) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		set4fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec[i]);
}

void Shader::set4fv_vector(const string& name, const glm::vec4& vec, const unsigned int& size) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		set4fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec);
}

void Shader::set4fv_vector(const string& listName, const string& memberName, const vector<glm::vec4>& vec) const
{
	for (size_t i = 0; i < vec.size(); i++)
		set4fv(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), vec[i]);
}

void Shader::set4fv_vector(const string& listName, const string& memberName, const glm::vec4& vec, const unsigned int& size) const
{
	for (size_t i = 0; i < size; i++)
		set4fv(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), vec);
}

void Shader::setMat3fv(const string& name, const glm::mat3& mat, bool transpose) const
{
	glUseProgram(id);
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, transpose, glm::value_ptr(mat));
}

void Shader::setMat3fv(const string& listName, const string& memberName, const glm::mat3& mat, bool transpose) const
{
	glUseProgram(id);
	glUniformMatrix3fv(glGetUniformLocation(id, (listName + "." + memberName).c_str()), 1, transpose, glm::value_ptr(mat));
}

void Shader::setMat3fv(const string& listName, const string& memberName, const glm::mat3& mat, const unsigned int& idx, bool transpose) const
{
	glUseProgram(id);
	glUniformMatrix3fv(getUniformBlockIndex(listName, memberName, idx), 1, transpose, glm::value_ptr(mat));
}

void Shader::setMat3fv(int location, const glm::mat3& mat, bool transpose) const
{
	glUseProgram(id);
	glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(mat));
}

void Shader::setMat3fv_vector(const string& name, const vector<glm::mat3>& vec, bool transpose) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		setMat3fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec[i], transpose);
}

void Shader::setMat3fv_vector(const string& name, const glm::mat3& mat, const unsigned int& size, bool transpose) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		setMat3fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), mat, transpose);
}

void Shader::setMat3fv_vector(const string& listName, const string& memberName, const vector<glm::mat3>& vec, bool transpose) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		setMat3fv(getUniformLocation(listName + "[" + std::to_string(i) + "]."+ memberName), vec[i], transpose);
}

void Shader::setMat3fv_vector(const string& listName, const string& memberName, const glm::mat3& mat, const unsigned int& size, bool transpose) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		setMat3fv(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), mat, transpose);
}

void Shader::setMat4fv(const string& name, const glm::mat4& mat, bool transpose) const
{
	glUseProgram(id);
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, transpose, glm::value_ptr(mat));
}

void Shader::setMat4fv(const string& listName, const string& memberName, const glm::mat4& mat, bool transpose) const
{
	glUseProgram(id);
	glUniformMatrix4fv(glGetUniformLocation(id, (listName + "." + memberName).c_str()), 1, transpose, glm::value_ptr(mat));
}

void Shader::setMat4fv(const string& listName, const string& memberName, const glm::mat4& mat, const unsigned int& idx, bool transpose) const
{
	glUseProgram(id);
	glUniformMatrix4fv(getUniformBlockIndex(listName, memberName, idx), 1, transpose, glm::value_ptr(mat));
}

void Shader::setMat4fv(int location, const glm::mat4& mat, bool transpose) const
{
	glUseProgram(id);
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(mat));
}

void Shader::setMat4fv_vector(const string& name, const vector<glm::mat4>& vec, bool transpose) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		setMat4fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), vec[i], transpose);
}

void Shader::setMat4fv_vector(const string& name, const glm::mat4& mat, const unsigned int& size, bool transpose) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		setMat4fv(getUniformLocation(name + "[" + std::to_string(i) + "]"), mat, transpose);
}

void Shader::setMat4fv_vector(const string& listName, const string& memberName, const vector<glm::mat4>& vec, bool transpose) const
{
	glUseProgram(id);
	for (size_t i = 0; i < vec.size(); i++)
		setMat4fv(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), vec[i], transpose);
}

void Shader::setMat4fv_vector(const string& listName, const string& memberName, const glm::mat4& mat, const unsigned int& size, bool transpose) const
{
	glUseProgram(id);
	for (size_t i = 0; i < size; i++)
		setMat4fv(getUniformLocation(listName + "[" + std::to_string(i) + "]." + memberName), mat, transpose);
}
