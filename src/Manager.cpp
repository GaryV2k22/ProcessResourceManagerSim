#include "Manager.h"

void Manager::init(int n, int u0, int u1, int u2, int u3) {
    // Reset the system
    pcbArray.clear();
    pcbArray.resize(16);

    // Set resource counts
    rcbArray.clear();
    rcbArray.push_back({0, u0, u0});
    rcbArray.push_back({1, u1, u1});
    rcbArray.push_back({2, u2, u2});
    rcbArray.push_back({3, u3, u3});

    // Setup Ready List with specified number of priority levels
    readyList.setLevels(n);

    // Initialize with a default process
    create(0); // Assuming the initial process is always at priority 0
}

void Manager::init_default() {
    init(3, 1, 1, 2, 3); // Default initialization parameters
}



void Manager::create(int priority) {
    if (pcbArray.size() >= 16) {
        std::cerr << "Maximum number of processes reached." << std::endl;
        return;
    }

    PCB newPCB = {getNextAvailablePID(), priority, 1, {}, {}, currentProcess};
    pcbArray[newPCB.pid] = newPCB;
    readyList.insert(priority, newPCB.pid);
    if (newPCB.parent != -1) {
        pcbArray[newPCB.parent].children.push_back(newPCB.pid);
    }
    std::cout << "Process " << newPCB.pid << " created with priority " << priority << std::endl;
    if (priority > pcbArray[currentProcess].priority) {
        scheduler();
    }
}


void Manager::scheduler() {
    for (int i = readyList.levels.size() - 1; i >= 0; i--) {
        if (!readyList.levels[i].empty()) {
            int pid = readyList.levels[i].front();
            if (pid != currentProcess) {
                currentProcess = pid;
                std::cout << "Context switch to process " << pid << std::endl;
                break;
            }
        }
    }
}

void Manager::request(int pid, int rid, int units) {
    RCB& resource = rcbArray[rid];
    if (resource.available >= units) {
        resource.available -= units;
        pcbArray[pid].resources.push_back({rid, units});
    } else {
        resource.waitList.push_back({pid, units});
        pcbArray[pid].state = 2; // Assuming state 2 is blocked
    }
    scheduler();
}

void Manager::release(int pid, int rid, int units) {
    RCB& resource = rcbArray[rid];
    resource.available += units;
    auto it = std::find_if(pcbArray[pid].resources.begin(), pcbArray[pid].resources.end(), [rid](const std::pair<int, int>& res) { return res.first == rid; });
    if (it != pcbArray[pid].resources.end()) it->second -= units;
    for (auto& wait : resource.waitList) {
        if (resource.available >= wait.second) {
            resource.available -= wait.second;
            pcbArray[wait.first].state = 1; // Assuming state 1 is ready
            readyList.insert(pcbArray[wait.first].priority, wait.first);
        }
    }
    scheduler();
}
