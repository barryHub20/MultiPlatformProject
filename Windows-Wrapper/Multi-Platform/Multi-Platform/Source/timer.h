#ifndef _TIMER_H
#define _TIMER_H

/********************************************************************************************************
Put winsock2 here as it needs to be build before windows.h as windows.h
contains winsock.h (OLD)
********************************************************************************************************/
#include <winsock2.h>
#include <windows.h>

class StopWatch
{
 
 private:
     
    LARGE_INTEGER frequency;
    LARGE_INTEGER prevTime, currTime;
    double LIToSecs( LARGE_INTEGER & L) ;
    UINT     wTimerRes;

 public:
     StopWatch() ;
     ~StopWatch();
     void startTimer();
     double getElapsedTime(); // get time in seconds since the last call to this function
     void waitUntil(long long time);  // wait until this time in milliseconds has passed
 };


#endif // _TIMER_H