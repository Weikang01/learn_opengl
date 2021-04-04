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

// Set light uniforms
vector<glm::vec3> lightPositions
{
	glm::vec3( 0.0f, 0.5f,  1.5f),
	glm::vec3(-4.0f, 0.5f, -3.0f),
	glm::vec3( 3.0f, 0.5f,  1.0f),
	glm::vec3(-0.8f, 2.4f, -1.0f)
};

vector<glm::vec3> lightColors
{
	glm::vec3( 5.0f, 5.0f,  5.0f),
	glm::vec3(10.0f, 0.0f,  0.0f),
	glm::vec3( 0.0f, 0.0f, 15.0f),
	glm::vec3( 0.0f, 5.0f,  0.0f)
};

vector<glm::vec3> cubePositions
{
	glm::vec3( 0.0f, -1.0f,  0.0f),
	glm::vec3( 0.0f,  1.5f,  0.0f),
	glm::vec3( 2.0f,  0.0f,  1.0f),
	glm::vec3(-1.0f, -1.0f,  2.0f),
	glm::vec3( 0.0f,  2.7f,  4.0f),
	glm::vec3(-2.0f,  1.0f, -3.0f),
	glm::vec3(-3.0f,  0.0f,  0.0f)
};

vector<glm::vec3> cubeScales
{
	glm::vec3(12.5f,  0.5f, 12.5f),
	glm::vec3( 0.5f,  0.5f,  0.5f),
	glm::vec3( 0.5f,  0.5f,  0.5f),
	glm::vec3( 1.0f,  1.0f,  1.0f),
	glm::vec3(1.25f, 1.25f, 1.25f),
	glm::vec3( 1.0f,  1.0f,  1.0f),
	glm::vec3( 0.5f,  0.5f,  0.5f)
};


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
	//glCullFace(GL_BACK);

	glViewport(0, 0, screen_width, screen_height);

	// Setup and compile our shaders
	Shader shader("bloom_shader_vertex.glsl", "bloom_shader_fragment.glsl");
	Shader blurShader("bloom_blurShader_vertex.glsl", "bloom_blurShader_fragment.glsl");
	Shader lightShader("bloom_lightShader_vertex.glsl", "bloom_lightShader_fragment.glsl");
	Shader finalShader("bloom_finalShader_vertex.glsl", "bloom_finalShader_fragment.glsl");

	// First. We get the relevant block indices
	GLuint uniformBlockIndexShader = shader.getUniformBlockIndex("Matrices");
	GLuint uniformBlockIndexLightShader = lightShader.getUniformBlockIndex("Matrices");
	// Then we link each shader's uniform block to this uniform binding point
	shader.uniformBlockBinding(uniformBlockIndexShader, 0);
	lightShader.uniformBlockBinding(uniformBlockIndexLightShader, 0);

	// Now actually create the buffer
	GLuint uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	// texture
	Texture texWood;
	stbi_set_flip_vertically_on_load(0);
	texWood.id = loadTexture("Images/wood.png");
	texWood.type = "texture_diffuse";
	Texture texCube;
	texCube.id = loadTexture("Images/container2.png");
	texCube.type = "texture_diffuse";
	// mesh
	Mesh cube(Prim::cubeVertices);
	Mesh quad(Prim::quadVertices);

	// hdr framebuffer
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		// colorbuffer
		Texture colorBuffers[2];
		GLuint colorBuffers_id[2];
		glGenTextures(2, colorBuffers_id);
		for (size_t i = 0; i < 2; i++)
		{
			colorBuffers[i].id = colorBuffers_id[i];
			glBindTexture(GL_TEXTURE_2D, colorBuffers_id[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers_id[i], 0);
			if (i == 0)
				colorBuffers[i].type = "FragColor";
			else
				colorBuffers[i].type = "BrightColor";
		}
		// depth renderbuffer
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_width, screen_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		GLuint attachment[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachment);

		quad.add_texture(colorBuffers[0]);
		quad.add_texture(colorBuffers[1]);
		

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Framebuffer not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// pingpong fbo
	GLuint pingpongFBO[2];
	GLuint pingpongColorBuffers_id[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorBuffers_id);

	Texture pingpongColorBuffers[2];
	for (size_t i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		pingpongColorBuffers[i].id = pingpongColorBuffers_id[i];
		pingpongColorBuffers[i].type = "BrightColor";
		glBindTexture(GL_TEXTURE_2D, pingpongColorBuffers_id[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorBuffers_id[i], 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Framebuffer not complete!" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	double delta = glfwGetTime();

	bool first = true;
	while (!main_program.shouldClose())
	{
		main_program.begin_loop();
		main_program.do_movement(camera);
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 projection = glm::perspective(camera.getFOV(), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
			glm::mat4 view = camera.getView();

			glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

			shader.set3fv_vector("lightPositions", lightPositions);
			shader.set3fv_vector("lightColors", lightColors);
			shader.set3fv("viewPos", camera.Position);
			cube.set_texture(texWood);
			for (size_t i = 0; i < cubePositions.size(); i++)
			{
				if (i == 1)
					cube.set_texture(texCube);
				glm::mat4 model(1.0f);
				model = glm::translate(model, cubePositions[i]);
				if (i == cubePositions.size() - 2)
					model = glm::rotate(model, glm::radians(124.f), glm::normalize(glm::vec3(1.f, 0.f, 1.f)));
				else if (i == cubePositions.size() - 3)
					model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.f, 0.f, 1.f)));
				else if (i == cubePositions.size() - 4)
					model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.f, 0.f, 1.f)));

				model = glm::scale(model, cubeScales[i]);

				shader.setMat4fv("model", model);
				shader.setMat3fv("normalMat", getNormalMat(model));
				quad.set_texture(colorBuffers[1], 1);
				cube.draw(shader);
			}
			cube.clear_texture();

			for (size_t i = 0; i < lightPositions.size(); i++)
			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, lightPositions[i]);
				model = glm::scale(model, glm::vec3(0.25f));
				lightShader.setMat4fv("model", model);
				lightShader.set3fv("lightColor", lightColors[i]);
				cube.draw(lightShader);
			}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ping-pong-framebuffer for blurring
		unsigned int amount = 10;
		bool first_iter = true;
		bool horizontal = true;
		for (size_t i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			blurShader.setBool("horizontal", horizontal);
			if (!first_iter)
				quad.set_texture(pingpongColorBuffers[!horizontal], 1);
			else
				first_iter = false;
			quad.draw(blurShader);
			horizontal = !horizontal;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		quad.set_texture(pingpongColorBuffers[!horizontal], 1);
		quad.draw(finalShader);
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
