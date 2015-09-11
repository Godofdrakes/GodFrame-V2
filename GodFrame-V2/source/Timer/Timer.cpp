#include "Timer.h"
#include <GLFW/glfw3.h>

Timer::Timer( double ticks_per_sec, double elapsed_cap ) :
    TICKS_PER_SECOND( 1.0 / ticks_per_sec ),
    ELAPSED_CAP( elapsed_cap ) {
    previous = glfwGetTime();
    lag = current = elapsed = 0.0;
}

void Timer::CatchUp() {
    current = glfwGetTime();
    elapsed = current - previous;
    if ( ELAPSED_CAP > TICKS_PER_SECOND && elapsed > ELAPSED_CAP ) { elapsed = ELAPSED_CAP; }
    previous = current;
    lag += elapsed;
    while ( lag >= TICKS_PER_SECOND ) {
        Logic();
        lag -= TICKS_PER_SECOND;
    }
}