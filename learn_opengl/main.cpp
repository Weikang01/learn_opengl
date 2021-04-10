#include "Program.h"
#include "Camera.h"
#include "Model.h"
#include "Structures.h"
#include "Primitive.h"

#define APIENTRY GLAPIENTRY
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
	while (GLenum errorCode = glGetError())
	{
		string error;
		switch (errorCode)
		{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}

		cout << "[" << error<<"] :: " << function << " | " << file << endl;
		return false;
	}
	return true;
}
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, void* userParam);
void GetFirstNMessages(GLuint numMsgs);


void drawScene(Shader& shader, Mesh& mesh, vector<Texture>& textures);
GLuint loadTexture(const GLchar* path, bool gamma = false);
GLuint* loadTexture_hdr(const GLchar* path, bool to_cubeMap = true); // return [texID, fboID]
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
vector<glm::vec3> lightPositions = {
	glm::vec3(-10.f,  10.f, 10.f),
	glm::vec3( 10.f,  10.f, 10.f),
	glm::vec3(-10.f, -10.f, 10.f),
	glm::vec3( 10.f, -10.f, 10.f)
};
vector<glm::vec3> lightColors = {
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f)
};


int main()
{
	lastX = (float)screen_width / 2.f;
	lastY = (float)screen_height / 2.f;
	Program main_program = Program(screen_width, screen_height);
	glfwSetCursorPosCallback(main_program.window(), cursorPosCallback);
	glfwSetScrollCallback(main_program.window(), scrollCallback);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT, GL_FILL);
	//glEnable(GL_MULTISAMPLE);

	glViewport(0, 0, screen_width, screen_height);
	// Setup and compile our shaders
	Shader shader("PBR_shader_vertex.glsl", "PBR_shader_fragment_env.glsl");
	Shader shader_noText("PBR_shader_vertex_noText.glsl", "PBR_shader_fragment_noText_env.glsl");
	Shader shader_skybox("PBR_skybox_vertex.glsl", "PBR_skybox_fragment.glsl");
	Shader shader_irradiance("rectangular2cubemap_vertex.glsl", "PBR_irradiance_convolution_fragment.glsl");
	// First. We get the relevant block indices
	GLuint uniformBlockIndexShader = shader.getUniformBlockIndex("Matrices");
	GLuint uniformBlockIndexShader_nt = shader_noText.getUniformBlockIndex("Matrices");
	GLuint uniformBlockIndexSkybox = shader_skybox.getUniformBlockIndex("Matrices");
	// Then we link each shader's uniform block to this uniform binding point
	shader.uniformBlockBinding(uniformBlockIndexShader, 0);
	shader_noText.uniformBlockBinding(uniformBlockIndexShader_nt, 0);
	shader_skybox.uniformBlockBinding(uniformBlockIndexSkybox, 0);

	shader_skybox.setInt("skybox", 0);

	// Now actually create the buffer
	GLuint uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	// mesh
	Mesh sphere = drawSphere(32, 32);
	Mesh cube(Prim::cubeVertices);
	Mesh quad(Prim::quadVertices);

	// texture -- bool: import textures or not
	Texture texEnvironment;
	GLuint* hdrPtr = loadTexture_hdr("Images/newport_loft.hdr");
	GLuint environFBO = hdrPtr[1];
	GLuint environRBO = hdrPtr[2];
	texEnvironment.id = hdrPtr[0];
	texEnvironment.type = "environment_cubemap";
	Texture texMetal;
	texMetal.id = loadTexture("Images/pbr_rust/albedo.png");
	texMetal.type = "texture_diffuse";
	Texture texMetal_roughness;
	texMetal_roughness.id = loadTexture("Images/pbr_rust/roughness.png");
	texMetal_roughness.type = "texture_roughness";
	Texture texMetal_normal;
	texMetal_normal.id = loadTexture("Images/pbr_rust/normal.png");
	texMetal_normal.type = "texture_normal";
	Texture texMetal_metal;
	texMetal_metal.id = loadTexture("Images/pbr_rust/metallic.png");
	texMetal_metal.type = "texture_metallic";
	Texture texMetal_ao;
	texMetal_ao.id = loadTexture("Images/pbr_rust/ao.png");
	texMetal_ao.type = "texture_ao";

	// pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
	Texture texIrradiance;
	texIrradiance.type = "irradiance_cubemap";
	glGenTextures(1, &texIrradiance.id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texIrradiance.id);
	for (size_t i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
			32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	glBindFramebuffer(GL_FRAMEBUFFER, environFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, environRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	glm::mat4 projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f);
	glm::mat4 views[] = {
		glm::lookAt(glm::vec3(0.f), glm::vec3( 1.f,  0.f,  0.f), glm::vec3(0.f, -1.f,  0.f)),
		glm::lookAt(glm::vec3(0.f), glm::vec3(-1.f,  0.f,  0.f), glm::vec3(0.f, -1.f,  0.f)),
		glm::lookAt(glm::vec3(0.f), glm::vec3( 0.f,  1.f,  0.f), glm::vec3(0.f,  0.f,  1.f)),
		glm::lookAt(glm::vec3(0.f), glm::vec3( 0.f, -1.f,  0.f), glm::vec3(0.f,  0.f, -1.f)),
		glm::lookAt(glm::vec3(0.f), glm::vec3( 0.f,  0.f,  1.f), glm::vec3(0.f, -1.f,  0.f)),
		glm::lookAt(glm::vec3(0.f), glm::vec3( 0.f,  0.f, -1.f), glm::vec3(0.f, -1.f,  0.f))
	};
	cube.set_texture(texEnvironment);
	shader_irradiance.setMat4fv("projection", projection);

	glViewport(0, 0, 32, 32);
	for (size_t i = 0; i < 6; i++)
	{
		shader_irradiance.setMat4fv("view", views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texIrradiance.id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cube.draw(shader_irradiance);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screen_width, screen_height);

	double delta = glfwGetTime();

	bool first = true;

	vector<Texture> textures_shader = { texIrradiance, texMetal, texMetal_roughness, 
		texMetal_normal, texMetal_metal, texMetal_ao };
	vector<Texture> textures_shader_noTex = { texIrradiance };
	

	while (!main_program.shouldClose())
	{
		main_program.begin_loop();
		main_program.do_movement(camera);
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		// generate gBuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 projection = glm::perspective(camera.getFOV(), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
			glm::mat4 view = camera.getView();

			glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

			glm::mat4 model(1.0f);
			//model = glm::translate(model, glm::vec3(-1.f, 0.f, 0.f));
			//shader.set3fv_vector("light", "position", lightPositions);
			//shader.set3fv_vector("light", "color", lightColors);
			//shader.set3fv("viewPos", camera.Position);
			//shader.setMat4fv("model", model);
			//shader.setMat3fv("normalMat", getNormalMat(model));
			//sphere.set_textures({ texIrradiance, texMetal,texMetal_roughness,texMetal_normal,texMetal_metal,texMetal_ao });
			//sphere.draw(shader, GL_TRIANGLE_STRIP);

			drawScene(shader_noText, sphere, textures_shader_noTex);

			//model = glm::mat4(1.f);
			//model = glm::translate(model, glm::vec3(1.f, 0.f, 0.f));
			//shader_noText.set3fv_vector("light", "position", lightPositions);
			//shader_noText.set3fv_vector("light", "color", lightColors);
			//shader_noText.set3fv("viewPos", camera.Position);
			//shader_noText.setMat4fv("model", model);
			//shader_noText.setMat3fv("normalMat", getNormalMat(model));
			//sphere.set_texture({ texIrradiance });
			//sphere.draw(shader_noText, GL_TRIANGLE_STRIP);

			glDisable(GL_CULL_FACE);
			shader_skybox.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texEnvironment.id);
			cube.set_texture(texEnvironment);
			cube.draw(shader_skybox);
			glEnable(GL_CULL_FACE);
		
		main_program.end_loop();
	}
}

