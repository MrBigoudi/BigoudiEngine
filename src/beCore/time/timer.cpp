#include "timer.hpp"
#include "errorHandler.hpp"
#include <cstdio>
#include <chrono>

namespace beCore{

/**
 * A basic constructor
*/
Timer::Timer(){}

/**
 * Get the state of the timer
*/
TimerState Timer::getState() const {
    return _State;
}

/**
 * Start the timer
*/
void Timer::start(){
    switch(_State){
        case ON_GOING:
            fprintf(stderr, "The timer is already running!\n");
            ErrorHandler::handle(ErrorCode::BAD_VALUE, ErrorLevel::WARNING);
            break;
        case PAUSED:
            fprintf(stderr, "The timer is on pause!\n");
            ErrorHandler::handle(ErrorCode::BAD_VALUE, ErrorLevel::WARNING);
            break;
        case STOPPED:
            _State = ON_GOING;
            _StartTicks = getCurrentTime();
            _PauseTicks = 0;
            break;
    }
}

/**
 * Pause the timer
*/
void Timer::pause(){
    switch(_State){                
        case ON_GOING:
            _State = PAUSED;
            _PauseTicks = getCurrentTime() - _StartTicks;
            _StartTicks = 0;
            break;
        case PAUSED:
            fprintf(stderr, "The timer is already on pause!\n");
            ErrorHandler::handle(ErrorCode::BAD_VALUE, ErrorLevel::WARNING);
            break;
        case STOPPED:
            fprintf(stderr, "The timer is stopped!\n");
            ErrorHandler::handle(ErrorCode::BAD_VALUE, ErrorLevel::WARNING);
            break;
    }
}

/**
 * Unpause the timer
*/
void Timer::unpause(){
    switch(_State){                
        case ON_GOING:
            fprintf(stderr, "The timer is running!\n");
            ErrorHandler::handle(ErrorCode::BAD_VALUE, ErrorLevel::WARNING);
            break;
        case PAUSED:
            _State = ON_GOING;
            _StartTicks = getCurrentTime() - _PauseTicks;
            _StartTicks = 0;
            break;
        case STOPPED:
            fprintf(stderr, "The timer is stopped!\n");
            ErrorHandler::handle(ErrorCode::BAD_VALUE, ErrorLevel::WARNING);
            break;
    }
}

/**
 * Stop the timer
*/
void Timer::stop(){
    switch(_State){
        default:
            _State = STOPPED;
            _StartTicks = 0;
            _PauseTicks = 0;
    }
}

/**
 * Get the ellapsed time when the timer was running
*/
unsigned int Timer::getTicks() const {
    switch(_State){
        case ON_GOING:
            return getCurrentTime() - _StartTicks;
        case PAUSED:
            return _PauseTicks;
        case STOPPED:
            fprintf(stderr, "The timer is stopped!\n");
            ErrorHandler::handle(ErrorCode::BAD_VALUE, ErrorLevel::WARNING);
            break;
    }
    return 0;
}

/**
 * Helper function to get current time in milliseconds 
 * @return The time in milliseconds
*/
unsigned int Timer::getCurrentTime() const {
    using namespace std::chrono;
    auto timeSinceEpoch = high_resolution_clock::now().time_since_epoch();
    auto timeSinceEpochMilliseconds = duration_cast<milliseconds>(timeSinceEpoch).count();
    return static_cast<unsigned int>(timeSinceEpochMilliseconds);
}

}