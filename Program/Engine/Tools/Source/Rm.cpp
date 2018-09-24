#include "Rm.h"

rm::vec2f::vec2f() :m_x(0), m_y(0){}

rm::vec2f::vec2f(const vec2f &vec) : m_x(vec.m_x), m_y(vec.m_y){}

rm::vec2f::vec2f(float64 x, float64 y) : m_x(x), m_y(y){}

float64 rm::vec2f::magnitude() { return sqrt(m_x * m_x + m_y * m_y); }

float64 rm::vec2f::dot(vec2f vec) { return m_x * vec.m_x + m_y * vec.m_y; }

rm::vec2f rm::vec2f::cross(vec2f vec) {	return vec2f(m_y, -m_x); }

rm::vec2f rm::vec2f::normalized() { return *this / magnitude(); }

void rm::vec2f::normalize() { *this = normalized(); }

rm::vec2f rm::vec2f::operator/(float64 scalar) { return vec2f(m_x / scalar, m_y / scalar); }

void rm::vec2f::operator/=(float64 scalar) { *this = *this / scalar; }

rm::vec3f::vec3f(const vec3f &vec) : rm::vec2f(vec.m_x, vec.m_y), m_z(vec.m_z) {}

rm::vec3f::vec3f(const vec2f & vecxy, float64 z) : rm::vec2f(vecxy), m_z(z){}

rm::vec3f::vec3f(float64 x, float64 y, float64 z) : rm::vec2f(x, y), m_z(z){}

rm::vec3f::vec3f() : rm::vec2f(), m_z(0){}
