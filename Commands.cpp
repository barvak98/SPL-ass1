
#include <string>
#include <iostream>
#include "Commands.h"
using namespace std;



    BaseCommand::BaseCommand(string args):args(args){}
    string BaseCommand::getArgs(){
        return args;
    }

    Directory* BaseCommand::getLegalPath(Directory* wd, FileSystem fs, string args) {
        if (args.length()==0)
            return wd;
        if (args.substr(0,2)=="..")
        {
            if (wd==&fs.getRootDirectory())
                return nullptr;
            return getLegalPath(wd->getParent(), fs , args.substr(0,2));
        }
        int index = args.find("/");
        if (index!=string::npos) {
            string name = args.substr((0, index));
            vector<BaseFile *> v = wd->getChildren();
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end(); ++it) {
                if (it.operator*()->getName() == name) {
                    if (it.operator*()->isFile())
                        return nullptr;
                    return getLegalPath((Directory *) it.operator*(), fs, args.substr(index + 1));
                }
            }
            return nullptr;
        }
        else {
            vector<BaseFile *> v = wd->getChildren();
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end(); ++it) {
                if (it.operator*()->getName() == args) {
                    if (it.operator*()->isFile())
                        return nullptr;
                    return (Directory*)it.operator*();
                }
            }
            return nullptr;
        }


    }

    PwdCommand::PwdCommand(string args):BaseCommand(args){}
    void PwdCommand::execute(FileSystem & fs){
        cout<<fs.getWorkingDirectory().getAbsolutePath();

    } // Every derived class should implement this function according to the document (pdf)
    string PwdCommand::toString() {
        return "pwdCommand";
    }



    CdCommand::CdCommand(string args):BaseCommand(args){}
    void CdCommand::execute(FileSystem & fs) {
        Directory& tempWorkDirectory=fs.getWorkingDirectory();

        if (getArgs().substr(0,1)=="/") {
            Directory *isLegal = getLegalPath(&fs.getRootDirectory(), fs, getArgs().substr(1));
            if (isLegal == nullptr) {
                cout << "The system cannot find the path specified";
            }
            else
            fs.setWorkingDirectory(isLegal);

        }else {
            Directory* isLegal = getLegalPath(&tempWorkDirectory, fs, getArgs());
            if (isLegal == nullptr) {
                cout << "The system cannot find the path specified";
            }
            else
            fs.setWorkingDirectory(isLegal);
            // TODO: delete isLegal here
        }


    }
    string CdCommand::toString(){
        return "Command : cd " + getArgs();
    }


    LsCommand::LsCommand(string args):BaseCommand(args){}
    void LsCommand::execute(FileSystem &fs) {
        if (getArgs().length()==0){
            vector<BaseFile *> v = fs.getWorkingDirectory().getChildren();
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end(); ++it) {
                cout << it.operator*()->toString()+"/n";
            }
        }
        else
        if (getArgs()=="-s"){
            fs.getWorkingDirectory().sortBySize();
            vector<BaseFile *> v = fs.getWorkingDirectory().getChildren();
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end(); ++it) {
                cout << it.operator*()->toString()+"/n";
            }
            fs.getWorkingDirectory().sortByName();
        }
        else {
            if (getArgs().substr(0,3)=="-s ") {
                Directory *isLegal = getLegalPath(&fs.getRootDirectory(), fs, getArgs().substr(3));
                if (isLegal == nullptr)
                    cout << "The system cannot find the path specified";
                else{
                    isLegal->sortBySize();
                    vector<BaseFile *> v = isLegal->getChildren();
                    for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end(); ++it) {
                        cout << it.operator*()->toString()+"/n";
                    }
                    isLegal->sortByName();
                }
            }
            else {
                Directory *isLegal = getLegalPath(&fs.getRootDirectory(), fs, getArgs());
                if (isLegal == nullptr)
                    cout << "The system cannot find the path specified";
                else{
                    vector<BaseFile *> v = isLegal->getChildren();
                    for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end(); ++it) {
                        cout << it.operator*()->toString()+"/n";
                    }
                }
            }

        }

    }

    string LsCommand::toString() {
        return "Command: ls "+getArgs();
    }


    MkdirCommand::MkdirCommand(string args):BaseCommand(args){

    }

    void MkdirCommand::execute(FileSystem & fs){
        Directory *isLegal = getLegalPath(&fs.getRootDirectory(), fs, getArgs());

        if (isLegal != nullptr)
            cout << "The directory already exists";
        else{
            Directory* curr = &fs.getWorkingDirectory();
            string path=getArgs();
            if(getArgs().substr(0,1)=="/"){
                curr=&fs.getRootDirectory();
                path=path.substr(1);

            }
            while (path.length()!=0){
                int index = path.find("/");
                if (index == string::npos) {
                    Directory *newdir = new Directory(path, curr);
                    curr->addFile(newdir);
                    path = "";
                }
                else{
                    string name = path.substr(0,index);
                    path=path.substr(index+1);
                    vector<BaseFile *> v = curr->getChildren();
                    bool found =false;
                    for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end() && !found; ++it) {
                        if (it.operator*()->getName() == name) {
                            if (it.operator*()->isFile())
                                cout << "The system cannot find the path specified";
                            else{
                                curr=(Directory*)it.operator*();
                            }

                    }
                        if (!found) {
                            Directory *newdir = new Directory(name, curr);
                            curr->addFile(newdir);
                            curr = newdir;
                        }

                }
            }

        }

    }

    string MkdirCommand::toString(){
        return "Command: mkdir "+ getArgs();
    }


//
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

