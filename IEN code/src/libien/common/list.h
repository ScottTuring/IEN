/*************************************************
Copyright��������ѧ�����о���Ժ PKUSZ
Author�����������������������������ص�ʵ���ң�ICNlab��
			   IENС��
web��http://www.ienlab.com/
Description:������̬���磨Intelligent Eco Networking��
**************************************************/
#pragma once

typedef struct tagIcnListItem {
	struct tagIcnListItem *prev, *next;
} IcnListItem;

typedef struct {
	IcnListItem head;
	size_t count;
} IcnList;


void IcnListInit(IcnList *list);

// �жϸ�Ԫ���Ƿ���list��
bool IcnListExists(const IcnList* list, const IcnListItem* target);

void IcnListPushBack(IcnList *list, IcnListItem *item);
void IcnListPushFront(IcnList *list, IcnListItem *item);

void IcnListInsertAfter(IcnList* list, IcnListItem* pos, IcnListItem* item);
void IcnListMultiInsertAfter(IcnList* list, IcnListItem* pos, IcnListItem** items, size_t count);


IcnListItem* IcnListPopBack(IcnList* list);
IcnListItem* IcnListPopFront(IcnList* list);
void IcnListErase(IcnList* list, IcnListItem* item);

void IcnListSwap(IcnList* list, IcnList* other);

// ��ȡԪ�ظ���
size_t IcnListSize(const IcnList* list);
bool IcnListIsEmpty(const IcnList* list);

// �������Ԫ��
void IcnListClear(IcnList* list);


// �ϲ������б�
void IcnListMerge(IcnList* list, IcnList* other, bool atBack);

void IcnListMergeAfter(IcnList* list, IcnList* other, IcnListItem* pos);

// �����ӿ�, [first, next, next, ..., last]
IcnListItem* IcnListFirst(const IcnList* list);
IcnListItem* IcnListNext(const IcnList* list, const IcnListItem *current);

// ���������[last, prev, prev, ..., first]
IcnListItem* IcnListLast(const IcnList* list);
IcnListItem* IcnListPrev(const IcnList* list, const IcnListItem* current);