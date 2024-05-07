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
    this->currentProcess=0;
    return true;
}

bool Manager::init_default() {
    return init(3, 1, 1, 2, 3); // Default initialization parameters
}
 


bool Manager::create(int priority) {
    if (pcbArray.size() >= 16 && numProcesses == 16) {
        return false;
    }
    //if priority is not between 0 and levels.size() - 1
    if (priority < 0 || priority >= readyList.getLevels()) {
        return false;
    }


    int next_available = getNextAvailablePID();
    PCB newPCB = PCB(next_available, priority, 1, this->currentProcess);

    pcbArray[newPCB.pid] = newPCB;
    readyList.insert(priority, newPCB.pid);

    if (newPCB.parent != -1) {
        if (newPCB.parent >= pcbArray.size()) {
            return false;
        } else {
            pcbArray[newPCB.parent].children.push_back(newPCB.pid);

        }
    }
    
    if (priority > pcbArray[currentProcess].priority) {
        scheduler();
    }
    numProcesses++;
    return true;
}


bool Manager::destroy(int pid, int actingCurrentProcess = -1) {
    if (actingCurrentProcess == -1) {
        actingCurrentProcess = currentProcess;
    }

    // Validate PID is within range and not already terminated or uninitialized
    if (pid < 0 || pid >= pcbArray.size() || pcbArray[pid].pid == -1 || pcbArray[pid].state == 2) {
        return false;
    }
    std::cout << "Acting Current Proc: " << actingCurrentProcess << "\n";
    
    // Ensure the PID to be destroyed is either the acting current process or a child of it
    if (std::find(pcbArray[actingCurrentProcess].children.begin(), pcbArray[actingCurrentProcess].children.end(), pid) == pcbArray[actingCurrentProcess].children.end() && pid != actingCurrentProcess) {
        std::cout << "process " << pid << " cannot be deleted\n";
        return false;
    }

    std::cout << "\n currently trying to delete " << pid << "\n";
    
    // Recursively destroy all children of the process (safe handling if children array changes)
    std::vector<int> children = pcbArray[pid].children; // Copy to handle modification during iteration
    for (int childPid : children) {
        std::cout << "CHILD: " << childPid << "\n";
        destroy(childPid, pid);  // Recursive call with updated acting current process
    }

    
    // Release all resources held by the process
    for (size_t i = 0; i < pcbArray[pid].resources.size(); ++i) {
        release(pid, i, pcbArray[pid].resources[i]);
        std::cout << "PID\'s "<< pid << pcbArray[pid].resources[i]; 
    }

    std::cout << "PAST THE RELEASE! \n";
    pcbArray[pid].resources = {0,0,0,0};

    // Remove the process from the children list of its parent
    if (pcbArray[pid].parent != -1) {
        auto& parents_children = pcbArray[pcbArray[pid].parent].children;
        parents_children.erase(std::remove(parents_children.begin(), parents_children.end(), pid), parents_children.end());
    }

    // Mark the process as terminated
    pcbArray[pid].state = 2; // Assuming 2 represents a terminated state

    // If destroying the current process, need to select a new one
    if (pid == currentProcess) {
        currentProcess = -1; // Or set to a valid default PID if available
        scheduler(); // This should handle setting a new currentProcess
    }

    // Decrease the count of processes
    numProcesses--;
    std::cout << "FINISHED DESTROYING " << pid << "\n";
    return true;
}




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
        return false;
    }
    
    RCB& resource = rcbArray[rid];
    resource.available += units;
    pcbArray[pid].resources[rid] -= units;
    // Check if there are any processes waiting for the resource
    auto it = resource.waitList.begin();
    while (it != resource.waitList.end()) {
        if (resource.available >= it->second) {
            int waitingProcess = it->first;
            resource.available -= it->second;
            pcbArray[waitingProcess].resources[rid] = it->second;
            pcbArray[waitingProcess].state = 1; // Assuming state 1 is ready
            readyList.insert(pcbArray[waitingProcess].priority, waitingProcess);

            // Safely erase and move to the next iterator
            it = resource.waitList.erase(it);
        } else {
            ++it;
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