#pragma once

#include "be_mathsTypes.hpp"

#include <array>
#include <string>

namespace be{

/**
 * A class representing a 3d vector of floating points 
*/
class Vector3{
    protected:
        /**
         * The array containing the values
        */
        std::array<float, 3> _Values = {0.f, 0.f, 0.f};

    public:
        /**
         * An empty constructor
        */
        Vector3();

        /**
         * A basic constructor
         * @param v The value for all the elements in the vector
        */
        Vector3(float v);

        /**
         * A basic constructor
         * @param v The values for the first 2 elements
         * @param z The third value
        */
        Vector3(const Vector2& v, float z);

        /**
         * A basic constructor
         * @param x The first value
         * @param y The second value
         * @param z The third value
        */
        Vector3(float x, float y, float z);

        /**
         * Create a vector fill with ones
         * @return The new vector
        */
        static Vector3 ones();

        /**
         * Create a vector fill with zeros
         * @return The new vector
        */
        static Vector3 zeros();

        /**
         * Create a random vector
         * @return The new vector
        */
        static Vector3 random();

        /**
         * Create a random vector
         * @param min The minimum value
         * @param max The maximum mvalue
         * @return The new vector
        */
        static Vector3 random(float min, float max);

        /**
         * Return true if the current vector is zero
         * @see Maths::isZero
        */
        bool isZero() const;

        /**
         * Cast the vector into a string
         * @return The string
        */
        const std::string toString() const;

        /**
         * Get the x value
         * @return x
        */
        float x() const;

        /**
         * Get the y value
         * @return y
        */
        float y() const;

        /**
         * Get the z value
         * @return z
        */
        float z() const;

        /**
         * Set the x value
         * @param x The new value
        */
        void x(float x);

        /**
         * Set the y value
         * @param y The new value
        */
        void y(float y);

        /**
         * Set the z value
         * @param z The new value
        */
        void z(float z);

        /**
         * Get the r value
         * @return r
        */
        float r() const;

        /**
         * Get the g value
         * @return g
        */
        float g() const;

        /**
         * Get the b value
         * @return b
        */
        float b() const;

        /**
         * Set the r value
         * @param r The new value
        */
        void r(float r);

        /**
         * Set the g value
         * @param g The new value
        */
        void g(float g);

        /**
         * Set the b value
         * @param b The new value
        */
        void b(float b);        

        /**
         * Getter for the vectors elements
         * @param index The index of the element to access
         * @return The wanted value
        */
        float operator[](int index) const;

        /**
         * Setter for the vectors elements
         * @param index The index of the element to set
         * @return The wanted value
        */
        float& operator[](int index);

        /**
         * Addition between two vectors
         * @param vector The second vertex
         * @return The sum of the two vectors
        */
        Vector3 operator+(const Vector3& vector) const;

        /**
         * Substraction between two vectors
         * @param vector The second vertex
         * @return The substraction of the two vectors
        */
        Vector3 operator-(const Vector3& vector) const;

        /**
         * Multiplication with a scalar
         * @param scalar The scalar
         * @return A new vector which is the results of the multiplication
        */
        Vector3 operator*(float scalar) const;

        /**
         * Multiplication with a scalar
         * @param scalar The scalar
         * @param vector The vector
         * @return A new vector which is the results of the multiplication
        */
        friend Vector3 operator*(float scalar, const Vector3& vector){
            return vector * scalar;
        }

        /**
         * Division with a scalar
         * @param scalar The scalar
         * @return A new vector which is the results of the division
        */
        Vector3 operator/(float scalar) const;

        /**
         * Addition between two vectors
         * @param vector The second vector
        */
        void operator+=(const Vector3& vector);

        /**
         * Substraction between two vectors
         * @param vector The second vector
        */
        void operator-=(const Vector3& vector);

        /**
         * Negate a vector
         * @return The negated vector
        */
        Vector3 operator-(void);

        /**
         * Multiplication with a scalar
         * @param scalar The scalar
        */
        void operator*=(float scalar);

        /**
         * Division with a scalar
         * @param scalar The scalar
        */
        void operator/=(float scalar);

        /**
         * Redefine the equality operator
         * @param vector The second vector
         * @return True if vectors are equal
        */
        bool operator==(const Vector3& vector) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix3x2 operator*(const Matrix1x2& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix3x3 operator*(const Matrix1x3& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix3x4 operator*(const Matrix1x4& matrix) const;

        /**
         * Component wise ultiplication with a vector
         * @param vector The second vector
         * @return A new vector which is the results of the multiplication
        */
        Vector3 operator*(const Vector3& vector) const;

        /**
         * Component wise multiplication between two vectors
         * @param vector The second vector
        */
        void operator*=(const Vector3& vector);

        static Vector3 mix(const Vector3& v1, const Vector3& v2, float a);

        /**
         * Dot product
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The dot product
        */
        static float dot(const Vector3& v1, const Vector3& v2);

        /**
         * Dot product
         * @param vector The vector for which to get the dot product
         * @return The dot product
        */
        float dot(const Vector3& vector) const;

        /**
         * Cross product
         * @param v1 The first vector
         * @param v2 The second vector
         * @return The new vector product
        */
        static Vector3 cross(const Vector3& v1, const Vector3& v2);

        /**
         * Cross product
         * @param vector The vector with which do the cross product
        */
        void cross(const Vector3& vector);

        /**
         * Get the vector's norm
         * @return The norm
        */
        float getNorm() const;

        /**
         * Get the vector's squared norm
         * @return The squared norm
        */
        float getSquaredNorm() const;

        /**
         * Normalize the vector
         * @param vector The vector to normalize
         * @return The normalized vector
        */
        static Vector3 normalize(const Vector3& vector);

        /**
         * Normalize the vector
        */
        void normalize();
};

}