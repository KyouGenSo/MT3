#pragma once
#include<Vector3.h>

struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};

struct Sphere {
	Vector3 center;
	float radius;
};

struct Plane {
	Vector3 normal;
	float distance;
};

struct Triangle {
	Vector3 vertex[3];
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

struct OBB {
	Vector3 center;
	Vector3 axis[3];
	Vector3 size;
};

struct MassPoint {
	Vector3 position_;
	Vector3 velocity_;
	Vector3 Acceleration;
	float mass_;
	float radius_;
	unsigned int color;
};

struct Spring {
	Vector3 anchor;
	float natrualLength;
	float stiffness;
	float damping;
};