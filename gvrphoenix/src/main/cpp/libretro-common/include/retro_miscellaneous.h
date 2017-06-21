/* Copyright  (C) 2010-2017 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this file (retro_miscellaneous.h).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __RARCH_MISCELLANEOUS_H
#define __RARCH_MISCELLANEOUS_H

#include <stdint.h>
#include <math.h>

#if defined(__CELLOS_LV2__) && !defined(__PSL1GHT__)
#include <sys/timer.h>
#elif defined(XENON)
#include <time/time.h>
#elif defined(GEKKO) || defined(__PSL1GHT__) || defined(__QNX__)
#include <unistd.h>
#elif defined(WIIU)
#include <wiiu/os/thread.h>
#elif defined(PSP)
#include <pspthreadman.h>
#elif defined(VITA)
#include <psp2/kernel/threadmgr.h>
#elif defined(_3DS)
#include <3ds.h>
#else
#include <time.h>
#endif

#if defined(_WIN32) && !defined(_XBOX)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(_WIN32) && defined(_XBOX)
#include <Xtl.h>
#endif

#include <limits.h>
#include <math.h>

#ifdef _MSC_VER
#include <compat/msvc.h>
#endif
#include <retro_inline.h>

#ifndef PATH_MAX_LENGTH
#if defined(_XBOX1) || defined(_3DS) || defined(PSP) || defined(GEKKO)|| defined(WIIU)
#define PATH_MAX_LENGTH 512
#else
#define PATH_MAX_LENGTH 4096
#endif
#endif

#ifndef M_PI
#if !defined(_MSC_VER) && !defined(USE_MATH_DEFINES)
#define M_PI 3.14159265358979323846264338327
#endif
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define RARCH_SCALE_BASE 256

#ifdef DJGPP
#define timespec timeval
#define tv_nsec tv_usec
#include <unistd.h>

extern int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);

static int nanosleepDOS(const struct timespec *rqtp, struct timespec *rmtp)
{
   usleep(1000000 * rqtp->tv_sec + rqtp->tv_nsec / 1000);

   if (rmtp)
      rmtp->tv_sec = rmtp->tv_nsec=0;

   return 0;
}

#define nanosleep nanosleepDOS
#endif

/**
 * retro_sleep:
 * @msec         : amount in milliseconds to sleep
 *
 * Sleeps for a specified amount of milliseconds (@msec).
 **/
static INLINE void retro_sleep(unsigned msec)
{
   struct timespec tv = {0};
   tv.tv_sec = msec / 1000;
   tv.tv_nsec = (msec % 1000) * 1000000;
   nanosleep(&tv, NULL);
}

/**
 * next_pow2:
 * @v         : initial value
 *
 * Get next power of 2 value based on  initial value.
 *
 * Returns: next power of 2 value (derived from @v).
 **/
static INLINE uint32_t next_pow2(uint32_t v)
{
   v--;
   v |= v >> 1;
   v |= v >> 2;
   v |= v >> 4;
   v |= v >> 8;
   v |= v >> 16;
   v++;
   return v;
}

/**
 * prev_pow2:
 * @v         : initial value
 *
 * Get previous power of 2 value based on initial value.
 *
 * Returns: previous power of 2 value (derived from @v).
 **/
static INLINE uint32_t prev_pow2(uint32_t v)
{
   v |= v >> 1;
   v |= v >> 2;
   v |= v >> 4;
   v |= v >> 8;
   v |= v >> 16;
   return v - (v >> 1);
}

/**
 * db_to_gain:
 * @db          : Decibels.
 *
 * Converts decibels to voltage gain.
 *
 * Returns: voltage gain value.
 **/
static INLINE float db_to_gain(float db)
{
   return powf(10.0f, db / 20.0f);
}

static INLINE uint32_t read_le(const uint8_t *data, unsigned size)
{
   unsigned i;
   uint32_t val = 0;

   size *= 8;
   for (i = 0; i < size; i += 8)
      val |= (uint32_t)*data++ << i;

   return val;
}

/* Helper macros and struct to keep track of many booleans.
 * To check for multiple bits, use &&, not &.
 * For OR, | can be used. */
typedef struct
{
   uint32_t data[8];
} retro_bits_t;

#define BIT_SET(a, bit)   ((a)[(bit) >> 3] |=  (1 << ((bit) & 7)))
#define BIT_CLEAR(a, bit) ((a)[(bit) >> 3] &= ~(1 << ((bit) & 7)))
#define BIT_GET(a, bit)   ((a)[(bit) >> 3] &   (1 << ((bit) & 7)))

#define BIT16_SET(a, bit)    ((a) |=  (1 << ((bit) & 15)))
#define BIT16_CLEAR(a, bit)  ((a) &= ~(1 << ((bit) & 15)))
#define BIT16_GET(a, bit) (!!((a) &   (1 << ((bit) & 15))))
#define BIT16_CLEAR_ALL(a)   ((a) = 0)

#define BIT32_SET(a, bit)    ((a) |=  (1 << ((bit) & 31)))
#define BIT32_CLEAR(a, bit)  ((a) &= ~(1 << ((bit) & 31)))
#define BIT32_GET(a, bit) (!!((a) &   (1 << ((bit) & 31))))
#define BIT32_CLEAR_ALL(a)   ((a) = 0)

#define BIT64_SET(a, bit)    ((a) |=  (UINT64_C(1) << ((bit) & 63)))
#define BIT64_CLEAR(a, bit)  ((a) &= ~(UINT64_C(1) << ((bit) & 63)))
#define BIT64_GET(a, bit) (!!((a) &   (UINT64_C(1) << ((bit) & 63))))
#define BIT64_CLEAR_ALL(a)   ((a) = 0)

#define BIT128_SET(a, bit)   ((a).data[(bit) >> 5] |=  (1 << ((bit) & 31)))
#define BIT128_CLEAR(a, bit) ((a).data[(bit) >> 5] &= ~(1 << ((bit) & 31)))
#define BIT128_GET(a, bit)   ((a).data[(bit) >> 5] &   (1 << ((bit) & 31)))
#define BIT128_CLEAR_ALL(a)  memset(&(a), 0, sizeof(a));

#endif