#include "Program.h"
#include "Camera.h"
#include "Model.h"
#include "Structures.h"
#include "Primitive.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		cout << "[OpenGL Error] (" << error << "): " << function << " " << file << endl;
		return false;
	}
	return true;
}

GLuint loadTexture(const GLchar* path, bool gamma = false);
GLuint loadCubeMap(const vector<const GLchar*>& texture_faces, bool gamma = false);
GLuint generateMultiSampleTexture(GLuint samples);
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
glm::mat3 getNormalMat(const glm::mat4& model);
static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

constexpr int screen_width = 800;
constexpr int screen_height = 600;
Camera camera((float)screen_width / screen_height, glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024]{ false };
float lastX, lastY;

void drawScene(Shader& shader, Mesh& mesh);

int main()
{
	lastX = (float)screen_width / 2.f;
	lastY = (float)screen_height / 2.f;
	Program main_program = Program(screen_width, screen_height);
	glfwSetCursorPosCallback(main_program.window(), cursorPosCallback);
	glfwSetScrollCallback(main_program.window(), scrollCallback);
	
	// Setup some OpenGL options
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glViewport(0, 0, screen_width, screen_height);

	// Setup and compile our shaders
	Shader shader("parallax_mapping_vertex.glsl", "parallax_mapping_fragment.glsl");

	// First. We get the relevant block indices
	GLuint uniformBlockIndexShader = shader.getUniformBlockIndex("Matrices");
	// Then we link each shader's uniform block to this uniform binding point
	shader.uniformBlockBinding(uniformBlockIndexShader, 0);

	// Now actually create the buffer
	GLuint uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	// texture
	Texture texBrick;
	stbi_set_flip_vertically_on_load(0);
	texBrick.id = loadTexture("Images/toy_box_diffuse.png", true);
	texBrick.type = "texture_diffuse";
	Texture texBrick_norm;
	texBrick_norm.id = loadTexture("Images/toy_box_normal.png");
	texBrick_norm.type = "texture_normal";
	Texture texBrick_disp;
	texBrick_disp.id = loadTexture("Images/toy_box_disp.png");
	texBrick_disp.type = "texture_height";
	// mesh
	vector<Vertex> vertices = Prim::quadVertices;
	//vector<Vertex> vertices = Prim::cubeVertices;

	for (size_t i = 0; i < vertices.size(); i += 3)
	{
		glm::vec3 edge1 = vertices[i + 1].position - vertices[i].position;
		glm::vec3 edge2 = vertices[i + 2].position - vertices[i].position;
		glm::vec2 deltaUV1 = vertices[i + 1].texCoord - vertices[i].texCoord;
		glm::vec2 deltaUV2 = vertices[i + 2].texCoord - vertices[i].texCoord;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent(0.f);
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		vertices[i].tangent = glm::normalize(tangent);
		vertices[i + 1].tangent = glm::normalize(tangent);
		vertices[i + 2].tangent = glm::normalize(tangent);

		glm::vec3 bitangent(0.f);
		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		vertices[i].bitangent = glm::normalize(bitangent);
		vertices[i + 1].bitangent = glm::normalize(bitangent);
		vertices[i + 2].bitangent = glm::normalize(bitangent);
	}

	Mesh cube(vertices);
	cube.add_texture(texBrick);
	cube.add_texture(texBrick_norm);
	cube.add_texture(texBrick_disp);

	// Light source
	glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

	double delta = glfwGetTime();
	bool parallax = true;

	while (!main_program.shouldClose())
	{
		main_program.begin_loop();
		main_program.do_movement(camera);

		//lightPos = glm::vec3(0.f, sin(glm::radians(glfwGetTime())), cos(glm::radians(glfwGetTime())));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);

        glm::mat4 projection = glm::perspective(camera.getFOV(), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
        glm::mat4 view = camera.getView();

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        
		// Set light uniforms
		shader.set3fv("lightPos", lightPos);
		shader.set3fv("viewPos", camera.Position);

		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4fv("model", model);
		shader.setMat3fv("normalMat", getNormalMat(model));
		shader.setFloat("height_scale", 0.2f);
		if (glfwGetKey(main_program.window(), GLFW_KEY_J) == GLFW_PRESS && glfwGetTime() - 0.5 > delta)
		{
			parallax = !parallax;
			delta = glfwGetTime();
		}
		shader.setBool("parallax", parallax);
		cube.draw(shader);

		main_program.end_loop();
	}
}

void drawScene(Shader& shader, Mesh& mesh)
{
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4fv("model", model);
	shader.setMat3fv("normalMat", getNormalMat(model));
	mesh.draw(shader);
}


void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	static bool firstMouse = true;
	if (firstMouse)
	{
		lastX = (float)xPos;
		lastY = (float)yPos;
		firstMouse = false;
	}

	GLfloat xoffset = (float)xPos - lastX;
	GLfloat yoffset = lastY - (float)yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = (float)xPos;
	lastY = (float)yPos;
	camera.processCameraRotation(xoffset, yoffset);
}

