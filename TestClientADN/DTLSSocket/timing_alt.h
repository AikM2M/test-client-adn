/**
 * \file timing.h
 *
 * \brief Portable interface to timeouts and to the CPU cycle counter
 */
/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
//#ifndef MBEDTLS_TIMING_H
//#define MBEDTLS_TIMING_H

//#if !defined(MBEDTLS_CONFIG_FILE)
//#include "config.h"
//#else
//#include MBEDTLS_CONFIG_FILE
//#endif

#include <stdint.h>

//#ifdef __cplusplus
//extern "C" {
//#endif

// Regular implementation
//

/**
 * \brief          timer structure
 */
/*struct mbedtls_timing_hr_time
{
    unsigned char opaque[32];
}; */ 

#include "mbed.h"
//#include <Timer.h>

//Timer       *timer;
//uint32_t    start, now;
/**
 * \brief          Context for mbedtls_timing_set/get_delay()
 */
//timer->start();
/*
struct dtls_timer_context {
    Timer       *timer; 
    uint32_t    int_ms;
    uint32_t    fin_ms;
    uint32_t    now, start;
}; */

//Timer    timer;

typedef struct mbedtls_timing_delay_context
{
    uint32_t                                                    now, ref;
    //mbedtls_timing_hr_time            _hr_time;
    uint32_t                            int_ms;
    uint32_t                            fin_ms;
};


//extern volatile int mbedtls_timing_alarmed;

/**
 * \brief          Return the CPU cycle counter value
 *
 * \warning        This is only a best effort! Do not rely on this!
 *                 In particular, it is known to be unreliable on virtual
 *                 machines.
 *
 * \note           This value starts at an unspecified origin and
 *                 may wrap around.
 */
//unsigned long mbedtls_timing_hardclock( void ){
    
//};

/**
 * \brief          Return the elapsed time in milliseconds
 *
 * \param val      points to a timer structure
 * \param reset    If 0, query the elapsed time. Otherwise (re)start the timer.
 *
 * \return         Elapsed time since the previous reset in ms. When
 *                 restarting, this is always 0.
 *
 * \note           To initialize a timer, call this function with reset=1.
 *
 *                 Determining the elapsed time and resetting the timer is not
 *                 atomic on all platforms, so after the sequence
 *                 `{ get_timer(1); ...; time1 = get_timer(1); ...; time2 =
 *                 get_timer(0) }` the value time1+time2 is only approximately
 *                 the delay since the first reset.
 */
/*unsigned long mbedtls_timing_get_timer( mbedtls_timing_hr_time *val, int reset ){
    if (reset) {
        
    }
    else {
        
    }  
}; */

/**
 * \brief          Setup an alarm clock
 *
 * \param seconds  delay before the "mbedtls_timing_alarmed" flag is set
 *                 (must be >=0)
 *
 * \warning        Only one alarm at a time  is supported. In a threaded
 *                 context, this means one for the whole process, not one per
 *                 thread.
 */
//void mbedtls_set_alarm( int seconds );

/**
 * \brief          Set a pair of delays to watch
 *                 (See \c mbedtls_timing_get_delay().)
 *
 * \param data     Pointer to timing data.
 *                 Must point to a valid \c mbedtls_timing_delay_context struct.
 * \param int_ms   First (intermediate) delay in milliseconds.
 *                 The effect if int_ms > fin_ms is unspecified.
 * \param fin_ms   Second (final) delay in milliseconds.
 *                 Pass 0 to cancel the current delay.
 *
 * \note           To set a single delay, either use \c mbedtls_timing_set_timer
 *                 directly or use this function with int_ms == fin_ms.
 */
// set delays to watch 
void mbedtls_timing_set_delay( void *data, uint32_t int_ms, uint32_t fin_ms ){

        mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;
        //dtls_timer_context *ctx = (dtls_timer_context *) data;
  
        ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;
    
    //printf("\nInitial delay time (ms) set: ctx->int_ms = %d\n", ctx->int_ms);
    //printf("\nFinish delay time (ms) set: ctx->fin_ms = %d\n", ctx->fin_ms);
    
    //timer.start();
    //printf("\ntimer.start() executed and timer.read_ms() = %d\n", timer.read_ms());
    
    if( fin_ms != 0 ) {
        // reset = 1; get_timer(data, 1)
        // ref/start = put clocks
        ctx->ref = osKernelGetTickCount();
        //printf("\nReference time set: ctx->ref = %d\n", ctx->ref);   
    }
};

/**
 * \brief          Get the status of delays
 *                 (Memory helper: number of delays passed.)
 *
 * \param data     Pointer to timing data
 *                 Must point to a valid \c mbedtls_timing_delay_context struct.
 *
 * \return         -1 if cancelled (fin_ms = 0),
 *                  0 if none of the delays are passed,
 *                  1 if only the intermediate delay is passed,
 *                  2 if the final delay is passed.
 */
int mbedtls_timing_get_delay( void *data ){
    
        mbedtls_timing_delay_context *ctx = (mbedtls_timing_delay_context *) data;
    //dtls_timer_context *ctx = (dtls_timer_context *) data;
    
        unsigned long elapsed_ms;

    if( ctx->fin_ms == 0 ) {
        //printf("\n Timer has been reset, returned -1\n");
        return( -1 );
    }

    ctx->now = osKernelGetTickCount();
        elapsed_ms = ((ctx->now - ctx->ref) / osKernelGetTickFreq()) * 1000;
    //printf("\nThe elapsed delay time (ms) is: elapsed_ms = %d %d\n", elapsed_ms, osKernelGetTickFreq());

    if( elapsed_ms >= ctx->fin_ms ) {
        //printf("\nFlight delay-Time has been over-delayed, returned 2\n");
        return( 2 );
    }

    if( elapsed_ms >= ctx->int_ms ) {
        //printf("\nOnly the intermediate delay is passed, returned 1\n");
        return( 1 );
    }
    
    //printf("\nNo delay is been passed, returned 0\n");
    return( 0 );
}; 

//#if defined(MBEDTLS_SELF_TEST)
/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if a test failed
 */
//int mbedtls_timing_self_test( int verbose );
//#endif

//#ifdef __cplusplus
//}
//#endif

//#endif /* timing.h */
