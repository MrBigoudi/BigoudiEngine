#pragma once

#include <array>
#include <string>

#include "mathsTypes.hpp"

namespace beMaths{

/**
 * A class representing a 1x3 floating point matrix
*/
class Matrix1x3{
    private:
        /**
         * The array containing the values
        */
        std::array<float, 3> _Values = {0.f, 0.f, 0.f};

    public:
        /**
         * An empty constructor
        */
        Matrix1x3();

        /**
         * A basic constructor
         * @param v The value for all the elements in the matrix
        */
        Matrix1x3(float v);

        /**
         * A basic constructor
         * @param values The array containing the values
        */
        Matrix1x3(const std::array<float, 3>& values);

        /**
         * Create a matrix fill with ones
        */
        static Matrix1x3 ones();

        /**
         * Create a matrix fill with zeros
        */
        static Matrix1x3 zeros();

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