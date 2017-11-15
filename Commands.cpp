
#include <string>
#include <iostream>
#include "Commands.h"
using namespace std;



    BaseCommand::BaseCommand(string args):args(args){}
    string BaseCommand::getArgs(){
        return args;
    }

    Directory BaseCommand::getLegalPath(Directory wd, FileSystem fs, string args) {
        if (args.length()==0)
            return wd;
        if (args.substr(0,1)=="/")
            return (getLegalPath(fs.getRootDirectory(), fs , args.substr(1)));

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
        Directory isLegal = getLegalPath(tempWorkDirectory, fs,getArgs());

    }
    string CdCommand::toString(){
        return "cdCommand";
    }




//
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

