/*************************************************
Copyright��������ѧ�����о���Ժ PKUSZ
Author�����������������������������ص�ʵ���ң�ICNlab��
			   IENС��
web��http://www.ienlab.com/
Description:������̬���磨Intelligent Eco Networking��
**************************************************/
#pragma once

typedef struct
{
	int32_t	year;		// ��ݣ�eg: 2019
	int32_t month;		// �·ݣ�1-12
	int32_t dayOfWeak;	// ���еڼ��죬0=�����죬1=����һ��ȡֵ��Χ0-6
	int32_t dayOfMonth;	// ���еڼ��죬��1��ʼ��ȡֵ��Χ1-31
	int32_t hour;		// Сʱ����0��ʼ��ȡֵ��Χ0-23
	int32_t minute;		// ���ӣ���0��ʼ��ȡֵ��Χ0-59
	int32_t second;		// �룬��0��ʼ��ȡֵ��Χ0-59(�����ڴ������룬�����п��ܻ��1���������60��ֵ)
	int32_t millisecond;	// ���룬��0��ʼ��ȡֵ��Χ0-999
} IcnSystemTime;


// ��ȡ��ǰϵͳʱ��
// local=true:����ʱ��  local=false:UTCʱ��
int64_t IcnTimeGetNow(bool asLocal);

// UTCʱ��<->����ʱ��
int64_t IcnUTCTimeToLocalTime(int64_t tm);
int64_t IcnLocalTimeToUTCTime(int64_t tm);

// SYSTEMTIME<->ʱ��
int64_t IcnSystemTimeToTime(const IcnSystemTime* lpSystemTime, bool asLocal);
void IcnTimeToSystemTime(int64_t time, IcnSystemTime* lpSystemTime, bool asLocal);