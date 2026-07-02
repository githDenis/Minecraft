#pragma once
#include "Framework.h"

template <class T>
class Vector
{
private:
	T* ptr;
	unsigned int size;
	unsigned int capacity;

public:
	explicit Vector() noexcept : ptr(nullptr), size(0), capacity(0) {}
	Vector(Vector&& vector) noexcept : ptr(vector.ptr), size(vector.size), capacity(vector.capacity)
	{
		vector.ptr = nullptr;
		vector.size = 0;
		vector.capacity = 0;
	}
	Vector(const Vector& vector) = delete;

	~Vector() noexcept
	{
		if (ptr)
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

	void Add(const T& value) noexcept
	{
		size++;
		if (size >= capacity)
		{
			capacity = capacity == 0 ? 8 : capacity * 2;
			T* newPtr = new T[capacity];

			for (int i = 0; i < size - 1; i++)
			{
				newPtr[i] = ptr[i];
			}
			delete[] ptr;
			ptr = newPtr;
		}
		ptr[size - 1] = value;
	}

	void AddArray(const T* arr, unsigned int arrSize) noexcept
	{
		unsigned int newSize = size + arrSize;

		if (newSize >= capacity)
		{
			capacity = capacity == 0 ? newSize : capacity * 2;
			T* newPtr = new T[capacity];

			memcpy(newPtr, ptr, size * sizeof(T));
			
			delete[] ptr;
			ptr = newPtr;
		}
		for (int i = size; i < newSize; i++)
		{
			ptr[i] = arr[i - size];
		}
		size = newSize;
	}

	void Clear() noexcept
	{
		size = 0;
		capacity = 0;

		if (ptr)
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

	inline T* GetPtr() const noexcept
	{
		return ptr;
	}

	inline unsigned int GetSize() const noexcept
	{
		return size;
	}

	T& operator[](int index) noexcept
	{
		return ptr[index];
	}

	Vector& operator=(Vector&& vector) noexcept
	{
		if (ptr)
		{
			delete[] ptr;
		}
		ptr = vector.ptr;
		size = vector.size;
		capacity = vector.capacity;

		vector.ptr = nullptr;
		vector.size = 0;
		vector.capacity = 0;
		return *this;
	}
};