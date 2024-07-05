#pragma once
#include<Vector3.h>
#include"MassPoint.h"

class Spring {
public:
	Spring(MassPoint* p1, MassPoint* p2, float naturalLength, float stiffness, float damping) {
		p1_ = p1;
		p2_ = p2;
		naturalLength_ = naturalLength;
		stiffness_ = stiffness;
		damping_ = damping;
	}

	// getter
	const float GetNaturalLength() { return naturalLength_; }
	const float GetStiffness() { return stiffness_; }
	const float GetDamping() { return damping_; }

	// setter
	void SetNaturalLength(float naturalLength) { naturalLength_ = naturalLength; }
	void SetStiffness(float stiffness) { stiffness_ = stiffness; }
	void SetDamping(float damping) { damping_ = damping; }

private:
	MassPoint* p1_;
	MassPoint* p2_;

	float naturalLength_;
	float stiffness_;
	float damping_;
};
