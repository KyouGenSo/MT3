#pragma once
#include<Vector3.h>

class MassPoint {
public:
	MassPoint(Vector3 initPosition, float initMass, float initRadius) {
		position_ = initPosition;
		mass_ = initMass;
		radius_ = initRadius;
	}

	// getter
	const Vector3 GetPosition() { return position_; }
	const Vector3 GetVelocity() { return velocity_; }
	const Vector3 GetTotalForce() { return totalForce_; }

	const float GetMass() { return mass_; }
	const float GetRadius() { return radius_; }

	// setter
	void SetPosition(Vector3 pos) { position_ = pos; }
	void SetVelocity(Vector3 vel) { velocity_ = vel; }
	void SetTotalForce(Vector3 total) { totalForce_ = total; }

	void SetMass(float m) { mass_ = m; }
	void SetRadius(float r) { radius_ = r; }

private:
	Vector3 position_;
	Vector3 velocity_;
	Vector3 totalForce_;

	float mass_;
	float radius_;
};