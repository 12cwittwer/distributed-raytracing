#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <cuda_runtime.h>

class vec3 {
public:
    double e[3];

    __host__ __device__ vec3() : e{0, 0, 0} {}
    __host__ __device__ vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    __host__ __device__ double x() const { return e[0]; }
    __host__ __device__ double y() const { return e[1]; }
    __host__ __device__ double z() const { return e[2]; }

    __host__ __device__ vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    __host__ __device__ double operator[](int i) const { return e[i]; }
    __host__ __device__ double& operator[](int i) { return e[i]; }

    __host__ __device__ vec3& operator+=(const vec3& v) {
        e[0] += v.e[0]; e[1] += v.e[1]; e[2] += v.e[2];
        return *this;
    }

    __host__ __device__ vec3& operator*=(double t) {
        e[0] *= t; e[1] *= t; e[2] *= t;
        return *this;
    }

    __host__ __device__ vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    __host__ __device__ double length() const {
        return sqrt(length_squared());
    }

    __host__ __device__ double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
};

using point3 = vec3;

__host__ __device__ inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u[0]+v[0], u[1]+v[1], u[2]+v[2]);
}

__host__ __device__ inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u[0]-v[0], u[1]-v[1], u[2]-v[2]);
}

__host__ __device__ inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u[0]*v[0], u[1]*v[1], u[2]*v[2]);
}

__host__ __device__ inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v[0], t*v[1], t*v[2]);
}

__host__ __device__ inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

__host__ __device__ inline vec3 operator/(const vec3& v, double t) {
    return (1 / t) * v;
}

__host__ __device__ inline double dot(const vec3& u, const vec3& v) {
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

__host__ __device__ inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(
        u[1]*v[2] - u[2]*v[1],
        u[2]*v[0] - u[0]*v[2],
        u[0]*v[1] - u[1]*v[0]
    );
}

__host__ __device__ inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

#endif