static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processCameraZoom((float)yOffset);
}

GLuint loadTexture(const GLchar* path, bool gamma)
{
	unsigned int texID;
	glGenTextures(1, &texID);

	int t_width, t_height, t_nrChannels;
	unsigned char* data = stbi_load(path, &t_width, &t_height, &t_nrChannels, 0);

	if (data)
	{
		GLenum colorChannelArray[]{ GL_FALSE, GL_R, GL_RG, GL_RGB, GL_RGBA };
		GLenum colorChannelArrayGamma[]{ GL_FALSE, GL_FALSE, GL_FALSE, GL_SRGB, GL_SRGB_ALPHA };

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		glBindTexture(GL_TEXTURE_2D, texID);
		if (!gamma)
			glTexImage2D(GL_TEXTURE_2D, 0, colorChannelArray[t_nrChannels], t_width, t_height, 0, 
				colorChannelArray[t_nrChannels], GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, colorChannelArrayGamma[t_nrChannels], t_width, t_height, 0,
				colorChannelArray[t_nrChannels], GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "ERROR::TEXTURE_IMAGE_IMPORT_FAILED::" << path << endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
}

GLuint loadCubeMap(const vector<const GLchar*>& texture_faces, bool gamma)
{
	unsigned int texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	GLenum colorChannelArray[]{ GL_FALSE, GL_R, GL_RG, GL_RGB, GL_RGBA };
	GLenum colorChannelArrayGamma[]{ GL_FALSE, GL_FALSE, GL_FALSE, GL_SRGB, GL_SRGB_ALPHA };

	int t_width, t_height, t_nrChannels;
	unsigned char* data;
	stbi_set_flip_vertically_on_load(0);
	for (size_t i = 0; i < texture_faces.size(); i++)
	{
		data = stbi_load(texture_faces[i], &t_width, &t_height, &t_nrChannels, 0);
		if (data)
		{
			if (!gamma)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
					colorChannelArray[t_nrChannels], t_width, t_height,
					0, colorChannelArray[t_nrChannels], GL_UNSIGNED_BYTE, data);
			else
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
					colorChannelArrayGamma[t_nrChannels], t_width, t_height,
					0, colorChannelArray[t_nrChannels], GL_UNSIGNED_BYTE, data);
		}
		else
		{
			cout << "ERROR::TEXTURE_IMAGE_IMPORT_FAILED::" << texture_faces[i] << endl;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return texID;
}

GLuint generateMultiSampleTexture(GLuint samples)
{
	GLuint texID;
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, screen_width, screen_height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	
	return texID;
}

GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, screen_width, screen_height, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else// Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screen_width, screen_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
}

glm::mat3 getNormalMat(const glm::mat4& model)
{
	glm::mat4 normalMat4 = glm::inverse(model);
	return glm::mat3(
		glm::vec3(normalMat4[0][0], normalMat4[1][0], normalMat4[2][0]),
		glm::vec3(normalMat4[0][1], normalMat4[1][1], normalMat4[2][1]),
		glm::vec3(normalMat4[0][2], normalMat4[1][2], normalMat4[2][2])
	);
}
