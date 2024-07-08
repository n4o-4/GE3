#pragma once

struct Vector2 {
	float x;
	float y;

	Vector2(float x = 0,float y = 0) : x(x), y(y){}

	Vector2 operator+=(const Vector2& v) { this->x += v.x; this->y += v.y; return *this; }
};

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

struct Vector4 {
	float x;
	float y;
	float z;
	float w;

   //Vector4(float x = 0, float y = 0, float z = 0,float w = 0) : x(x), y(y), z(z), w(0) {}
};