/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// The rtc is using RTC which is shared with us ticker
#include "rtc_api.h"
#include "us_ticker_api.h"
#include <time.h>

/**
 * We need this value as "ticks_per_second" to update
 * the time of rtc correctly
 */
#define RTC_CLOCK_FREQ          (uint32_t)(32768)
#define RTC_COUNTER_SIZE        (uint32_t)(0x1000000)
#define RTC_COUNTER_MASK        (uint32_t)(0x0FFFFFF)
#define RTC_COUNTER_SIZE_SEC    (RTC_COUNTER_SIZE / RTC_CLOCK_FREQ)

extern uint32_t rtc1_getCounter(void);
extern void rtc1_stop(void);

static uint32_t rtc_prv;
static time_t   time_base;

static inline void rtc_update(void)
{
    uint32_t rtc_now = rtc1_getCounter() / RTC_COUNTER_SIZE_SEC;
    uint32_t dlt_sec = ( ( rtc_now + RTC_COUNTER_SIZE_SEC ) - rtc_prv ) % RTC_COUNTER_SIZE_SEC;

    time_base = time_base + (time_t)dlt_sec;
    rtc_prv = rtc_now;
}


void rtc_init(void)
{
    us_ticker_init();
}

/**
 * @brief Function for stopping the RTC1 timer. We don't expect to call this.
 */
void rtc_free(void)
{
    rtc1_stop();
}

time_t rtc_read(void)
{
    rtc_update();
    return time_base;
}

/**
 * @brief Updates the time_base of the rtc to the given value
 */
void rtc_write(time_t t)
{
    // set the current time from a parameter
    time_base   = t;
    rtc_prv     = rtc1_getCounter() / ticks_per_second;
}
