#pragma once

#include <array>
#include <string>

#include "mathsTypes.hpp"

namespace beMaths{

/**
 * A class representing a 1x4 floating point matrix
*/
class Matrix1x4f{
    private:
        /**
         * The array containing the values
        */
        std::array<float, 4> _Values = {0.f, 0.f, 0.f, 0.f};

    public:
        /**
         * An empty constructor
        */
        Matrix1x4f();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix1x4f(float v);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix1x4f(const std::array<float, 4>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix1x4f ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix1x4f zeros();

        /**
         * Cast the matrix into a string
         * @return The string
        */
        const std::string toString() const;

        /**
         * Getter for the matrices elements
         * @param index The index of the element to access
         * @return The wanted value
        */
        float operator[](int index) const;

        /**
         * Setter for the matrices elements
         * @param index The index of the element to set
         * @return The wanted value
        */
        float& operator[](int index);
};

}


namespace beMaths{

/**
 * A class representing a 1x4 integer matrix
*/
class Matrix1x4i{
    private:
        /**
         * The array containing the values
        */
        std::array<int, 4> _Values = {0, 0, 0, 0};

    public:
        /**
         * An empty constructor
        */
        Matrix1x4i();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix1x4i(float v);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix1x4i(const std::array<int, 4>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix1x4i ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix1x4i zeros();

        /**
         * Cast the matrix into a string
         * @return The string
        */
        const std::string toString() const;

        /**
         * Getter for the matrices elements
         * @param index The index of the element to access
         * @return The wanted value
        */
        int operator[](int index) const;

        /**
         * Setter for the matrices elements
         * @param index The index of the element to set
         * @return The wanted value
        */
        int& operator[](int index);
};

}