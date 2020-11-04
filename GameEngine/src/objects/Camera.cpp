#include "Camera.h"
#include "../Application.h"

const float Camera::move_offset = 0.05f;

void Camera::updatePosition() {
	// Moving the camera if the movements keys are currently down
	if (Application::move_keys[Application::Key::W]) 
		position_.z -= move_offset;

	if (Application::move_keys[Application::Key::A]) 
		position_.x -= move_offset;

	if (Application::move_keys[Application::Key::S]) 
		position_.z += move_offset;

	if (Application::move_keys[Application::Key::D])
		position_.x += move_offset;

	if (Application::move_keys[Application::Key::Q])
		position_.y -= move_offset;
	
	if (Application::move_keys[Application::Key::E])
		position_.y += move_offset;
}
