#pragma once

namespace beCore{

/**
 * @enum The level of the error
*/
enum ErrorLevel{
    FATAL,
    WARNING,
};

/**
 * @enum The different error codes
*/
enum ErrorCode{
    NO_ERROR,
    GL_ERROR,
    IO_ERROR,
    INIT_FAILURE,
    BAD_VALUE,
};

/**
 * The class to handle errors
*/
class ErrorHandler{
    private:
        /**
         * Private constructor to make the class purely virtual
        */
        ErrorHandler();

    public:
        /**
         * Handle the error
         * @param error The error to handle
         * @param level The error level
        */
        static void handle(ErrorCode error, ErrorLevel level = FATAL);

        /**
         * Handle an OpenGL error
         * @param format The error message
        */
        static void handleGL(const char* format, ...);

};

}