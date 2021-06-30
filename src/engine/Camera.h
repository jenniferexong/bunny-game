#pragma once

#include <glm/glm.hpp>

#include "FrustumBox.h"
#include "util/Log.h"

using glm::vec3;

class Camera {
protected:
	FrustumBox view_frustum_box_;

    /**
     * Updates the view frustum
     */
	void updateView() { view_frustum_box_.update(*this); }

	vec3 position_ = vec3(0);
	vec3 rotation_ = vec3(0, 0.1f, 0); // yaw, pitch, roll

public:
	vec3 getPosition() const { return position_; }
	vec3 getRotation() const { return rotation_; }
	bool canSeePoint(vec3 point) { return view_frustum_box_.containsPoint(point); }

    void positionForReflection(float water_height);
    void positionForRefraction(float water_height);

    virtual ~Camera() = default;

    virtual void print() {
        printf(
            "Camera: x: %.1f, y: %.1f, z: %.1f, y: %.1f, p: %.1f, r: %.1f\n",
            position_.x, position_.y, position_.z, 
            rotation_.x, rotation_.y, rotation_.z
        );
    }
};
