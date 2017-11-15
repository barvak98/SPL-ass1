
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
        return "pwdcommand";
    }



    CdCommand::CdCommand(string args):BaseCommand(args){}
    void CdCommand::execute(FileSystem & fs) {
        Directory& tempWorkDirectory=fs.getWorkingDirectory();

        if (getArgs().substr(0,1)=="/") {
            Directory *isLegal = getLegalPath(&fs.getRootDirectory(), fs, getArgs().substr(1));
            if (isLegal == nullptr)
                cout << "The system cannot find the path specified";

        }else {
            Directory* isLegal = getLegalPath(&tempWorkDirectory, fs, getArgs());
            fs.setWorkingDirectory(isLegal);
            // TODO: delete isLegal here
        }


    }
    string CdCommand::toString(){
        return "Command : cd " + getArgs();
    }




//
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

