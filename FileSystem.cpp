
#include "FileSystem.h"
using namespace std;



    FileSystem::FileSystem():rootDirectory(), workingDirectory(){}
    Directory& FileSystem::getRootDirectory() const{
        return *rootDirectory;
    } // Return reference to the root directory
    Directory& FileSystem::getWorkingDirectory() const{
        return *workingDirectory;
    } // Return reference to the working directory
    void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory){
        this->workingDirectory=newWorkingDirectory;
    } // Change the working directory of the file system





//
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

