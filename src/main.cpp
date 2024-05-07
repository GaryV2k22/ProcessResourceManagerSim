#include <iostream>
#include "../headers/Shell.h"


int main(){

    // Shell shell;

    // shell.run();


    Manager simple_jack;


    for(auto it : simple_jack.pcbArray){
        std::cout << it.pid << " ";
        std::cout << it.parent << " ";
        std::cout << it.state << " ";
        std::cout << "\n";
    }

    simple_jack.init(3, 1, 1, 2, 3);
    std::cout << simple_jack.getCurrentProcess() << " is the current process \n";


    simple_jack.create(1);
    std::cout << simple_jack.getCurrentProcess() << " is the current process n";

    simple_jack.create(2);
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";

    simple_jack.create(2);
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";

    simple_jack.create(1);
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";

    simple_jack.request(simple_jack.getCurrentProcess(), 1,1);
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";

    simple_jack.timeout();
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";

    simple_jack.request(simple_jack.getCurrentProcess(), 2, 1);
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";

    simple_jack.request(simple_jack.getCurrentProcess(), 1, 1);
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";

    simple_jack.request(simple_jack.getCurrentProcess(), 2, 2);
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";


    for(auto it : simple_jack.pcbArray){
        std::cout << it.pid << " ";
        std::cout << it.parent << " ";
        std::cout << it.state << " ";
        std::cout << "Children: ";
        for(auto child: it.children)
        {
            std::cout << child << " "; 
        }
        std::cout << "\n";
        std::cout << "Resources: ";
        for(auto resource : it.resources ){
            std::cout << resource << " ";
        }
        std::cout << "\n";
    }
    


    simple_jack.destroy(2, simple_jack.getCurrentProcess());
    std::cout << "SUCCESSFUL DESTROY\n";
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";

    simple_jack.timeout();
    std::cout << simple_jack.getCurrentProcess() <<  " is the current process \n";


    for(auto it : simple_jack.pcbArray){
        std::cout << it.pid << " ";
        std::cout << it.parent << " ";
        std::cout << it.state << " ";
        std::cout << "Children: ";
        for(auto child: it.children)
        {
            std::cout << child << " "; 
        }
        std::cout << "\n";
        std::cout << "Resources: ";
        for(auto resource : it.resources ){
            std::cout << resource << " ";
        }
        std::cout << "\n";
    }

    return 0;
}