#include "RL.h"


void RL::setLevels(int n) {
    levels.clear();
    levels.resize(n);
}


int RL::getLevels() {
    return levels.size() ;
}


//presupposes that the priority is valid and so is the pid
void RL::insert(int priority, int pid) {
    levels[priority].push(pid);
}


int RL::removeHighestPriorityProcess() {
    for (int i = levels.size() - 1; i >= 0; i--) {
        if (!levels[i].empty()) {
            int pid = levels[i].front();
            levels[i].pop();
            return pid;
        }
    }
    return -1;
}


int RL::getHighestPriorityProcess() {
    //highest priority has the highest index
    for (int i = levels.size() - 1; i >= 0; i--) {
        if (!levels[i].empty()) {
            return levels[i].front();
        }
    }
    return -1;
}