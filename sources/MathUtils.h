#include <W4Framework.h>

constexpr long double deg2rad(long double deg)
{
	return deg * w4::math::DEG2RAD;
}

constexpr long double rad2deg(long double rad)
{
	return rad / w4::math::DEG2RAD;
}

constexpr long double operator"" _deg(long double deg)
{
	return deg2rad(deg);
}

constexpr long double operator"" _deg(uint64_t deg)
{
	return deg2rad(deg);
}