#ifndef INCLUDE_PLATFORM_LINUX_DATETIME_H
#define INCLUDE_PLATFORM_LINUX_DATETIME_H

typedef struct {
	usz msec;
	usz usec;
	usz nsec;

	u8 hour;
	u8 min;
	u8 sec;
	u8 dst;

	u8 month;
	u8 day;
	u16 year;
} LogTime;

local int
is_leap_year(int year)
{
	return ((year % 4 == 0 ) && (year % 100 != 0)) || (year % 400 == 0);
}

local void
linux_get_time(LogTime *lt)
{
	struct timespec ts;
	usz time;

	sys_clock_gettime(CLOCK_REALTIME, &ts);
	time = ts.tv_sec % 86400;
	
	lt->hour = time / 3600;
	time = time % 3600;

	lt->min = time / 60;
	lt->sec = time % 60;
	
	lt->msec = ts.tv_nsec / 1000000;
	lt->usec = ts.tv_nsec / 1000;
	lt->nsec = ts.tv_nsec;
}

local void
linux_get_date(LogTime *lt)
{
	struct timeval t;
	int days;
	int leap;
	sticky const int days_per_month[2][12] = {
	    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // Non-leap
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  // Leap
	};

	sys_gettimeofday(&t, 0);
	days = t.tv_sec / 86400;
	lt->year = 1970;

	leap = is_leap_year(lt->year);
	while (days >= (365 + leap)) {
		days -= 365 + leap;
		lt->year++;
		leap = is_leap_year(lt->year);
	}

	lt->month = 0;
	while (days >= days_per_month[leap][lt->month]) {
		days -= days_per_month[leap][lt->month];
		lt->month++;
	}

	lt->day = days + 1;
	lt->month++;
}

local void
linux_get_datetime(LogTime *lt)
{
	linux_get_date(lt);
	linux_get_time(lt);
}

#endif // INCLUDE_PLATFORM_LINUX_DATETIME_H

