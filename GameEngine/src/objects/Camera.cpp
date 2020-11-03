#include "Camera.h"
#include "../Application.h"

const float Camera::move_offset = 0.05f;

void Camera::updatePosition() {
	// Moving the camera if the movements keys are currently down
	if (Application::move_keys[Application::Key::W]) 
		_position.z -= move_offset;

	if (Application::move_keys[Application::Key::A]) 
		_position.x -= move_offset;

	if (Application::move_keys[Application::Key::S]) 
		_position.z += move_offset;

	if (Application::move_keys[Application::Key::D])
		_position.x += move_offset;

	if (Application::move_keys[Application::Key::Q])
		_position.y -= move_offset;
	
	if (Application::move_keys[Application::Key::E])
		_position.y += move_offset;
}
