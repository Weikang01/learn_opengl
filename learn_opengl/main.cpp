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
template<typename T>
T lerp(T a, T b, T f);
constexpr int screen_width = 800;
constexpr int screen_height = 600;
Camera camera((float)screen_width / screen_height, glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024]{ false };
float lastX, lastY;

// Set light uniforms
glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

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
	//glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_MULTISAMPLE);

	glViewport(0, 0, screen_width, screen_height);

	// Setup and compile our shaders
	Shader shader("SSAO_shader_vertex.glsl", "SSAO_shader_fragment.glsl");
	Shader deferShader("SSAO_deferShader_vertex.glsl", "SSAO_deferShader_fragment.glsl");
	Shader SSAOshader("SSAO_SSAOshader_vertex.glsl", "SSAO_SSAOshader_fragment.glsl");
	Shader SSAOblurShader("SSAO_SSAOblurShader_vertex.glsl", "SSAO_SSAOblurShader_fragment.glsl");

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

	constexpr unsigned int NR_LIGHTS = 32;

	// texture
	Texture texBox;
	stbi_set_flip_vertically_on_load(0);
	texBox.id = loadTexture("Images/container2.png");
	texBox.type = "texture_diffuse";
	Texture texBox_spec;
	texBox_spec.id = loadTexture("Images/container2_specular.png");
	texBox_spec.type = "texture_specular";
	// mesh
	Model backpack("Models/backpack/backpack.obj");
	Mesh cube(Prim::cubeVertices);
	Mesh quad(Prim::quadVertices);

	// hdr framebuffer
	GLuint gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		// colorbuffer
		constexpr unsigned int BF_NR = 3;
		Texture colorBuffers[BF_NR];
		GLuint colorBuffers_id[BF_NR];
		colorBuffers[0].type = "gPosition";
		colorBuffers[1].type = "gNormal";
		colorBuffers[2].type = "gAlbedo";
		glGenTextures(BF_NR, colorBuffers_id);
		for (size_t i = 0; i < BF_NR; i++)
		{
			colorBuffers[i].id = colorBuffers_id[i];
			glBindTexture(GL_TEXTURE_2D, colorBuffers_id[i]);
			if (i == 0)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
			else if (i == 1)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
			else if (i == 2)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers_id[i], 0);
		}
		// depth renderbuffer
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_width, screen_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		GLuint attachment[BF_NR] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(BF_NR, attachment);

		quad.add_texture(colorBuffers[0]);
		quad.add_texture(colorBuffers[1]);
		quad.add_texture(colorBuffers[2]);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Framebuffer not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint ssaoFBO;
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		Texture ssaoColorBuffer;
		ssaoColorBuffer.type = "AmbientOcclusion";
		glGenTextures(1, &ssaoColorBuffer.id);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, screen_width, screen_height, 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer.id, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Framebuffer not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint ssaoBlurFBO;
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		Texture ssaoBlurColorBuffer;
		ssaoBlurColorBuffer.type = "AmbientOcclusion";
		glGenTextures(1, &ssaoBlurColorBuffer.id);
		glBindTexture(GL_TEXTURE_2D, ssaoBlurColorBuffer.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, screen_width, screen_height, 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBlurColorBuffer.id, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Framebuffer not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// generate sample kernel
	std::uniform_real_distribution<float> randomFloat(0.f, 1.f);
	std::default_random_engine generator;
	vector<glm::vec3> ssaoKernel;
	for (size_t i = 0; i < 64; i++)
	{
		glm::vec3 sample(randomFloat(generator) * 2.f - 1.f, randomFloat(generator) * 2.f - 1.f, randomFloat(generator));
		sample = glm::normalize(sample);
		sample *= randomFloat(generator);
		float scale = float(i) / 64.f;

		// scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	// generate noise texture
	vector<glm::vec3> ssaoNoise;
	for (size_t i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloat(generator) * 2.f - 1.f, randomFloat(generator) * 2.f - 1.f, 0.f);
		ssaoNoise.push_back(noise);
	}
	Texture noiseTexture;
	noiseTexture.type = "texNoise";
	glGenTextures(1, &noiseTexture.id);
	glBindTexture(GL_TEXTURE_2D, noiseTexture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	double delta = glfwGetTime();

	bool first = true;
	bool occlusion = false;
	while (!main_program.shouldClose())
	{
		main_program.begin_loop();
		main_program.do_movement(camera);
		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		// generate gBuffer
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 projection = glm::perspective(camera.getFOV(), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
			glm::mat4 view = camera.getView();

			glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

			{
				glm::mat4 model(1.0f);
				model = glm::translate(model, glm::vec3(0.0, 3.5f, 0.0f));
				model = glm::scale(model, glm::vec3(7.5f));
				shader.setMat4fv("model", model);
				shader.setMat3fv("normalMat", getNormalMat(model));
				shader.setBool("invertedNormal", true);
				glCullFace(GL_FRONT);
				cube.draw(shader);
				glCullFace(GL_BACK);
				shader.setBool("invertedNormal", false);
			}
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
				model = glm::scale(model, glm::vec3(.2f));
				shader.setMat4fv("model", model);
				shader.setMat3fv("normalMat", getNormalMat(model));
				backpack.draw(shader);
			}
		// generate	SSAO
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
			glClear(GL_COLOR_BUFFER_BIT);
			SSAOshader.set3fv_vector("samples", ssaoKernel);
			SSAOshader.setFloat("screen_width", screen_width);
			SSAOshader.setFloat("screen_height", screen_height);

			SSAOshader.setMat4fv("projection", projection);

			quad.set_texture(noiseTexture, 2);
			quad.draw(SSAOshader);
		// blur SSAO output
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
			glClear(GL_COLOR_BUFFER_BIT);
			quad.clear_texture();
			quad.add_texture(ssaoColorBuffer);
			quad.draw(SSAOblurShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float linear = .09f;
		float quadratic = .032f;

		glm::vec3 lightPosView = glm::vec3(camera.getView() * glm::vec4(lightPos, 1.f));

		deferShader.set3fv("light", "position", lightPosView);
		deferShader.set3fv("light", "color", lightColor);
		deferShader.setFloat("light", "linear", linear);
		deferShader.setFloat("light", "quadratic", quadratic);
		deferShader.set3fv("viewPos", camera.Position);

		if ((glfwGetKey(main_program.window(), GLFW_KEY_O) == GLFW_PRESS) && glfwGetTime() - 1 > delta)
		{
			occlusion = !occlusion;
			delta = glfwGetTime();
			cout << "pressed::"<< occlusion << endl;
		}

		//glDisable(GL_DEPTH_TEST);
		deferShader.setBool("occlusion", occlusion);
		quad.set_texture(colorBuffers[0], 0);
		quad.set_texture(colorBuffers[1], 1);
		quad.set_texture(colorBuffers[2], 2);
		quad.set_texture(ssaoBlurColorBuffer, 3);
		quad.draw(deferShader);
		//glEnable(GL_DEPTH_TEST);

		
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

template<typename T>
T lerp(T a, T b, T f)
{
	return a + f * (b - a);
}