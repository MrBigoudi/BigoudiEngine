#pragma once

#include "mathsTypes.hpp"

#include <array>
#include <string>

namespace beMaths{

/**
 * A class representing a 2d vector of floating points 
*/
class Vector2f{
    private:
        /**
         * The array containing the values
        */
        std::array<float, 2> _Values = {0.f, 0.f};

    public:
        /**
         * An empty constructor
        */
        Vector2f();

        /**
         * A basic constructor
         * @param v The value for all the elements in the vector
        */
        Vector2f(float v);

        /**
         * A basic constructor
         * @param x The first value
         * @param y The second value
        */
        Vector2f(float x, float y);

        /**
         * Create a vector fill with ones
        */
        static Vector2f ones();

        /**
         * Create a vector fill with zeros
        */
        static Vector2f zeros();

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
         * Get the u value
         * @return u
        */
        float u() const;

        /**
         * Get the v value
         * @return v
        */
        float v() const;

        /**
         * Set the u value
         * @param u The new value
        */
        void u(float u);

        /**
         * Set the v value
         * @param v The new value
        */
        void v(float v);
        

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
        Vector2f operator+(const Vector2f& vector) const;

        /**
         * Substraction between two vectors
         * @param vector The second vertex
         * @return The substraction of the two vectors
        */
        Vector2f operator-(const Vector2f& vector) const;

        /**
         * Addition between two vectors
         * @param vector The second vector
        */
        void operator+=(const Vector2f& vector);

        /**
         * Substraction between two vectors
         * @param vector The second vector
        */
        void operator-=(const Vector2f& vector);

        /**
         * Multiplication with a scalar
         * @param val The scalar
        */
        void operator*=(float scalar);

        /**
         * Division with a scalar
         * @param val The scalar
        */
        void operator/=(float scalar);

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix2x2f operator*(const Matrix1x2f& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix2x3f operator*(const Matrix1x3f& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix2x4f operator*(const Matrix1x4f& matrix) const;

};

}

namespace beMaths{

/**
 * A class representing a 2d vector of integers
*/
class Vector2i{
    private:
        /**
         * The array containing the values
        */
        std::array<int, 2> _Values = {0, 0};

    public:
        /**
         * An empty constructor
        */
        Vector2i();

        /**
         * A basic constructor
         * @param v The value for all the elements in the vector
        */
        Vector2i(int v);

        /**
         * A basic constructor
         * @param x The first value
         * @param y The second value
        */
        Vector2i(int x, int y);

        /**
         * Create a vector fill with ones
        */
        static Vector2i ones();

        /**
         * Create a vector fill with zeros
        */
        static Vector2i zeros();

        /**
         * Cast the vector into a string
         * @return The string
        */
        const std::string toString() const;

        /**
         * Get the x value
         * @return x
        */
        int x() const;

        /**
         * Get the y value
         * @return y
        */
        int y() const;

        /**
         * Set the x value
         * @param x The new value
        */
        void x(int x);

        /**
         * Set the y value
         * @param y The new value
        */
        void y(int y);

        /**
         * Get the r value
         * @return r
        */
        int r() const;

        /**
         * Get the g value
         * @return g
        */
        int g() const;

        /**
         * Set the r value
         * @param r The new value
        */
        void r(int r);

        /**
         * Set the g value
         * @param g The new value
        */
        void g(int g);

        /**
         * Get the u value
         * @return u
        */
        int u() const;

        /**
         * Get the v value
         * @return v
        */
        int v() const;

        /**
         * Set the u value
         * @param u The new value
        */
        void u(int u);

        /**
         * Set the v value
         * @param v The new value
        */
        void v(int v);
        

        /**
         * Getter for the vectors elements
         * @param index The index of the element to access
         * @return The wanted value
        */
        int operator[](int index) const;

        /**
         * Setter for the vectors elements
         * @param index The index of the element to set
         * @return The wanted value
        */
        int& operator[](int index);

        /**
         * Addition between two vectors
         * @param vector The second vertex
         * @return The sum of the two vectors
        */
        Vector2i operator+(const Vector2i& vector) const;

        /**
         * Substraction between two vectors
         * @param vector The second vertex
         * @return The substraction of the two vectors
        */
        Vector2i operator-(const Vector2i& vector) const;

        /**
         * Addition between two vectors
         * @param vector The second vector
        */
        void operator+=(const Vector2i& vector);

        /**
         * Substraction between two vectors
         * @param vector The second vector
        */
        void operator-=(const Vector2i& vector);

        /**
         * Multiplication with a scalar
         * @param val The scalar
        */
        void operator*=(float scalar);

        /**
         * Division with a scalar
         * @param val The scalar
        */
        void operator/=(float scalar);

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix2x2i operator*(const Matrix1x2i& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix2x3i operator*(const Matrix1x3i& matrix) const;

        /**
         * Multiplication with a matrix
         * @param matrix The matrix
        */
        Matrix2x4i operator*(const Matrix1x4i& matrix) const;

};

}