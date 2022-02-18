#include "./common.h"
#include <assert.h>
#if defined(ICN_OS_WIN)
#include "./time_win.inl"
#else
#include "./time_posix.inl"
#endif


int64_t IcnTimeGetNow(bool asLocal)
{
	return _getTime(asLocal);
}

int64_t IcnUTCTimeToLocalTime(int64_t tm)
{
	return _convert(tm, true);
}

int64_t IcnLocalTimeToUTCTime(int64_t tm) 
{
	return _convert(tm, false);
}

int64_t IcnSystemTimeToTime(const IcnSystemTime* systemTime, bool asLocal) {
	return _fromSystemTime(systemTime, asLocal);
}

void IcnTimeToSystemTime(int64_t time, IcnSystemTime* systemTime, bool asLocal) {
	_toSystemTime(time, systemTime, asLocal);
}

