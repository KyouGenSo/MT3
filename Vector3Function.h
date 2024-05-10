#pragma once
#include<Vector3.h>
#include<math.h>
#include<Novice.h>

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