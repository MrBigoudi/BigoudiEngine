#pragma once

#include <array>
#include <string>

#include "mathsTypes.hpp"

namespace beMaths{

/**
 * A class representing a 1x3 floating point matrix
*/
class Matrix1x3f{
    private:
        /**
         * The array containing the values
        */
        std::array<float, 3> _Values = {0.f, 0.f, 0.f};

    public:
        /**
         * An empty constructor
        */
        Matrix1x3f();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix1x3f(float v);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix1x3f(const std::array<float, 3>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix1x3f ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix1x3f zeros();

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
 * A class representing a 1x3 integer matrix
*/
class Matrix1x3i{
    private:
        /**
         * The array containing the values
        */
        std::array<int, 3> _Values = {0, 0, 0};

    public:
        /**
         * An empty constructor
        */
        Matrix1x3i();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix1x3i(int v);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix1x3i(const std::array<int, 3>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix1x3i ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix1x3i zeros();

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