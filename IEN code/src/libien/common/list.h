/*************************************************
Copyright：北京大学深圳研究生院 PKUSZ
Author：深圳市内容中心网络与区块链重点实验室（ICNlab）
			   IEN小组
web：http://www.ienlab.com/
Description:智能生态网络（Intelligent Eco Networking）
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

// 判断该元素是否在list中
bool IcnListExists(const IcnList* list, const IcnListItem* target);

void IcnListPushBack(IcnList *list, IcnListItem *item);
void IcnListPushFront(IcnList *list, IcnListItem *item);

void IcnListInsertAfter(IcnList* list, IcnListItem* pos, IcnListItem* item);
void IcnListMultiInsertAfter(IcnList* list, IcnListItem* pos, IcnListItem** items, size_t count);


IcnListItem* IcnListPopBack(IcnList* list);
IcnListItem* IcnListPopFront(IcnList* list);
void IcnListErase(IcnList* list, IcnListItem* item);

void IcnListSwap(IcnList* list, IcnList* other);

// 获取元素个数
size_t IcnListSize(const IcnList* list);
bool IcnListIsEmpty(const IcnList* list);

// 清空所有元素
void IcnListClear(IcnList* list);


// 合并两个列表
void IcnListMerge(IcnList* list, IcnList* other, bool atBack);

void IcnListMergeAfter(IcnList* list, IcnList* other, IcnListItem* pos);

// 遍历接口, [first, next, next, ..., last]
IcnListItem* IcnListFirst(const IcnList* list);
IcnListItem* IcnListNext(const IcnList* list, const IcnListItem *current);

// 反向遍历，[last, prev, prev, ..., first]
IcnListItem* IcnListLast(const IcnList* list);
IcnListItem* IcnListPrev(const IcnList* list, const IcnListItem* current);