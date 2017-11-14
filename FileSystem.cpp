
#include "Files.h"


class FileSystem {
private:
    Directory* rootDirectory;
    Directory* workingDirectory;
public:
    FileSystem():rootDirectory(), workingDirectory(){}
    Directory& getRootDirectory() const{
        return *rootDirectory;
    } // Return reference to the root directory
    Directory& getWorkingDirectory() const{
        return *workingDirectory;
    } // Return reference to the working directory
    void setWorkingDirectory(Directory *newWorkingDirectory){
        this->workingDirectory=newWorkingDirectory;
    } // Change the working directory of the file system


};


//
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

