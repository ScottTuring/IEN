/*************************************************
Copyright��������ѧ�����о���Ժ PKUSZ
Author�����������������������������ص�ʵ���ң�ICNlab��
               IENС��
web��http://www.ienlab.com/
Description:������̬���磨Intelligent Eco Networking��
**************************************************/
#pragma once
#include "./basic_type.h"

// ��ȡһ��[0, UINT64_MAX]����������
uint64_t random64();

// ��ȡһ��[minValue, maxValue]����������
int32_t randomRange32(int32_t minValue, int32_t maxValue);

// ��ȡһ��[0, maxValue)����������
uint64_t randomRange64(uint64_t maxValue);


void randomBytes(uint8_t* lpBuffer, size_t size);