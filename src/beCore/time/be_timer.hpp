#pragma once

#include <memory>

namespace be{

/**
 * Forward declaration for the Timer class
*/
class Timer;

/**
 * A type defining a smart pointer to a timer
*/
using TimerPtr = std::shared_ptr<Timer>;

/**
 * An enum representing the timer states
*/
enum TimerState{
    ON_GOING,
    PAUSED,
    STOPPED,
};

/**
 * A class representing a timer
*/
class Timer{

    private:
        /**
         * The clock time when the time started
        */
        unsigned int _StartTicks = 0;

        /**
         * The ticks stored when the timer was on pause
        */
        unsigned int _PauseTicks = 0;

        /**
         * The time state
        */
        TimerState _State = STOPPED;

    private:
        /**
         * Helper function to get current time in milliseconds 
         * @return The time in milliseconds
        */
        unsigned int getCurrentTime() const;

    public:
        /**
         * A basic constructor
        */
        Timer();

        /**
         * Get the state of the timer
        */
        TimerState getState() const;

        /**
         * Start the timer
        */
        void start();

        /**
         * Pause the timer
        */
        void pause();

        /**
         * Unpause the timer
        */
        void unpause();

        /**
         * Stop the timer
        */
        void stop();

        /**
         * Get the ellapsed time when the timer was running
        */
        unsigned int getTicks() const;

};

}