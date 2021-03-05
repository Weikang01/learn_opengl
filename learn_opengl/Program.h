#pragma once
#include "Libs.h"
#include "Camera.h"

class Program
{
private:
	GLFWwindow* m_window;
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
public:
	Program(const int screen_width, const int screen_height);
	~Program();
	void do_movement(Camera& camera);
	
	int shouldClose();
	void begin_loop();
	void end_loop();
	GLFWwindow* window() const { return m_window; }
};