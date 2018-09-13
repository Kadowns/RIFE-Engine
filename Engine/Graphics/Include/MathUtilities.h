#include <algorithm>

namespace math {
	template<class T>
	const T& clamp(const T& x, const T& upper, const T& lower) {
		return std::min(upper, std::max(x, lower));
	}
}
