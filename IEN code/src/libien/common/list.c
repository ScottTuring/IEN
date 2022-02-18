#include "./common.h"

void IcnListInit(IcnList* list)
{
	list->count = 0;
	list->head.next = list->head.prev = &list->head;
}

bool IcnListExists(const IcnList *list, const IcnListItem *target) {
	const IcnListItem* it = list->head.next;
	while (it != &list->head) {
		if (it == target) {
			return true;
		}
		it = it->next;
	}

	return false;
}

void IcnListInsertAfter(IcnList* list, IcnListItem *pos, IcnListItem *item) {
	assert(pos == &list->head || IcnListExists(list, pos));
	assert(!IcnListExists(list, item));

	item->next = pos->next;
	item->prev = pos;

	pos->next->prev = item;
	pos->next = item;

	++list->count;
}

void IcnListMultiInsertAfter(IcnList* list, IcnListItem* pos, IcnListItem** items, size_t count) {

    for (size_t i = 0; i < count; ++i) {
        IcnListItem* item = items[i];
        IcnListInsertAfter(list, pos, item);
        pos = item;
    }
}

void IcnListErase(IcnList* list, IcnListItem* item) {
	assert(IcnListExists(list, item));

	assert(list->head.next != &list->head);
	assert(list->head.prev != &list->head);

	item->next->prev = item->prev;
	item->prev->next = item->next;

	item->next = item->prev = NULL;

	assert(list->count > 0);
	--list->count;
}

void IcnListSwap(IcnList* list, IcnList* other) {
    
    IcnListItem listHead = list->head;
    //IcnListItem otherHead = list->head;
    
    if (other->count == 0) {
        list->head.next = list->head.prev = &list->head;
    } else {
        list->head = other->head;
        list->head.next->prev = &list->head;
        list->head.prev->next = &list->head;
    }
    
    if (list->count == 0) {
        other->head.next = other->head.prev = &other->head;
    } else {
        other->head = listHead;
        other->head.next->prev = &other->head;
        other->head.prev->next = &other->head;
    }

	size_t size = list->count;
	list->count = other->count;
	other->count = size;
}

void IcnListPushBack(IcnList* list, IcnListItem* item) {
	IcnListInsertAfter(list, list->head.prev, item);
}

void IcnListPushFront(IcnList* list, IcnListItem* item) {
	IcnListInsertAfter(list, &list->head, item);
}

IcnListItem* IcnListPopBack(IcnList* list) {
	IcnListItem* item = NULL;
	if (list->count > 0)
	{
		item = list->head.prev;
		IcnListErase(list, item);
	}

	return item;
}

IcnListItem* IcnListPopFront(IcnList* list) {
	IcnListItem* item = NULL;
	if (list->count > 0)
	{
		item = list->head.next;
		IcnListErase(list, item);
	}

	return item;
}

size_t IcnListSize(const IcnList* list) {
	assert(list->count >= 0);
	return list->count;
}

bool IcnListIsEmpty(const IcnList* list) {
	assert(list->count >= 0);
	return list->count <= 0;
}

void IcnListClear(IcnList* list) {
	list->count = 0;
	list->head.next = list->head.prev = &list->head;
}

IcnListItem* IcnListFirst(const IcnList* list) {
	if (list->count == 0) {
		return NULL;
	}

	return list->head.next;
}

IcnListItem* IcnListNext(const IcnList* list, const IcnListItem* current) {
	// assert(IcnListExists(list, current));

	assert(current != &list->head);
	if (current->next == &list->head) {
		return NULL;
	}

	return current->next;
}

IcnListItem* IcnListLast(const IcnList* list) {
    if (list->count == 0) {
        return NULL;
    }

    return list->head.prev;
}

IcnListItem* IcnListPrev(const IcnList* list, const IcnListItem* current) {
    assert(IcnListExists(list, current));

    assert(current != &list->head);
    if (current->prev == &list->head) {
        return NULL;
    }

    return current->prev;
}

void IcnListMerge(IcnList* list, IcnList* other, bool atBack) {
    BLOG_CHECK(other);

    if (other->count == 0) {
        return;
    }

    if (atBack) {
        other->head.next->prev = list->head.prev;
        other->head.prev->next = &list->head;
        
        list->head.prev->next = other->head.next;
        list->head.prev = other->head.prev;
    } else {
        other->head.next->prev = &list->head;
        other->head.prev->next = list->head.next;

        list->head.next->prev = other->head.prev;
        list->head.next = other->head.next;
    }

    list->count += other->count;
    IcnListClear(other);
}

void IcnListMergeAfter(IcnList* list, IcnList* other, IcnListItem* pos) {
    BLOG_CHECK(other);

    if (other->count == 0) {
        return;
    }

    other->head.next->prev = pos;
    other->head.prev->next = pos->next;

    pos->next->prev = other->head.prev;
    pos->next = other->head.next;

    list->count += other->count;
    IcnListClear(other);
}