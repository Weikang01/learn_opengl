#include "Program.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Structures.h"


static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f)
	//glm::vec3(0.0f,  0.0f, -3.0f)
};
vector<Vertex> vertices{
	// position         
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),

	Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f)),

	Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),

	 Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)),
	 Vertex(glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)),
	 Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)),
	 Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)),
	 Vertex(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)),
	 Vertex(glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)),

	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)),

	Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f)),
	Vertex(glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f)),
	Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f)),
	Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f))
};

constexpr int screen_width = 800;
constexpr int screen_height = 600;
Camera camera((float)screen_width / screen_height, glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024]{ false };
float lastX, lastY;

// light options
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
int main()
{
	
	lastX = (float)screen_width / 2.f;
	lastY = (float)screen_height / 2.f;
	Program main_program = Program(screen_width, screen_height);
	glfwSetCursorPosCallback(main_program.window(), cursorPosCallback);
	glfwSetScrollCallback(main_program.window(), scrollCallback);

	Shader lampShader("vertex_core.glsl", "lamp_fragment.glsl", nullptr);
	Shader lightShader("lighting_vertex.glsl", "lighting_fragment.glsl", nullptr);


	// uniform locations
	lampShader.use();
	int modelMatLocation_lamp = lampShader.getUniformLocation("model");
	int viewMatLocation_lamp = lampShader.getUniformLocation("view");
	int projectionMatLocation_lamp = lampShader.getUniformLocation("projection");
	glm::mat4 model = glm::mat4(1.f);

	lampShader.setMat4fv(modelMatLocation_lamp, glm::value_ptr(model));

	lightShader.use();
	//int objectColorLocation = lightShader.getUniformLocation("objectColor");
	Material mat("material",
		glm::vec3(1.0f, 0.5f, 0.31f),
		glm::vec3(1.0f, 0.5f, 0.31f),
		glm::vec3(0.5f, 0.5f, 0.5f), 32.f);
	mat.setAsUniform(lightShader);

	DirLight dirLight("dirLight", glm::vec3(-0.2f, -1.0f, -0.3f),
		glm::vec3(.05f), glm::vec3(.4f), glm::vec3(.5f));
	dirLight.setAsUniform(lightShader);
	SpotLight spotLight("spotLight", glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(.05f), glm::vec3(.8f), glm::vec3(1.f),glm::vec3(0.f,0.f,1.f));
	spotLight.setAsUniform(lightShader);
	vector<PointLight> pointLights;
	
	for (size_t i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++)
	{
		pointLights.push_back(PointLight("pointLights[" + std::to_string(i) + "]", 
			pointLightPositions[i], glm::vec3(.05f), glm::vec3(.8f), glm::vec3(1.f), 1.f,.09f, .032f));
		pointLights[i].setAsUniform(lightShader);
	}
	
	//lig.setAsUniform(lightShader);
	int lightColorLocation = lightShader.getUniformLocation("lightColor");
	//int lightPosLocation = lightShader.getUniformLocation("lightPos");

	int modelMatLocation_light = lightShader.getUniformLocation("model");
	int viewMatLocation_light = lightShader.getUniformLocation("view");
	int projectionMatLocation_light = lightShader.getUniformLocation("projection");
	int normalMatLocation_light = lightShader.getUniformLocation("normalMat");

	int viewPosLocation_light = lightShader.getUniformLocation("viewPos");
	
	model = glm::mat4(1.f);
	//model = glm::scale(model, glm::vec3(1.5f, 2.f, 0.75f));
	glm::mat4 normalMat4 = glm::inverse(model);
	glm::mat3 normalMat(
		glm::vec3(normalMat4[0][0], normalMat4[1][0], normalMat4[2][0]),
		glm::vec3(normalMat4[0][1], normalMat4[1][1], normalMat4[2][1]),
		glm::vec3(normalMat4[0][2], normalMat4[1][2], normalMat4[2][2])
	);
	lightShader.setMat4fv(modelMatLocation_light, glm::value_ptr(model));
	lightShader.setMat3fv(normalMatLocation_light, glm::value_ptr(normalMat));
	//lightShader.set3f(objectColorLocation, 1.0f, 0.5f, 0.31f);
	lightShader.set3f(lightColorLocation, 1.0f, 1.f, 1.f);
	
	//int intAttribs;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &intAttribs);
	stbi_set_flip_vertically_on_load(true);

	// Material mat;
	// mat.addTex(texture("Images/container2.png", "diffuse"))
	// mat.addTex(texName, fileName, texSettingOptions)
	// mat.addTex(texName, fileName, texSettingOptions)
	// mesh.addMaterial(mat)
	// mesh.bindS
	// 

	Texture texture01("Images/container2.png");
	Texture texture02("Images/container2_specular.png");
	Texture texture03("Images/matrix.jpg");
	//Texture texture02("Images/awesomeface.png", GL_RGBA);
	//glBindTexture(GL_TEXTURE_2D, 0);
	texture01.bindShader(lightShader, "material.diffuse");
	texture02.bindShader(lightShader, "material.specular");
	texture03.bindShader(lightShader, "material.emission");
	//lightShader.setFloat("material.shininess", 0.2f);
	//texture02.bindShader(shader, 1, "texture2");
	
	glm::mat4 view{};
	glm::mat4 projection{};
	glm::vec3 oscilate{};
	glm::vec3 lightColor{};



	Mesh mesh(vertices);
	Mesh light(vertices);

	while (!main_program.shouldClose())
	{
		main_program.begin_loop();
		main_program.do_movement(camera);
		oscilate.x = cos((float)glfwGetTime());
		oscilate.y = sin((float)glfwGetTime());
		oscilate.z = cos((float)glfwGetTime());
		texture01.bind();
		texture02.bind();
		texture03.bind();

		view = camera.getView();
		projection = camera.getProjection();

		
		{
			model = glm::mat4(1.f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
			model = glm::scale(model, glm::vec3(.2f));
			lampShader.setMat4fv(modelMatLocation_lamp, glm::value_ptr(model));
			lampShader.setMat4fv(viewMatLocation_lamp, glm::value_ptr(view));
			lampShader.setMat4fv(projectionMatLocation_lamp, glm::value_ptr(projection));
			mesh.draw(lampShader);
		}

		for (size_t i = 0; i < sizeof(pointLightPositions)/sizeof(glm::vec3); i++)
		{
			model = glm::mat4(1.f);
			model = glm::translate(model, pointLightPositions[i] + oscilate);
			model = glm::scale(model, glm::vec3(.2f));
			lampShader.setMat4fv(modelMatLocation_lamp, glm::value_ptr(model));
			lampShader.setMat4fv(viewMatLocation_lamp, glm::value_ptr(view));
			lampShader.setMat4fv(projectionMatLocation_lamp, glm::value_ptr(projection));
			mesh.draw(lampShader);
		}
		
		lightShader.setMat4fv(viewMatLocation_light, glm::value_ptr(view));
		lightShader.setMat4fv(projectionMatLocation_light, glm::value_ptr(projection));
		lightShader.set3fv(viewPosLocation_light, glm::value_ptr(camera.Position));
		//lightShader.set3fv(lightPosLocation, glm::value_ptr(lightPos + oscilate));
		//lig.setPositionAsUniform(lightShader, glm::value_ptr(glm::vec4(lightPos.x, lightPos.y, lightPos.z,1.f)));
		//lig.setDirectionAsUniform(lightShader, glm::value_ptr(lightPos));
		{
			spotLight.setFrontAsUniform(lightShader, glm::value_ptr(oscilate));
		}

		for (size_t i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++)
		{
			pointLights[i].setPositionAsUniform(lightShader, glm::value_ptr(pointLightPositions[i] + oscilate));
		}
		/*glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());*/
		for (size_t i = 0; i < 10; i++)
		{
			model = glm::mat4(1.f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.f * i;
			if (i % 3 == 0)
				angle += (float)glfwGetTime();
			model = glm::rotate(model, angle, glm::vec3(1.f, .2f, 0.f));
			glm::mat4 normalMat4 = glm::inverse(model);
			glm::mat3 normalMat(
				glm::vec3(normalMat4[0][0], normalMat4[1][0], normalMat4[2][0]),
				glm::vec3(normalMat4[0][1], normalMat4[1][1], normalMat4[2][1]),
				glm::vec3(normalMat4[0][2], normalMat4[1][2], normalMat4[2][2])
			);

			lightShader.setMat4fv(modelMatLocation_light, glm::value_ptr(model));
			lightShader.setMat3fv(normalMatLocation_light, glm::value_ptr(normalMat));
			light.draw(lightShader);
		}

		main_program.end_loop();
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