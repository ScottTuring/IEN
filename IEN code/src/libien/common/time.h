/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
**************************************************/
#pragma once

typedef struct
{
	int32_t	year;		// 年份，eg: 2019
	int32_t month;		// 月份，1-12
	int32_t dayOfWeak;	// 周中第几天，0=星期天，1=星期一，取值范围0-6
	int32_t dayOfMonth;	// 月中第几天，从1开始，取值范围1-31
	int32_t hour;		// 小时，从0开始，取值范围0-23
	int32_t minute;		// 分钟，从0开始，取值范围0-59
	int32_t second;		// 秒，从0开始，取值范围0-59(但由于存在闰秒，所以有可能会加1正闰秒产生60的值)
	int32_t millisecond;	// 毫秒，从0开始，取值范围0-999
} IcnSystemTime;


// 获取当前系统时间
// local=true:本地时间  local=false:UTC时间
int64_t IcnTimeGetNow(bool asLocal);

// UTC时间<->本地时间
int64_t IcnUTCTimeToLocalTime(int64_t tm);
int64_t IcnLocalTimeToUTCTime(int64_t tm);

// SYSTEMTIME<->时间
int64_t IcnSystemTimeToTime(const IcnSystemTime* lpSystemTime, bool asLocal);
void IcnTimeToSystemTime(int64_t time, IcnSystemTime* lpSystemTime, bool asLocal);