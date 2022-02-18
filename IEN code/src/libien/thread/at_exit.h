#pragma once

typedef struct {
    IcnList procList;
} _AtExitManager;

_AtExitManager* _newAtExitManager();
void _deleteAtExitManager(_AtExitManager* manager);

void _execAtExits(_AtExitManager* manager);
int _atExit(IcnThreadExitCallBack proc, IcnUserData userData);