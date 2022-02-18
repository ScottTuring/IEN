#pragma once

static int64_t _ftToMicroSeconds(const FILETIME *ft) {
	return (*(int64_t*)ft) / 10;
}

static void _microSecondsToFT(int64_t ms, FILETIME *ft) {
	assert(ms >= 0LL);

	*(int64_t*)ft = (ms * 10);
}


static int64_t _getTime(bool local) {
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	if (local) {
		FILETIME local;
		FileTimeToLocalFileTime(&ft, &local);
		ft = local;
	}

	return _ftToMicroSeconds(&ft);
}

static int64_t _convert(int64_t time, bool toLocal) {
	FILETIME ft, dest;
	_microSecondsToFT(time, &ft);

	if (toLocal) {
		FileTimeToLocalFileTime(&ft, &dest);
	} else {
		LocalFileTimeToFileTime(&ft, &dest);
	}

	return _ftToMicroSeconds(&dest);
}

static int64_t _fromSystemTime(const IcnSystemTime *st, bool asLocal) {

	// 1 �л���win32�µ�ϵͳʱ���ʽ��
	// 2 ͳһת����UTCʱ�䣬
	// 3 �л�����׼ʱ������
	SYSTEMTIME wst;
	wst.wYear = (WORD)(st->year);
	wst.wMonth = (WORD)(st->month);
	wst.wDay = (WORD)(st->dayOfMonth);
	wst.wDayOfWeek = (WORD)(st->dayOfWeak);
	wst.wHour = (WORD)(st->hour);
	wst.wMinute = (WORD)(st->minute);
	wst.wSecond = (WORD)(st->second);
	wst.wMilliseconds = (WORD)(st->millisecond);

	FILETIME ft;
	BOOL ret;
	if (asLocal) {
		SYSTEMTIME utcTime;
		ret = TzSpecificLocalTimeToSystemTime(NULL, &wst, &utcTime);
		if (ret) {
			ret = SystemTimeToFileTime(&utcTime, &ft);
		}
	} else {
		ret = SystemTimeToFileTime(&wst, &ft);
	}

	int64_t ms = 0;
	if (ret) {
		ms = _ftToMicroSeconds(&ft);
	} else {
		BLOG_ERROR("convert from system time got error, error=%d", GetLastError());
	}

	return ms;
}

static void _toSystemTime(int64_t ms, IcnSystemTime *st, bool asLocal) {
	if (ms < INT64_C(0)) {
		memset(&st, 0, sizeof(st));
		return;
	}

	// 1. ��ת����UTC���ļ�ʱ�䣬
	// 2. ת������Ӧ��ϵͳʱ��
	// 3. ���ɶ�Ӧ��SystemTime��ʽ
	FILETIME ftUTC;
	_microSecondsToFT(ms, &ftUTC);

	SYSTEMTIME wst;
	BOOL ret = FALSE;
	if (asLocal) {
		SYSTEMTIME stUTC;
		ret = FileTimeToSystemTime(&ftUTC, &stUTC);
		if (ret) {
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &wst);
		}
	} else {
		ret = FileTimeToSystemTime(&ftUTC, &wst);
	}

	if (ret) {
		st->year = wst.wYear;
		st->month = wst.wMonth;
		st->dayOfMonth = wst.wDay;
		st->dayOfWeak = wst.wDayOfWeek;
		st->hour = wst.wHour;
		st->minute = wst.wMinute;
		st->second = wst.wSecond;
		st->millisecond = wst.wMilliseconds;
	} else {
		BLOG_ERROR("convert to system time got error, err=%d", GetLastError());
		memset(&st, 0, sizeof(st));
	}
}
