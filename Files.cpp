

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Files.h"

using namespace std;
    //BaseFile Class

    BaseFile::BaseFile(string name):name(name){}
    string BaseFile::getName() const{
        return name;
    }
    void BaseFile::setName(string newName){
        name = newName;
    }

    //File Class



    File::File(string name, int size):BaseFile(name){
        this->size=size;
    }
    File::File(File &otherFile):BaseFile(otherFile.getName()){
        size=(otherFile.getSize());
    }
    int  File::getSize() {
        return size;
    }
    bool File::isFile()  {
        return true;
    }

    string File::toString(){
        return "FILE      "+getName()+"     " + to_string(getSize());
    }

    //Directory Class
    Directory::Directory(Directory& otherDir) :BaseFile(otherDir.getName()){
        this->setParent(otherDir.getParent());
        vector<BaseFile *> v = otherDir.getChildren();
        for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end(); ++it) {
            if(it.operator*()->isFile()){
                File* f= new File((File&)it.operator*());
                this->addFile(f);
            }
            else{
                Directory* d= new Directory((Directory&)it.operator*());
                d->setParent(this);
                this->addFile(d);

            }
        }
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
        //TODO: removeFile by name
        //children.erase(std::remove(children.begin(),children.end(),name),children.end());
    }// Remove the file with the specified name from children
    void Directory::removeFile(BaseFile* file){
        children.erase(std::remove(children.begin(),children.end(),file),children.end());
    }// Remove the file from children
    void Directory::sortByName(){
        std:: sort(children.begin(), children.end(), Directory::compName);
    } // Sort children by name alphabetically (not recursively)
    void Directory::sortBySize(){
        sort(children.begin(), children.end(), Directory::compSize);
    } // Sort children by size (not recursively)
    vector<BaseFile*> Directory::getChildren()
    {
        return children;
    }// Return children
    int Directory::getSize()  {
        int size=0;
        for (std::vector<BaseFile *>::iterator it = children.begin(); it != children.end(); ++it){
            size+=size + it.operator*()->getSize();
        }
        return size;
    }


    string Directory::getAbsolutePath(){
        if (parent!=nullptr)
            return parent->getAbsolutePath() + "/" +getName();
        return "/" +getName();
    }
    string Directory::toString(){
    return "DIR      "+getName()+"     " + to_string(getSize());
}

// asjbfasfab
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17
//

