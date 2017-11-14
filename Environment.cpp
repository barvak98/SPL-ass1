#include "Files.h"
#include "Commands.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
    vector<BaseCommand*> commandsHistory;
    FileSystem fs;

public:
    Environment():commandsHistory(), fs(){}
    void start();
    FileSystem& getFileSystem() {
        return fs ;
    } // Get a reference to the file system
    void addToHistory(BaseCommand *command){
        commandsHistory.push_back(command);
    } // Add a new command to the history
    const vector<BaseCommand*>& getHistory() const{
        return commandsHistory;
    } // Return a reference to the history of commands
};




// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

