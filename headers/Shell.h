#pragma once

class Shell {
public:
    void run();
private:
    Manager os_manager; // Instance of the operating system manager
    void parseCommand(const std::string& command);
};
