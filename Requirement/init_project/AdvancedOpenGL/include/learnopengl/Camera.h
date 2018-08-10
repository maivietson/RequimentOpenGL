#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

// Define several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum Camera_Zoom
{
	CAMERA_ZOOM_IN,
	CAMERA_ZOOM_OUT
};

// Defines camera values
const GLfloat YAW			= -90.0f;
const GLfloat PITCH			= 0.0f;
const GLfloat SPEED			= 30.0;
const GLfloat SENSITIVTY	= 0.5f;
const GLfloat ZOOM			= 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular AngLes, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivty;
	GLfloat Zoom;

	// Contructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), GLfloat yaw = YAW, GLfloat pitch = PITCH)
		: Front(glm::vec3(0.f, 0.f, -1.f)),
		MovementSpeed(SPEED),
		MouseSensitivty(SENSITIVTY),
		Zoom(ZOOM)
	{
		this->Position			= position;
		this->WorldUp			= up;
		this->Yaw				= yaw;
		this->Pitch				= pitch;
		this->updateCameraVectors();
	}

	// Contructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
		: Front(glm::vec3(0.f, 0.f, -1.f)),
		MovementSpeed(SPEED),
		MouseSensitivty(SENSITIVTY),
		Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera define ENUM (to abstract it from windowing system)
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
			this->Position += this->Front * velocity;
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity;
		if (direction == LEFT)
			this->Position -= this->Right * velocity;
		if (direction == RIGHT)
			this->Position += this->Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= this->MouseSensitivty;
		yoffset *= this->MouseSensitivty;

		this->Yaw	+= xoffset;
		this->Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset)
	{
		cout << "yoffset: " << yoffset << std::endl;
		if (yoffset > 0)
		{
			// zoom in
			this->Zoom -= yoffset * this->MouseSensitivty;
			if (this->Zoom <= 1.0f)
				this->Zoom = 1.0f;
			cout << "Zoom in: " << this->Zoom << std::endl;
		}
		else
		{
			this->Zoom -= yoffset * this->MouseSensitivty;
			if (this->Zoom >= 90.0f)
				this->Zoom = 90.0f;
			cout << "Zoom out: " << this->Zoom << std::endl;
		}
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculates the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
		this->Up	= glm::normalize(glm::cross(this->Right, this->Front));
	}
};