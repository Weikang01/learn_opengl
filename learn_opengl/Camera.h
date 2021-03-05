#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#include "Libs.h"

const GLfloat CAMERA_YAW = -90.f;
const GLfloat CAMERA_PITCH = 0.f;
const GLfloat CAMERA_SPEED = 3.f;
const GLfloat CAMERA_SENSITIVITY = .03f;
const GLfloat CAMERA_FOV = 45.f;

class Camera
{
public:
	enum class CAMERA_DIRECTION
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	glm::vec3 Position;
private:
	unsigned int viewID;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;
	// camera options
	float CameraSpeed;
	float MouseSensitivity;
	float ScrollSensitivity;
	float FOV;
	float Aspect;

	void UpdateVectors();

public:

	Camera(float aspect, glm::vec3 position = glm::vec3{ 0.f },
		glm::vec3 up = glm::vec3{0.f, 1.f, 0.f}, 
		float yaw = CAMERA_YAW, float pitch = CAMERA_PITCH, 
		float fov = CAMERA_FOV);
	
	void processCameraMovement(CAMERA_DIRECTION direction, GLfloat deltaTime);
	void processCameraRotation(GLfloat xOffset, GLfloat yOffset, bool constrainPitch = true);
	void processCameraZoom(GLfloat offset);
	glm::mat4 getView();
	glm::mat4 getProjection();
	float getFOV()const { return FOV; }
};