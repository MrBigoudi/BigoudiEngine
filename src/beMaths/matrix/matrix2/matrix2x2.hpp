#pragma once

#include <array>
#include <string>

#include "mathsTypes.hpp"

namespace beMaths{

/**
 * A class representing a 2x2 floating point matrix
*/
class Matrix2x2{
    private:
        /**
         * The array containing the values
        */
        std::array<std::array<float,2>, 2> _Values = {{0.f, 0.f}, {0.f, 0.f}};

    public:
        /**
         * An empty constructor
        */
        Matrix2x2();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix2x2(float v);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix2x2(const std::array<std::array<float,2>, 2>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix2x2 ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix2x2 zeros();

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