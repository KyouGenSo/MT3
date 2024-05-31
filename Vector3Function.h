#pragma once
#include<Vector3.h>
#include<math.h>
#include<Novice.h>

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

void Vector3ScreenPrint(int x, int y, Vector3 v, const char* string) {
	Novice::ScreenPrintf(x, y, "%s", string);
	Novice::ScreenPrintf(x + 0 * kColumnWidth, y + kRowHeight, "%6.02f", v.x);
	Novice::ScreenPrintf(x + 1 * kColumnWidth, y + kRowHeight, "%6.02f", v.y);
	Novice::ScreenPrintf(x + 2 * kColumnWidth, y + kRowHeight, "%6.02f", v.z);
}

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 Multiply(const Vector3& v, float scaler) {
	return Vector3(v.x * scaler, v.y * scaler, v.z * scaler);
}

float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double Length(const Vector3& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
	double length = Length(v);
	return Vector3(v.x / (float)length, v.y / (float)length, v.z / (float)length);
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

float Distance(const Vector3& v1, const Vector3& v2) {
	return float(Length(Subtract(v2, v1)));
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	return Multiply(Normalize(v2), Dot(v1, Normalize(v2)));
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
		return Add(segment.origin, project);
}
