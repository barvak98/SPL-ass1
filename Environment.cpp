
#include "Environment.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


    Environment::Environment():commandsHistory(), fs(){}
    void Environment::start() {
        cout <<"not implemented";
    }
    FileSystem& Environment::getFileSystem() {
        return fs ;
    } // Get a reference to the file system
    void Environment::addToHistory(BaseCommand *command){
        commandsHistory.push_back(command);
    } // Add a new command to the history
    const vector<BaseCommand*>& Environment::getHistory() const{
        return commandsHistory;
    } // Return a reference to the history of commands





// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

