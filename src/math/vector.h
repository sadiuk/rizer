#include <cassert>

template<typename T>
struct unaligned_vector3
{
	T x, y, z;
	T& operator[](size_t index)
	{
		if (index == 0) return x;
		else if (index == 1) return y;
		else if (index == 2) return z;
		assert(false);
	}
};

template<typename T>
struct alignas(16) vector3 : unaligned_vector3<T> {};


struct alignas(16) vector4
{
	float x, y, z, w;
};