#include "Manager.h"


bool Manager::init(int n, int u0, int u1, int u2, int u3) {
    // Reset all PCBs
    for (PCB& pcb : pcbArray) {
        pcb = PCB(); // Reset PCB to default values
    }

    // Set RCBs with specified number of units
    rcbArray[0] = RCB(0, u0);
    rcbArray[1] = RCB(1, u1);
    rcbArray[2] = RCB(2, u2);
    rcbArray[3] = RCB(3, u3);
    
    // List with specified number of priority levels
    this->readyList.setLevels(n); 

    // Initialize with a default process
    create(0); // Assuming the initial process is always at priority 0

    return true;
}

bool Manager::init_default() {
    return init(3, 1, 1, 2, 3); // Default initialization parameters
}



bool Manager::create(int priority) {
    if (pcbArray.size() >= 16) {
        std::cerr << "Maximum number of processes reached." << std::endl;
        return false;
    }
    //if priority is not between 0 and levels.size() - 1
    if (priority < 0 || priority >= readyList.getLevels()) {
        std::cerr << "Invalid priority level." << std::endl;
        return false;
    }

    PCB newPCB = {getNextAvailablePID(), priority, 1, this->currentProcess};
    pcbArray[newPCB.pid] = newPCB;
    readyList.insert(priority, newPCB.pid);
    if (newPCB.parent != -1) {
        pcbArray[newPCB.parent].children.push_back(newPCB.pid);
    }
    std::cout << "Process " << newPCB.pid << " created with priority " << priority << std::endl;
    if (priority > pcbArray[currentProcess].priority) {
        scheduler();
    }
    return true;
}


bool Manager::destroy(int pid) {
    //if pid is not valid
    if (pid < 0 || pid >= pcbArray.size() || pcbArray[pid].pid == -1 || pcbArray[pid].pid == 2) {
        std::cerr << "Invalid process ID." << std::endl;
        return false;
    }

    // if the pid is not in the list of children of the current process or the current process
    if (std::find(pcbArray[currentProcess].children.begin(), pcbArray[currentProcess].children.end(), pid) == pcbArray[currentProcess].children.end() && pid != currentProcess) {
        std::cerr << "Process " << pid << " is not a child of the current process." << std::endl;
        return false;
    }

    // if we are deleting the current process destroy the children
    if (pid == currentProcess) {
        for (int child : pcbArray[pid].children) {
            destroy(child);
        }
    }

    // release all resources held by the process
    for (int i = 0; i < pcbArray[pid].resources.size(); i++) {
        release(pid, i, pcbArray[pid].resources[i]);
    }

    // remove the process from the list of children of the parent
    pcbArray[pcbArray[pid].parent].children.erase(std::remove(pcbArray[pcbArray[pid].parent].children.begin(), pcbArray[pcbArray[pid].parent].children.end(), pid), pcbArray[pcbArray[pid].parent].children.end());

    scheduler();

}


// not done
// suggested to make the scheduler
// do a context switch when the current process is not
// the highest priority process i.e. head of priority list
void Manager::scheduler() {
    //get the current process off the readylist 
    int currentProcess = readyList.removeHighestPriorityProcess();

    //if the currentProcess.state is not blocked i.e. 0 then
    //put it back on the readylist at the same priority level
    if (pcbArray[currentProcess].state != 0) {
        readyList.insert(pcbArray[currentProcess].priority, currentProcess);
    }
    
    //get the next process from the readylist
    int nextProcess = readyList.getHighestPriorityProcess();
    this->currentProcess = nextProcess;

    std::cout << "Current Process" << currentProcess;
}

void Manager::timeout(){
    // we want to take the current process and move it to the end
    // of the readylist at the same priority level
    // the scheduler already does that so we just call it
    scheduler();
}


bool Manager::request(int pid, int rid, int units) {
    // Check if the process is requesting a valid amount of units
    if(units < 0 || units > rcbArray[rid].inventory || units + pcbArray[pid].resources[rid] > rcbArray[rid].inventory ) {
        std::cerr << "Invalid number of units requested." << std::endl;
        return false;
    }

    // Check if the process is requesting more units than available
    if (units > rcbArray[rid].available) {
        pcbArray[pid].state = 0; // Assuming state 0 is blocked
        rcbArray[rid].waitList.push_back(std::make_pair(pid, units));
        scheduler();
        return false;
    }
    else{
        rcbArray[rid].available -= units;
        pcbArray[pid].resources[rid] = units;
        return true;
    }
}

bool Manager::release(int pid, int rid, int units) {

    // Check if the process is releasing a valid amount of units
    if(units < 0 || units > pcbArray[pid].resources[rid]) {
        std::cerr << "Invalid number of units released." << std::endl;
        return false;
    }

    RCB& resource = rcbArray[rid];
    resource.available += units;
    pcbArray[pid].resources[rid] -= units;

    // Check if there are any processes waiting for the resource
    for(auto it = resource.waitList.begin(); it != resource.waitList.end(); it++) {
        if(resource.available >= it->second) {
            int waitingProcess = it->first;
            resource.available -= it->second;
            pcbArray[waitingProcess].resources[rid] = it->second;
            pcbArray[waitingProcess].state = 1; // Assuming state 1 is ready
            readyList.insert(pcbArray[waitingProcess].priority, waitingProcess);
            resource.waitList.erase(it);
        }
    }

    // we call the scheduler because there might be a higher priority process that was blocked
    scheduler();
    return true;
}

// looks for the first -1 in the pid of the pcbArray
int Manager::getNextAvailablePID() {
    for (int i = 0; i < pcbArray.size(); i++) {
        if (pcbArray[i].pid == -1) {
            return i;
        }
    }
    return -1;
}


int Manager::getCurrentProcess() {
    return currentProcess;
}