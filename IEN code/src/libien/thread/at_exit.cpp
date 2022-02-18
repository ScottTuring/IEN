#include "../common/common.h"
#include "./thread.h"
#include "./at_exit.h"
#include "./thread_info.h"


typedef struct {
    IcnListItem stub;

    IcnThreadExitCallBack proc;
    IcnUserData userData;
} _AtExitItem;


_AtExitManager* _newAtExitManager() {
    _AtExitManager* manager = (_AtExitManager*)malloc(sizeof(_AtExitManager));
    IcnListInit(&manager->procList);

    return manager;
}

void _deleteAtExitManager(_AtExitManager* manager) {
    free(manager);
}

static _AtExitItem* _newAtExit(IcnThreadExitCallBack proc, IcnUserData userData) {
    _AtExitItem* item = (_AtExitItem*)malloc(sizeof(_AtExitItem));

    item->proc = proc;
    item->userData = userData;
    if (userData.lpfnAddRefUserData) {
        userData.lpfnAddRefUserData(userData.userData);
    }

    return item;
}

static void _deleteAtExit(_AtExitItem* item) {
    item->proc = NULL;

    if (item->userData.lpfnReleaseUserData) {
        item->userData.lpfnReleaseUserData(item->userData.userData);
    }

    free(item);
}

static void _pendAtExit(_AtExitManager* manager, IcnThreadExitCallBack proc, IcnUserData userData) {
    _AtExitItem* item = _newAtExit(proc, userData);
    IcnListPushFront(&manager->procList, &item->stub);
}

static void _execAtExit(_AtExitItem* item) {
    assert(item->proc);
    item->proc(item->userData.userData);

    _deleteAtExit(item);
}

void _execAtExits(_AtExitManager* manager) {
    if (IcnListSize(&manager->procList) == 0) {
        return;
    }

    IcnListItem* it = IcnListFirst(&manager->procList);
    do {
        _AtExitItem* item = (_AtExitItem*)it;
        it = IcnListNext(&manager->procList, it);

        _execAtExit(item);

    } while (it);

    IcnListClear(&manager->procList);
}

int _atExit(IcnThreadExitCallBack proc, IcnUserData userData) {
    _ThreadInfo* info = _getCurrentThreadInfo();
    if (info == NULL) {
        return ICN_RESULT_NOT_SUPPORT;
    }

    if (info->atExitManager == NULL) {
        info->atExitManager = _newAtExitManager();
    }

    _pendAtExit(info->atExitManager, proc, userData);

    return 0;
}