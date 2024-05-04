#pragma once

class Manager {
private:
    std::vector<PCB> pcbArray;
    std::vector<RCB> rcbArray;
    RL readyList;
    int currentProcess;
public:
    Manager() : pcbArray(16), rcbArray(4), currentProcess(-1) {}
    bool init(int n, int u0, int u1, int u2, int u3);
    void init_default();
    void create(int priority);
    void destroy(int pid);
    void request(int pid, int rid, int units);
    void release(int pid, int rid, int units);
    void timeout();
    void scheduler();
};
