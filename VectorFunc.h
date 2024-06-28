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

void Vector3ScreenPrint(int x, int y, Vector3 v, const char* string);

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(const Vector3& v, float scaler);

float Dot(const Vector3& v1, const Vector3& v2);

double Length(const Vector3& v);

Vector3 Normalize(const Vector3& v);

Vector3 Cross(const Vector3& v1, const Vector3& v2);

float Distance(const Vector3& v1, const Vector3& v2);

Vector3 Project(const Vector3& v1, const Vector3& v2);

Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

Vector3 Max(const Vector3& v1, const Vector3& v2);

Vector3 Min(const Vector3& v1, const Vector3& v2);

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

Vector3 CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);