void drawScene(Shader& shader, Mesh& mesh, vector<Texture>& textures)
{
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5f;

	shader.set3fv_vector("light", "position", lightPositions);
	shader.set3fv_vector("light", "color", lightColors);
	shader.set3fv("viewPos", camera.Position);
	mesh.set_textures(textures);
	glm::mat4 model(1.0f);
	for (int row = 0; row < nrRows; row++)
	{
		shader.setFloat("metallic", (float)row / (float)nrRows);
		for (int col = 0; col < nrColumns; col++)
		{
			// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			shader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.f));
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(float)(col - (nrColumns / 2)) * spacing,
				(float)(row - (nrRows / 2)) * spacing,
				-2.0f
			));
			shader.setMat4fv("model", model);
			shader.setMat3fv("normalMat", getNormalMat(model));
			mesh.draw(shader, GL_TRIANGLE_STRIP);
		}
	}
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
		GLenum colorChannelArray[]{ GL_FALSE, GL_RED, GL_RG, GL_RGB, GL_RGBA };
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

GLuint* loadTexture_hdr(const GLchar* path, bool to_cubeMap)  // return [texID, fboID, rboID]
{
	unsigned int texID;
	glGenTextures(1, &texID);

	int t_width, t_height, t_nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &t_width, &t_height, &t_nrChannels, 0);

	if (data)
	{
		GLenum colorChannelArray_internal[]{ GL_FALSE, GL_R16F, GL_RG16F, GL_RGB16F, GL_RGBA16F };
		GLenum colorChannelArray[]{ GL_FALSE, GL_RED, GL_RG, GL_RGB, GL_RGBA };

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, colorChannelArray_internal[t_nrChannels], t_width, t_height, 0,
			colorChannelArray[t_nrChannels], GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (to_cubeMap)
		{
			constexpr unsigned int cubemap_width = 512;
			constexpr unsigned int cubemap_height = 512;
			unsigned int captureFBO, captureRBO;
			glGenFramebuffers(1, &captureFBO);
			glGenRenderbuffers(1, &captureRBO);

			glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
			glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubemap_width, cubemap_height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

			unsigned int cubeID;
			glGenTextures(1, &cubeID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeID);

			for (size_t i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
					colorChannelArray_internal[t_nrChannels], cubemap_width, cubemap_height, 0,
					colorChannelArray[t_nrChannels], GL_FLOAT, nullptr);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glm::mat4 projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f);
			glm::mat4 views[] = {
				glm::lookAt(glm::vec3(0.f), glm::vec3( 1.f,  0.f,  0.f), glm::vec3(0.f, -1.f,  0.f)),
				glm::lookAt(glm::vec3(0.f), glm::vec3(-1.f,  0.f,  0.f), glm::vec3(0.f, -1.f,  0.f)),
				glm::lookAt(glm::vec3(0.f), glm::vec3( 0.f,  1.f,  0.f), glm::vec3(0.f,  0.f,  1.f)),
				glm::lookAt(glm::vec3(0.f), glm::vec3( 0.f, -1.f,  0.f), glm::vec3(0.f,  0.f, -1.f)),
				glm::lookAt(glm::vec3(0.f), glm::vec3( 0.f,  0.f,  1.f), glm::vec3(0.f, -1.f,  0.f)),
				glm::lookAt(glm::vec3(0.f), glm::vec3( 0.f,  0.f, -1.f), glm::vec3(0.f, -1.f,  0.f))
			};

			Shader rectangularToCubemap("rectangular2cubemap_vertex.glsl","rectangular2cubemap_fragment.glsl");
			Mesh cube(Prim::cubeVertices);
			Texture rectangularMap;
			rectangularMap.id = texID;
			rectangularMap.type = "rectangularMap";
			cube.add_texture(rectangularMap);
			rectangularToCubemap.setMat4fv("projection", projection);

			glViewport(0, 0, cubemap_width, cubemap_height);
			for (size_t i = 0; i < 6; i++)
			{
				rectangularToCubemap.setMat4fv("view", views[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeID, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				cube.draw(rectangularToCubemap);
			}
			glViewport(0, 0, screen_width, screen_height);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			stbi_image_free(data);
			static GLuint r[3];
			r[0] = cubeID;
			r[1] = captureFBO;
			r[2] = captureRBO;
			return r;
		}
	}
	else
	{
		cout << "ERROR::TEXTURE_IMAGE_IMPORT_FAILED::" << path << endl;
	}
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);


	static GLuint r[3];
	r[0] = texID;
	r[1] = 0;
	r[2] = 0;
	return r;
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

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	cout << "---------------" << endl;
	cout << "Debug message (" << id << "): " << message << endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           cout << "Source: Other"; break;
	} cout << endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               cout << "Type: Other"; break;
	} cout << endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: cout << "Severity: notification"; break;
	} cout << endl;
	cout << endl;
}

