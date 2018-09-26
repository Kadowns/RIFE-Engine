#include <Types.h>
#include <math.h>
#include <algorithm>

namespace rm {

    template<class T>
    const T& clamp(const T& x, const T& upper, const T& lower) {
        return std::min(upper, std::max(x, lower));
    }

    class vec2f {
    protected:

        float64 m_x, m_y;

    public:	
        //Constructor
        vec2f();
        vec2f(const vec2f &vec);
        vec2f(float64 x, float64 y);


        //Getters
        float64 x() { return m_x; }
        float64 y() { return m_y; }
        vec2f xy() { return *this; }
        float64 magnitude();
        float64 dot(vec2f vec);
        vec2f cross(vec2f vec);
        vec2f normalized();

        //Methods
        void normalize();
        

        //Operators
        vec2f operator/(float64 scalar);
        void operator/=(float64 scalar);

    };

    class vec3f : vec2f {
    protected:

        float64 m_z;

    public:

        //Constructors
        vec3f(const vec3f &vec);
        vec3f(const vec2f &vecxy, float64 z);
        vec3f(float64 x, float64 y, float64 z);
        vec3f();


        //Getters
        float64 z() { return m_z; }
        vec2f xz() { return vec2f(m_x, m_z); }
        vec2f yz() { return vec2f(m_y, m_z); }
        vec3f xyz() { return *this; }
    };
}