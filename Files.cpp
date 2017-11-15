

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Files.h"

using namespace std;

BaseFile::BaseFile(string name):name(name){}
    string BaseFile::getName() const{
        return name;
    }
    void BaseFile::setName(string newName){
        name = newName;
    }





    File::File(string name, int size):BaseFile(name){
        this->size=size;
    }
    int  File::getSize() {
        return size;
    }
    bool File::isFile()  {
        return true;
    }



         bool Directory::compName(BaseFile* f1, BaseFile* f2) {
            return f1->getName() < f2->getName();
        }


         bool Directory::compSize(BaseFile* f1, BaseFile* f2) {
            return f1->getSize() < f2->getSize();
        }

    bool Directory::isFile()  {
        return false;
    }
    Directory::Directory(string name, Directory *parent) :BaseFile(name), children(), parent(parent){}// Constructor
    Directory* Directory::getParent() const{
        return parent;
    } // Return a pointer to the parent of this directory
    void Directory::setParent(Directory *newParent){
        this->parent=parent;
    } // Change the parent of this directory
    void Directory::addFile(BaseFile* file){
        children.push_back(file);
        sortByName();
    } // Add the file to children
    void Directory::removeFile(string name){
        //children.erase(std::remove(children.begin(),children.end(),name),children.end());
    }// Remove the file with the specified name from children
    void Directory::removeFile(BaseFile* file){
        children.erase(std::remove(children.begin(),children.end(),file),children.end());
    }// Remove the file from children
    void Directory::sortByName(){
        std:: sort(children.begin(), children.end(), Directory::compName);
    } // Sort children by name alphabetically (not recursively
    void Directory::sortBySize(){
        sort(children.begin(), children.end(), Directory::compSize);
    } // Sort children by size (not recursively)
    vector<BaseFile*> Directory::getChildren()
    {
        return children;
    }// Return children
    int Directory::getSize()  {


    } // Return the size of the directory (recursively)


    string Directory::getAbsolutePath(){
        if (parent!=nullptr)
            return parent->getAbsolutePath() + "/" +getName();
        return "/" +getName();
    }  //Return the path from the root to this

// asjbfasfab
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17
//

