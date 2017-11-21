
#include "FileSystem.h"
#include "GlobalVariables.h"
#include <iostream>
using namespace std;



    FileSystem::FileSystem():rootDirectory (new Directory("", nullptr)), workingDirectory(rootDirectory){}
    FileSystem::~FileSystem(){
        if (verbose==1 || verbose==3)
            std::cout <<"FileSystem::~FileSystem(){";
        delete rootDirectory;

    }
    FileSystem::FileSystem(const FileSystem &other):rootDirectory(other.rootDirectory), workingDirectory(other.workingDirectory){
        if (verbose==1 || verbose==3)
            std::cout <<"FileSystem::FileSystem(const FileSystem &other)";
    }
    FileSystem::FileSystem( FileSystem &&other):rootDirectory(other.rootDirectory), workingDirectory(other.workingDirectory){
        if (verbose==1 || verbose==3)
            std::cout <<"FileSystem::FileSystem( FileSystem &&other)";
    }
    FileSystem* FileSystem::operator=(const FileSystem &other){
        if (verbose==1 || verbose==3)
            std::cout << "FileSystem* FileSystem::operator=(const FileSystem &other)";
    }
    FileSystem* FileSystem::operator=( FileSystem &&other){
        if (verbose==1 || verbose==3)
            std::cout <<"FileSystem* FileSystem::operator=( FileSystem &&other)";
    }
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

