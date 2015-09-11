#pragma once
#ifndef _Timer_H_
#define _Timer_H_

class Timer {
    const double TICKS_PER_SECOND, ELAPSED_CAP;
    double current, previous, lag, elapsed;
    void( *func_ptr )( );
public:
    Timer( void( *callback )( ), double ticks_per_sec, double elapsed_cap = 0.0 );
    ~Timer();
    void CatchUp();
};

#endif


