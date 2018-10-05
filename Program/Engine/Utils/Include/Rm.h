#include <Types.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <Eigen/Dense>

#define DEG_TO_RAD (M_PI / 180)
#define RAD_TO_DEG (180 / M_PI)


namespace rm {

    template<class T>
    const T& clamp(const T& x, const T& upper, const T& lower) {
        return std::min(upper, std::max(x, lower));
    }

	template<class T>
	Eigen::Matrix<T, 4, 4> perspective( double fovy, double aspect, double zNear, double zFar) {
		typedef Eigen::Matrix<T, 4, 4> Matrix4;

		assert(aspect > 0);
		assert(zFar > zNear);
  
        T h = (T)tan((double)(fovy / T(2)));
   
        Matrix4 res = Matrix4::Zero();
        res(0, 0) = T(1) / (h * aspect);
        res(1, 1) = T(1) / h;
        res(2, 2) = -(zFar + zNear) / (zFar - zNear);
        res(2, 3) = -T(1);
        res(3, 2) = -(T(2) * zFar * zNear) / (zFar - zNear);
       
        return res;
	}

	template<class T>
	Eigen::Matrix<T, 4, 4> lookAt(Eigen::Matrix<T, 3, 1> const & eye,
		Eigen::Matrix<T, 3, 1> const &center, Eigen::Matrix<T, 3, 1> const & up) {
		typedef Eigen::Matrix<T, 4, 4> Matrix4;
		typedef Eigen::Matrix<T, 3, 1> Vector3;

        Vector3 X, Y, Z;

        Z = (eye - center).normalized();
        Y = up.normalized();
        X = Y.cross(Z);
        Y = Z.cross(X);
        X.normalize();
        Y.normalize();

        Matrix4 matrix;

        matrix(0, 0) = X.x();
        matrix(1,0) = X.y();
        matrix(2,0) = X.z();
        matrix(3,0) = -X.dot(eye);
        matrix(0,1) = Y.x();
        matrix(1,1) = Y.y();
        matrix(2,1) = Y.z();
        matrix(3,1) = -Y.dot(eye);
        matrix(0,2) = Z.x();
        matrix(1,2) = Z.y();
        matrix(2,2) = Z.z();
        matrix(3,2) = -Z.dot(eye);
        matrix(0,3) = T(0);
        matrix(1,3) = T(0);
        matrix(2,3) = T(0);
        matrix(3,3) = T(1);

		return matrix;
	}
};