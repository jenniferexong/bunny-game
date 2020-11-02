#include "Camera.h"
#include "../Application.h"

const float Camera::s_move_offset = 0.05f;

void Camera::updatePosition() {
	// Moving the camera if the movements keys are currently down
	if (Application::s_move_keys['w']) 
		m_position.z -= s_move_offset;

	if (Application::s_move_keys['a']) 
		m_position.x -= s_move_offset;

	if (Application::s_move_keys['s']) 
		m_position.z += s_move_offset;

	if (Application::s_move_keys['d'])
		m_position.x += s_move_offset;

	if (Application::s_move_keys['q'])
		m_position.y -= s_move_offset;
	
	if (Application::s_move_keys['e'])
		m_position.y += s_move_offset;
}
