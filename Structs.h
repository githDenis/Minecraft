#pragma once

struct Color
{
	float red;
	float green;
	float blue;
};

struct Vector3
{
	float x;
	float y;
	float z;

	//void Normilize() noexcept
	//{
	//	float length = sqrt(x * x + y * y + z * z);
	//	x /= length;
	//	y /= length;
	//	z /= length;
	//}

	Vector3& operator=(const Vector3& another) noexcept
	{
		x = another.x;
		y = another.y;
		z = another.z;
		return *this;
	}

	inline Vector3 operator+(const Vector3& another) noexcept
	{
		return Vector3{ x + another.x, y + another.y, z + another.z };
	}

	inline Vector3 operator-(const Vector3& another) noexcept
	{
		return Vector3{ x - another.x, y - another.y, z - another.z };
	}

	inline Vector3 operator*(float num) noexcept
	{
		return Vector3{ x * num, y * num, z * num };
	}
};

struct Vector2
{
	float x;
	float y;

	Vector2& operator=(const Vector2& another) noexcept
	{
		x = another.x;
		y = another.y;
		return *this;
	}

	inline Vector2 operator+(const Vector2& another) noexcept
	{
		return Vector2{ x + another.x, y + another.y };
	}

	inline Vector2 operator-(const Vector2& another) noexcept
	{
		return Vector2{ x - another.x, y - another.y };
	}
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