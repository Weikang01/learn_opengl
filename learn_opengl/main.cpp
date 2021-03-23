#include "Program.h"
#include "Camera.h"
#include "Model.h"
#include "Structures.h"
#include "Primitive.h"

GLuint loadTexture(const GLchar* path);
GLuint loadCubeMap(const vector<const GLchar*>& texture_faces);
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

vector<glm::vec3> grassPos{
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f, 0.0f, 0.51f)  ,
	glm::vec3(0.0f, 0.0f, 0.7f)	  ,
	glm::vec3(-0.3f, 0.0f, -2.3f) ,
	glm::vec3(0.5f, 0.0f, -0.6f)
};


int main()
{
	lastX = (float)screen_width / 2.f;
	lastY = (float)screen_height / 2.f;
	Program main_program = Program(screen_width, screen_height);
	glfwSetCursorPosCallback(main_program.window(), cursorPosCallback);
	glfwSetScrollCallback(main_program.window(), scrollCallback);
	
	//glDepthFunc(GL_ALWAYS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_PROGRAM_POINT_SIZE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//Shader shader("vertex_modelLoading.glsl", "fragment_modelLoading.glsl");
	Shader colorShader("vertex_core.glsl", "fragment_singleColor.glsl");
	//Shader singleColorShader("vertex_modelLoading.glsl", "fragment_singleColor.glsl");
	Shader screenShader("vertex_screen.glsl", "fragment_screen.glsl");
	//Shader skyboxShader("vertex_skybox.glsl", "fragment_skybox.glsl");
	//Shader diffuseShader("vertex_core.glsl", "fragment_core.glsl");
	//Shader instanceShader("rock_vertex.glsl", "rock_fragment.glsl");
	//Shader reflexShader("reflex_vertex.glsl", "reflex_fragment.glsl");
	//Shader cubemapShader("cubemap_vertex.glsl","cubemap_fragment.glsl");
	//Shader pointSizeShader("pointSize_vertex.glsl", "pointSize_fragment.glsl");
	//Shader frontFacingShader("frontFacing_vertex.glsl", "frontFacing_fragment.glsl");
	//Shader testGeoShader("pass_through_vertex.glsl", "pass_through_fragment.glsl", "pass_through_geometry.glsl");
	//Shader explodeShader("normal_vertex.glsl", "normal_fragment.glsl", "normal_geometry.glsl");
	//Shader instanceShader("instance_vertex.glsl", "instance_fragment.glsl");
	// Create a uniform buffer object
	// First. We get the relevant block indices
	GLuint uniformBlockIndexColor       = colorShader.getUniformBlockIndex("Matrices");
	//GLuint uniformBlockIndexDiffuse       = diffuseShader.getUniformBlockIndex("Matrices");
	//GLuint uniformBlockIndexInstance      = instanceShader.getUniformBlockIndex("Matrices");
	//GLuint uniformBlockIndexSkybox        = skyboxShader.getUniformBlockIndex("Matrices");
	//GLuint uniformBlockIndexPointSize     = pointSizeShader.getUniformBlockIndex("Matrices");
	//GLuint uniformBlockIndexFrontFacing = frontFacingShader.getUniformBlockIndex("Matrices");
	//GLuint uniformBlockIndexExplode = explodeShader.getUniformBlockIndex("Matrices");
	//GLuint uniformBlockIndexTestGeoShader = testGeoShader.getUniformBlockIndex("Matrices");


	// Then we link each shader's uniform block to this uniform binding point
	colorShader.uniformBlockBinding(uniformBlockIndexColor, 0);
	//skyboxShader.uniformBlockBinding(uniformBlockIndexSkybox, 0);
	//diffuseShader.uniformBlockBinding(uniformBlockIndexDiffuse, 0);
	//instanceShader.uniformBlockBinding(uniformBlockIndexInstance, 0);
	//pointSizeShader.uniformBlockBinding(uniformBlockIndexPointSize, 0);
	//frontFacingShader.uniformBlockBinding(uniformBlockIndexFrontFacing, 0);
	//explodeShader.uniformBlockBinding(uniformBlockIndexExplode, 0);
	//testGeoShader.uniformBlockBinding(uniformBlockIndexTestGeoShader, 0);
	
	// Now actually create the buffer
	GLuint uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// Define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices,0, 2 * sizeof(glm::mat4));

	// texture
	Texture texCube;
	Texture texFloor;
	Texture texGrass;
	Texture texSkybox;

	texCube.id = loadTexture("Images/pattern4diffuseblack.jpg");
	texCube.type = "texture_diffuse";

	texFloor.id = loadTexture("Images/metal.png");
	texFloor.type = "texture_diffuse";

	texGrass.id = loadTexture("Images/grass.png");
	texGrass.type = "texture_diffuse";

	vector<const GLchar*> faces;
	faces.push_back("Images/skybox/left.jpg");
	faces.push_back("Images/skybox/right.jpg");
	faces.push_back("Images/skybox/top.jpg");
	faces.push_back("Images/skybox/bottom.jpg");
	faces.push_back("Images/skybox/back.jpg");
	faces.push_back("Images/skybox/front.jpg");
	texSkybox.id = loadCubeMap(faces);
	texSkybox.type = "skybox";

	// mesh
	//vector<Texture> cubeTexture{ texCube };
	Mesh cube(Prim::cubeVertices);
	Mesh screen(Prim::quadVertices);
	//vector<Texture> floorTexture{ texFloor };
	//Mesh floor(Prim::planeVertices, floorTexture);
	//vector<Texture> grassTexture{ texGrass };
	//Mesh grass(Prim::transparentVertices, grassTexture);
	//vector<Texture> skyboxTexture{ texSkybox };
	//Mesh skybox(Prim::skyboxVertices, skyboxTexture);
	//Mesh cube(Prim::cubeVertices, skyboxTexture);
	//vector<Texture> testGeoTexture{};
	//Mesh testGeoMesh(Prim::pointsVertices, testGeoTexture);
	//cube.add_texture(texCube);
	//cube.add_texture(texFloor);
	//Model ourModel("Models/nanosuit/nanosuit.obj");
	//Model planet("Models/planet/planet.obj");
	//Model rock("Models/rock/rock.obj");
	//ourModel.add_texture(texSkybox);
	//glm::vec2 translations[100]{};
	//vector<glm::vec2>translations{};
	//int index = 0;
	//GLfloat offset = .1f;
	//for (GLint y = -10; y < 10; y += 2)
	//{
	//	for (GLint x = -10; x < 10; x += 2)
	//	{
	//		glm::vec2 translation{};
	//		translation.x = (GLfloat)x / 10.f + offset;
	//		translation.y = (GLfloat)y / 10.f + offset;
	//		translations.push_back(translation);
	//	}
	//}
	////instanceShader.use();
	//Mesh instances(Prim::smallScreenVertices);
	//instances.instantiate(100);
	//instances.setInstanceUniform(instanceShader, translations, "offsets");

	//glBindVertexArray(instances.VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, instances.instanceVBO);
	//glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glVertexAttribDivisor(2, 1); // Tell OpenGL this is an instanced vertex attribute.
	//glBindVertexArray(0);

	// Generate a large list of semi-random model transformation matrices
	//GLuint amount = 10000;
	//glm::mat4* modelMatrices;
	//modelMatrices = new glm::mat4[amount];
	//srand(glfwGetTime());
	//GLfloat radius = 50.f;
	//GLfloat offset = 2.5f;
	//for (size_t i = 0; i < amount; i++)
	//{
	//	glm::mat4 model(1.f);
	//	// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
	//	GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.f;
	//	GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.f - offset;
	//	GLfloat x = sin(angle) * radius + displacement;
	//	displacement = (rand() % (GLint)(2 * offset * 100)) / 100.f - offset;
	//	GLfloat y = displacement * .4f; // Keep height of asteroid field smaller compared to width of x and z
	//	displacement = (rand() % (GLint)(2 * offset * 100)) / 100.f - offset;
	//	GLfloat z = cos(angle) * radius + displacement;
	//	model = glm::translate(model, glm::vec3(x, y, z));

	//	// 2. Scale: Scale between 0.05 and 0.25f
	//	GLfloat scale = (rand() % 20) / 100.f + .05f;
	//	model = glm::scale(model, glm::vec3(scale));

	//	// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
	//	GLfloat rotAngle = (rand() % 360);
	//	model = glm::rotate(model, rotAngle, glm::vec3(.4f, .6f, .8f));

	//	modelMatrices[i] = model;
	//}

	//for (size_t i = 0; i < rock.meshes.size(); i++)
	//{
	//	GLuint VAO = rock.meshes[i].VAO;
	//	GLuint buffer;
	//	glBindVertexArray(VAO);
	//	glGenBuffers(1, &buffer);
	//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	//	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	//	GLsizei vec4Size = sizeof(glm::vec4);
	//	glEnableVertexAttribArray(3);
	//	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	//	glEnableVertexAttribArray(4);
	//	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	//	glEnableVertexAttribArray(5);
	//	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	//	glEnableVertexAttribArray(6);
	//	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

	//	glVertexAttribDivisor(3, 1);
	//	glVertexAttribDivisor(4, 1);
	//	glVertexAttribDivisor(5, 1);
	//	glVertexAttribDivisor(6, 1);

	//	glBindVertexArray(0);
	//}
	//GLfloat quadVertices[] = {
	//	// Positions   // Colors
	//	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	//	-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,
	//	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,

	//	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	//	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	//	 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	//};
	//GLuint quadVAO, quadVBO;
	//glGenVertexArrays(1, &quadVAO);
	//glGenBuffers(1, &quadVBO);
	//glBindVertexArray(quadVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	// Also set instance data

	//DirLight dirLight("dirLight");
	//dirLight.setAsUniform(shader);
	//PointLight pointLight("pointLight");
	//pointLight.setAsUniform(shader);

	//int modelLocation           = shader.getUniformLocation("model");

	//int modelLocation_scs       = singleColorShader.getUniformLocation("model");

	//int projectionLocation_skb  = skyboxShader.getUniformLocation("projection");
	//int viewLocation_skb        = skyboxShader.getUniformLocation("view");

	//int viewDirLocation         = shader.getUniformLocation("viewDir");
	//int shininessLocation       = shader.getUniformLocation("shininess");
	//int normalMatLocation       = shader.getUniformLocation("normalMat");

	//int modelLocation_rflx      = reflexShader.getUniformLocation("model");
	//int camPosLocation_rflx     = reflexShader.getUniformLocation("camPos");
	//int normalMatLocation_rflx  = reflexShader.getUniformLocation("normalMat");

	//int modelLocation_cube      = cubemapShader.getUniformLocation("model");
	//int camPosLocation_cube     = cubemapShader.getUniformLocation("camPos");
	//int normalMatLocation_cube  = cubemapShader.getUniformLocation("normalMat");
	//

	//int modelLocation_ffs       = frontFacingShader.getUniformLocation("model");

	//int modelLocation_ps        = pointSizeShader.getUniformLocation("model");

	//shader.setFloat(shininessLocation, 36.f);

	// framebuffer
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// Create a multisampled color attachment texture
	GLuint textureColorBufferMultiSampled = generateMultiSampleTexture(4);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, screen_width, screen_height);  // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	// Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// second buffer
	GLuint intermediateFBO;
	Texture screenTex;
	screenTex.id = generateAttachmentTexture(false, false);
	screenTex.type = "texture_diffuse";
	screen.add_texture(screenTex);
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTex.id, 0);	// We only need a color buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	while (!main_program.shouldClose())
	{
		main_program.begin_loop();
		main_program.do_movement(camera);

		// Set uniforms
		/////////////////////////////////////////////////////
        // First render pass: Mirror texture...
        // Bind to framebuffer and draw to color texture as 
        // we normally would, but with the view camera 
        // reversed.
        // //////////////////////////////////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(.1f, .08f, .07f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		// cube
		glm::mat4 view = camera.getView();
		glm::mat4 projection = glm::perspective(camera.getFOV(), (float)screen_width / screen_height, .1f, 10000.f);
		//model = glm::scale(model, glm::vec3(.3f));
		//glm::mat3 normalMat = getNormalMat(model);

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glm::mat4 model(1.f);
		colorShader.setMat4fv("model", glm::value_ptr(model));
		cube.draw(colorShader);

		// 2. Now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		// 3. Now render quad with scene's visuals as its texture image
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		screen.draw(screenShader);
		//model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		//diffuseShader.setMat4fv("model", glm::value_ptr(model));
		//planet.draw(diffuseShader);
		
		// Draw meteorites
		//instanceShader.use();
		//glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // Note we also made the textures_loaded vector public (instead of private) from the model class.
		//for (size_t i = 0; i < rock.meshes.size(); i++)
		//{
		//	glBindVertexArray(rock.meshes[i].VAO);
		//	glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		//	glBindVertexArray(0);
		//}
		//for (size_t i = 0; i < amount; i++)
		//{
		//	diffuseShader.setMat4fv("model", glm::value_ptr(modelMatrices[i]));
		//	rock.draw(diffuseShader);
		//}
		//instanceShader.use();
		//instances.draw(instanceShader);


		// skybox
		view = glm::mat4(glm::mat3(camera.getView()));

		//glDepthMask(GL_FALSE);

		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		//skyboxShader.setMat4fv(projectionLocation_skb, glm::value_ptr(projection), false);
		//skyboxShader.setMat4fv(viewLocation_skb, glm::value_ptr(view), false);

		//testGeoShader.use();
		//testGeoMesh.draw(testGeoShader, GL_POINTS);

		// skybox cube
		//skyboxShader.use();
		//skybox.draw(skyboxShader);

		glDepthFunc(GL_LESS);


		//glDepthMask(GL_TRUE);

#pragma region code
		//glm::mat4 model{ 1.f };
		//glm::mat3 normalMat;
		//glm::mat4 projection = glm::perspective(camera.getFOV(), (float)screen_width / screen_height, .1f, 100.f);
		//camera.Yaw += 180.f;
		//camera.Pitch += 180.f;
		//camera.processCameraRotation(0, 0, false);
		//glm::mat4 view = camera.getView();
		//camera.Yaw -= 180.f;
		//camera.Pitch -= 180.f;
		//camera.processCameraRotation(0, 0, false);

		//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		//glClearColor(.1f, .08f, .07f, 1.f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);

		//singleColorShader.setMat4fv(projectionLocation_scs, glm::value_ptr(projection), false);
		//singleColorShader.setMat4fv(viewLocation_scs, glm::value_ptr(view), false);
		//
		//shader.setMat4fv(projectionLocation, glm::value_ptr(projection), false);
		//shader.setMat4fv(viewLocation, glm::value_ptr(view), false);
		//shader.set3fv(viewDirLocation, glm::value_ptr(camera.Position));

		//skyboxShader.setMat4fv(projectionLocation_skb, glm::value_ptr(projection), false);
		//skyboxShader.setMat4fv(viewLocation_skb, glm::value_ptr(view), false);

		//// skybox
		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);

		//skyboxShader.use();
		//skybox.draw(skyboxShader);

		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);

		//// Draw floor as normal, we only care about the containers. The floor should NOT fill the stencil buffer so we set its mask to 0x00
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//shader.setMat4fv(modelLocation, glm::value_ptr(model), false);
		//glStencilMask(0x00);
		//floor.draw(shader);

		//// Render pass, draw objects as normal, filling the stencil buffer
		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);
		//// Draw cubes
		//model = glm::mat4{1.f};
		//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//shader.setMat4fv(modelLocation, glm::value_ptr(model), false);
		////ourModel.draw(shader);

		//shader.use();
		//cube.draw(shader);

		//model = glm::mat4(1.f);
		//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//shader.setMat4fv(modelLocation, glm::value_ptr(model), false);

		//cube.draw(shader);

		//// Render pass, now draw slightly scaled versions of the objects, this time disabling stencil writing.
		//// Because stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are now not drawn, thus only drawing
		//// the objects' size differences, making it look like borders.
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		//GLfloat scale = 1.05f;

		//// cubes
		//model = glm::mat4(1.f);
		//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(scale, scale, scale));
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//singleColorShader.setMat4fv(modelLocation, glm::value_ptr(model), false);
		////ourModel.draw(shader);

		//singleColorShader.use();
		//cube.draw(singleColorShader);

		//model = glm::mat4(1.f);
		//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(scale, scale, scale));
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//singleColorShader.setMat4fv(modelLocation, glm::value_ptr(model), false);

		//cube.draw(singleColorShader);

		//glStencilMask(0xFF);
		//glDisable(GL_STENCIL_TEST);
		//glEnable(GL_DEPTH_TEST);

		//// grass
		//glDisable(GL_CULL_FACE);
		//map<float, glm::vec3> sorted{};
		//for (size_t i = 0; i < grassPos.size(); i++)
		//{
		//	GLfloat distance = glm::length(camera.Position - grassPos[i]);
		//	sorted[distance] = grassPos[i];
		//}
		////glStencilFunc(GL_GREATER, 1, 0xFF);

		//for (map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		//{
		//	model = glm::mat4(1.f);
		//	model = glm::translate(model, it->second);
		//	glm::mat4 normalMat4 = glm::inverse(model);
		//	glm::mat3 normalMat = getNormalMat(model);
		//	shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//	shader.setMat4fv(modelLocation, glm::value_ptr(model), false);
		//	grass.draw(shader);
		//}
		///////////////////////////////////////////////////////
  //      // Second render pass: Draw as normal
  //      // //////////////////////////////////////////////////

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClearColor(.1f, .08f, .07f, 1.f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//glEnable(GL_CULL_FACE);

		//view = camera.getView();

		//singleColorShader.setMat4fv(projectionLocation, glm::value_ptr(projection), false);
		//singleColorShader.setMat4fv(viewLocation, glm::value_ptr(view), false);
		//singleColorShader.set3fv(viewDirLocation, glm::value_ptr(camera.Position));

		//shader.setMat4fv(projectionLocation, glm::value_ptr(projection), false);
		//shader.setMat4fv(viewLocation, glm::value_ptr(view), false);
		//shader.set3fv(viewDirLocation, glm::value_ptr(camera.Position));

		//skyboxShader.setMat4fv(projectionLocation_skb, glm::value_ptr(projection), false);
		//skyboxShader.setMat4fv(viewLocation_skb, glm::value_ptr(view), false);

		//// skybox
		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);

		//skyboxShader.use();
		//skybox.draw(skyboxShader);

		//glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		//// Draw floor as normal, we only care about the containers. The floor should NOT fill the stencil buffer so we set its mask to 0x00
		//model = glm::mat4(1.f);
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//shader.setMat4fv(modelLocation, glm::value_ptr(model), false);
		//glStencilMask(0x00);
		//floor.draw(shader);

		//// Render pass, draw objects as normal, filling the stencil buffer
		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);
		//// Draw cubes
		//model = glm::mat4{ 1.f };
		//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//shader.setMat4fv(modelLocation, glm::value_ptr(model), false);
		////ourModel.draw(shader);

		//shader.use();
		//cube.draw(shader);

		//model = glm::mat4(1.f);
		//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//shader.setMat4fv(modelLocation, glm::value_ptr(model), false);

		//cube.draw(shader);

		//// Render pass, now draw slightly scaled versions of the objects, this time disabling stencil writing.
		//// Because stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are now not drawn, thus only drawing
		//// the objects' size differences, making it look like borders.
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		//scale = 1.05f;

		//// cubes
		//model = glm::mat4(1.f);
		//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(scale, scale, scale));
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//singleColorShader.setMat4fv(modelLocation, glm::value_ptr(model), false);
		////ourModel.draw(shader);

		//singleColorShader.use();
		//cube.draw(singleColorShader);

		//model = glm::mat4(1.f);
		//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(scale, scale, scale));
		//normalMat = getNormalMat(model);
		//shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//singleColorShader.setMat4fv(modelLocation, glm::value_ptr(model), false);

		//cube.draw(singleColorShader);

		//glStencilMask(0xFF);
		//glDisable(GL_STENCIL_TEST);
		//glEnable(GL_DEPTH_TEST);

		//// grass
		//glDisable(GL_CULL_FACE);
		//sorted = map<float, glm::vec3>{};
		//for (size_t i = 0; i < grassPos.size(); i++)
		//{
		//	GLfloat distance = glm::length(camera.Position - grassPos[i]);
		//	sorted[distance] = grassPos[i];
		//}
		////glStencilFunc(GL_GREATER, 1, 0xFF);

		//for (map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		//{
		//	model = glm::mat4(1.f);
		//	model = glm::translate(model, it->second);
		//	glm::mat4 normalMat4 = glm::inverse(model);
		//	glm::mat3 normalMat = getNormalMat(model);
		//	shader.setMat3fv(normalMatLocation, glm::value_ptr(normalMat));
		//	shader.setMat4fv(modelLocation, glm::value_ptr(model), false);
		//	grass.draw(shader);
		//}

		///////////////////////////////////////////////////////
  //      // Bind to default framebuffer again and draw the 
  //      // quad plane with attched screen texture.
  //      // //////////////////////////////////////////////////
		////glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//// Clear all relevant buffers
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER | GL_STENCIL_BUFFER);
		//glDisable(GL_DEPTH_TEST);

		//screenShader.use();
		//quad.draw(screenShader);
#pragma endregion

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

GLuint loadTexture(const GLchar* path)
{
	unsigned int texID;
	glGenTextures(1, &texID);

	int t_width, t_height, t_nrChannels;
	unsigned char* data = stbi_load(path, &t_width, &t_height, &t_nrChannels, 0);

	if (data)
	{
		GLenum colorChannelArray[]{ GL_FALSE, GL_R, GL_RG, GL_RGB, GL_RGBA };

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, colorChannelArray[t_nrChannels], t_width, t_height, 0,
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

GLuint loadCubeMap(const vector<const GLchar*>& texture_faces)
{
	unsigned int texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	GLenum colorChannelArray[]{ GL_FALSE, GL_R, GL_RG, GL_RGB, GL_RGBA };
	
	int t_width, t_height, t_nrChannels;
	unsigned char* data;
	stbi_set_flip_vertically_on_load(0);
	for (size_t i = 0; i < texture_faces.size(); i++)
	{
		data = stbi_load(texture_faces[i], &t_width, &t_height, &t_nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, colorChannelArray[t_nrChannels], t_width, t_height, 0,
				colorChannelArray[t_nrChannels], GL_UNSIGNED_BYTE, data);
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
