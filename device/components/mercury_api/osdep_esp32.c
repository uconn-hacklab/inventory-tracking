/**
 *  @file osdep_esp32.c
 *  @brief Mercury API - OS-dependent functions for ESP32
 *  @author Kalin Kochnev
 *  @date 8/13/2024
 */

#include "freertos/FreeRTOS.h"
#include <sys/time.h>   // For gettimeofday() and struct timeval
#include <time.h>
#include <stdint.h>
#include "osdep.h"

uint64_t
tmr_gettime()
{
  struct timeval tv;
  uint64_t totalms;

  gettimeofday(&tv, NULL);
  totalms = (((uint64_t)tv.tv_sec) * 1000) + ((uint64_t) tv.tv_usec) / 1000;

  return totalms;
}

uint32_t tmr_gettime_low()
{
  /* Fill in with code that returns the low 32 bits of a millisecond
   * counter. The API will not otherwise interpret the counter value.
   */

  return (tmr_gettime() >>  0) & 0xffffffff;
}

uint32_t tmr_gettime_high()
{
  /* Fill in with code that returns the hugh 32 bits of a millisecond
   * counter. The API will not otherwise interpret the counter value.
   * Returning 0 is acceptable here if you do not have a large enough counter.
   */

  return (tmr_gettime() >> 32) & 0xffffffff;
}

void
tmr_sleep(uint32_t sleepms)
{
  /*
   * Fill in with code that returns after at least sleepms milliseconds
   * have elapsed.
   */
  vTaskDelay(sleepms / portTICK_PERIOD_MS);
}

TMR_TimeStructure
tmr_gettimestructure()
{
  time_t now;
  uint64_t temp;
  struct tm *timestamp;
  TMR_TimeStructure timestructure;

  temp = tmr_gettime();
  now = (time_t)(temp/1000);
  timestamp = localtime(&now);

  timestructure.tm_year = (uint32_t)(1900 + timestamp->tm_year);
  timestructure.tm_mon = (uint32_t)(1 + timestamp->tm_mon);
  timestructure.tm_mday = (uint32_t)timestamp->tm_mday;
  timestructure.tm_hour = (uint32_t)timestamp->tm_hour;
  timestructure.tm_min = (uint32_t)timestamp->tm_min;
  timestructure.tm_sec = (uint32_t)timestamp->tm_sec;
  return timestructure;  
}
