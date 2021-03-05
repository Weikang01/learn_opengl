#include "Program.h"

static bool keys[1024];
//float lastX, lastY;
GLfloat delta_time;
GLfloat last_time;

void readFile(const char* fileName, string& target)
{
	string cur{};
	ifstream file(fileName);
	if (file.is_open())
	{
		while (getline(file, cur))
		{
			target.append(cur + "\n");
		}
		file.close();
	}
	else
		cout << "FILE_OPEN_FAILED::" << fileName << endl;
}

Program::Program(const int screen_width, const int screen_height)
{
	glfwInit();
	constexpr int WINDOW_VERSION_MAJOR = 4;
	constexpr int WINDOW_VERSION_MINOR = 4;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, WINDOW_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, WINDOW_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	m_window = glfwCreateWindow(screen_width, screen_height, "hello world", NULL, NULL);

	// set callback functions
	glfwSetKeyCallback(m_window, keyCallback);
	// set input mode
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	if (!m_window) { glfwTerminate(); }

	glfwMakeContextCurrent(m_window);

	if (glewInit() != GLEW_OK)
	{
		cout << "GLEW_INIT_ERROR" << endl;
		glfwTerminate();
	}
	glewExperimental = GL_TRUE;
	glfwSetWindowSizeCallback(m_window,
		[](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height);});

	// openGL options
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

Program::~Program()
{
	glfwTerminate();
}

int Program::shouldClose()
{
	return glfwWindowShouldClose(m_window);
}

void Program::begin_loop()
{
	// clearcolor ClearColor Clearcolor
	glClearColor(.07f, .06f, .05f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Program::end_loop()
{
	GLfloat current_time = glfwGetTime();
	delta_time = current_time - last_time;
	last_time = current_time;

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Program::do_movement(Camera& camera)
{
	if (keys[GLFW_KEY_W])
	{
		camera.processCameraMovement(Camera::CAMERA_DIRECTION::FORWARD, delta_time);
	}
	if (keys[GLFW_KEY_S])
	{
		camera.processCameraMovement(Camera::CAMERA_DIRECTION::BACKWARD, delta_time);
	}
	if (keys[GLFW_KEY_A])
	{
		camera.processCameraMovement(Camera::CAMERA_DIRECTION::LEFT, delta_time);
	}
	if (keys[GLFW_KEY_D])
	{
		camera.processCameraMovement(Camera::CAMERA_DIRECTION::RIGHT, delta_time);
	}
}

void Program::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
		if (key == GLFW_KEY_ESCAPE ||
			key == GLFW_KEY_SPACE ||
			key == GLFW_KEY_ENTER)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

