// Copyright Valve Corporation, All rights reserved.
// 
// Determine CPU speed under linux

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include "tier0/platform.h"

#define rdtsc(x) \
	__asm__ __volatile__ ("rdtsc" : "=A" (x))

namespace
{

struct TimeVal
{
	TimeVal() = default;

	TimeVal& operator=(const TimeVal &val) { m_TimeVal = val.m_TimeVal; return *this; }

	inline double operator-(const TimeVal &left) const
	{
		uint64 left_us = (uint64) left.m_TimeVal.tv_sec * 1000000 + left.m_TimeVal.tv_usec;
		uint64 right_us = (uint64) m_TimeVal.tv_sec * 1000000 + m_TimeVal.tv_usec;
		uint64 diff_us = right_us - left_us;
		return diff_us * ( 1.0 / 1000000.0 );
	}

	timeval m_TimeVal;
};

// Compute the positive difference between two 64 bit numbers.
constexpr inline uint64 diff(uint64 v1, uint64 v2)
{
	return v1 > v2 ? v1 - v2 : v2 - v1;
}

}  // namespace

#ifdef OSX

// Mac
uint64 GetCPUFreqFromPROC()
{
	uint64_t frequency = 0;
	size_t len = sizeof(frequency);

	// Only for Intel Macs. M1+ have multiple CPUs with different frequency.
	if (sysctlbyname("hw.cpufrequency",
		&frequency, &len, NULL, 0) == 0)
	{
		return frequency;
	}

	// Assume 2.4 GHz for M1+ for now.
	return 2400000000;
}

#else

// Linux
uint64 GetCPUFreqFromPROC()
{
	double mhz = 0;
	char line[1024], *s, search_str[] = "cpu MHz";

	/* open proc/cpuinfo */
	FILE *fp = fopen( "/proc/cpuinfo", "r" );
	if (fp == NULL)
	{
		return 0;
	}

	/* ignore all lines until we reach MHz information */
	while (fgets(line, 1024, fp) != NULL) 
	{ 
		if (strstr(line, search_str) != NULL) 
		{
			/* ignore all characters in line up to : */
			for (s = line; *s && (*s != ':'); ++s)
				;

			/* get MHz number */
			if ( *s && ( sscanf( s + 1, "%lf", &mhz) == 1 ) )
				break;
		}
    }

    fclose(fp);

    return ( uint64 )( mhz * 1000000 );
}

#endif

uint64 CalculateCPUFreq()
{
#ifdef LINUX
	const char *cpu_mhz{getenv( "CPU_MHZ" )};
	if ( cpu_mhz )
	{
		constexpr uint64 factor = 1000000;
		return factor * atoi( cpu_mhz );
	}
#endif

	// Try to open cpuinfo_max_freq. If the kernel was built with cpu scaling support disabled, this will fail.
	FILE *fp = fopen( "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r" );
	if ( fp )
	{
		char buf[ 256 ];
		uint64 retVal = 0;

		buf[ 0 ] = 0;
		if( fread( buf, 1, std::size( buf ), fp ) )
		{
			retVal = ( uint64 )atoll( buf );
		}
		fclose(fp);

		if( retVal )
		{
			return retVal * 1000;
		}
	}

	// Compute the period. Loop until we get 3 consecutive periods that
	// are the same to within a small error. The error is chosen
	// to be +/- 0.02% on a P-200.
	const uint64 error = 40000;
	const int max_iterations = 600;
	int count;
	uint64 period, period1 = error * 2, period2 = 0,  period3 = 0;

	for (count = 0; count < max_iterations; count++)
	{
		TimeVal start_time, end_time;
		uint64 start_tsc, end_tsc;

		gettimeofday( &start_time.m_TimeVal, 0 );
		rdtsc( start_tsc );
		usleep( 5000 ); // sleep for 5 msec
		gettimeofday( &end_time.m_TimeVal, 0 );
		rdtsc( end_tsc );
	
		// end_time - start_time calls into the overloaded TimeVal operator- way above, and returns a double.
		period3 = ( end_tsc - start_tsc ) / ( end_time - start_time );

		if (diff ( period1, period2 ) <= error &&
			diff ( period2, period3 ) <= error &&
			diff ( period1, period3 ) <= error )
		{
			break;
		}

		period1 = period2;
		period2 = period3;
    }

	if ( count == max_iterations )
    {
		return GetCPUFreqFromPROC(); // fall back to /proc
    }

	// Set the period to the average period measured.
	period = ( period1 + period2 + period3 ) / 3;

	// Some Pentiums have broken TSCs that increment very
	// slowly or unevenly. 
	if (period < 10000000)
	{
		return GetCPUFreqFromPROC(); // fall back to /proc
	}

	return period;
}

