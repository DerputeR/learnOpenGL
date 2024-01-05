#pragma once
// http://www.suodenjoki.dk/us/archive/2010/min-max.htm
//#define NOMINMAX
#include <algorithm>

// https://stackoverflow.com/questions/9323903/most-efficient-elegant-way-to-clip-a-number
template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

template <typename T>
T wrap(const T& n, const T& lower, const T& upper) {
	T m = n;
	while (m < lower) {
		m = m + (upper - lower);
	}
	while (m >= upper) {
		m = m - (upper - lower);
	}
	return m;
}