void GetFirstNMessages(GLuint numMsgs)
{
	GLint maxMsgLen = 0;
	glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &maxMsgLen);

	std::vector<GLchar> msgData(numMsgs * maxMsgLen);
	std::vector<GLenum> sources(numMsgs);
	std::vector<GLenum> types(numMsgs);
	std::vector<GLenum> severities(numMsgs);
	std::vector<GLuint> ids(numMsgs);
	std::vector<GLsizei> lengths(numMsgs);

	GLuint numFound = glGetDebugMessageLog(numMsgs, 512, &sources[0], &types[0], &ids[0], &severities[0], &lengths[0], &msgData[0]);

	sources.resize(numFound);
	types.resize(numFound);
	severities.resize(numFound);
	ids.resize(numFound);
	lengths.resize(numFound);

	std::vector<std::string> messages;
	messages.reserve(numFound);

	std::vector<GLchar>::iterator currPos = msgData.begin();
	for (size_t msg = 0; msg < lengths.size(); ++msg)
	{
		messages.push_back(std::string(currPos, currPos + lengths[msg] - 1));
		currPos = currPos + lengths[msg];
	}
	for (size_t i = 0; i < messages.size(); i++)
	{
		cout << messages[i] << endl;
	}
}


template<typename T>
T lerp(T a, T b, T f)
{
	return a + f * (b - a);
}