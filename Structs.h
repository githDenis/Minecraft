#pragma once

struct Color
{
	float red;
	float green;
	float blue;
};

struct Rotator
{
	float yaw;
	float pitch;
	float roll;
};

struct UV
{
	float u0;
	float v0;
	float u1;
	float v1;

	UV& operator=(const UV& other) noexcept
	{
		u0 = other.u0;
		v0 = other.v0;
		u1 = other.u1;
		v1 = other.v1;
		return *this;
	}
};