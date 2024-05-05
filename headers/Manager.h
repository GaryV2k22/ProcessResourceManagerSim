#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "PCB.h"
#include "RCB.h"
#include "RL.h"

class Manager {

// private:

public:
    std::vector<PCB> pcbArray;
    std::vector<RCB> rcbArray;
    RL readyList;
    int numProcesses;
    int currentProcess;


    Manager() : pcbArray(16), rcbArray(4), readyList(3) {
        currentProcess = -1;
    }

    bool init(int n, int u0, int u1, int u2, int u3); // done!
    bool init_default(); // done!

    bool create(int priority);
    bool destroy(int pid);
    bool request(int pid, int rid, int units);
    bool release(int pid, int rid, int units);

    void timeout();
    void scheduler();

    int getNextAvailablePID();
    int getCurrentProcess();

};
