#include "Camera.h"

Camera::Camera(float aspect, glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov)
	:Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch),
	MouseSensitivity(CAMERA_SENSITIVITY), ScrollSensitivity(.02f), FOV(fov),
	CameraSpeed(CAMERA_SPEED), Aspect(aspect)
{
	UpdateVectors();
}

void Camera::UpdateVectors()
{
	glm::vec3 direction{};

	direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	direction.y = sin(glm::radians(Pitch));
	direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	this->Front = glm::normalize(direction);
	this->Right = glm::normalize(glm::cross(Front, WorldUp));
	this->Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::processCameraMovement(Camera::CAMERA_DIRECTION direction, GLfloat deltaTime)
{
	//glm::vec3 moveDir = glm::vec3(Front.x, 0.f, Front.z);
	
	if (direction == Camera::CAMERA_DIRECTION::FORWARD)
	{
		this->Position += Front * CameraSpeed * deltaTime;
	}
	if (direction == Camera::CAMERA_DIRECTION::BACKWARD)
	{
		this->Position -= Front * CameraSpeed * deltaTime;
	}
	if (direction == Camera::CAMERA_DIRECTION::LEFT)
	{
		this->Position -= glm::normalize(glm::cross(Front, Up)) * CameraSpeed * deltaTime;
	}
	if (direction == Camera::CAMERA_DIRECTION::RIGHT)
	{
		this->Position += glm::normalize(glm::cross(Front, Up)) * CameraSpeed * deltaTime;
	}
}

void Camera::processCameraRotation(GLfloat xOffset, GLfloat yOffset, bool constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	this->Yaw   += xOffset;
	this->Pitch += yOffset;
	if (constrainPitch)
	{
		if (Pitch > 89.f)
			Pitch = 89.f;
		if (Pitch < -89.f)
			Pitch = -89.f;
	}
	this->UpdateVectors();
}

void Camera::processCameraZoom(GLfloat offset)
{
	ScrollSensitivity = .1f;
	if (this->FOV <= 45.f && this->FOV >= 1.f)
		this->FOV -= offset * ScrollSensitivity;
	else if (this->FOV >= 45.f)
		this->FOV = 45.f;
	else
		this->FOV = 1.f;
}

glm::mat4 Camera::getView()
{
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

glm::mat4 Camera::getProjection()
{
	return glm::perspective(this->FOV, this->Aspect, .1f, 100.f);
}

