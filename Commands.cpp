
#include <string>
#include <iostream>
#include "FileSystem.h"
using namespace std;


class BaseCommand {
private:
    string args;

public:
    BaseCommand(string args):args(args){}
    string getArgs(){
        return args;
    }
    virtual void execute(FileSystem & fs) = 0;
    virtual string toString() = 0;
};

class PwdCommand : public BaseCommand {
private:
public:
    PwdCommand(string args):BaseCommand(args){}
    void execute(FileSystem & fs){
        cout<<fs.getWorkingDirectory().getAbsolutePath();

    } // Every derived class should implement this function according to the document (pdf)
    virtual string toString();
};

class CdCommand : public BaseCommand {
private:
public:
    CdCommand(string args):BaseCommand(args){}
    void execute(FileSystem & fs){
        if (getArgs()==".."){
            if (&(fs).getWorkingDirectory()==&(fs).getRootDirectory())
                fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
            return;
        }
        string tmpargs=getArgs().substr(0);
        string root = tmpargs.substr(0,tmpargs.find("/"));
        if (root!=fs.getRootDirectory().getName()) {
            cout << "The system cannot find the path specified";
            return;
        }
        else{
            string path = root;
            Directory& curr = fs.getRootDirectory();
            path= path.substr((0,path.find("/")));
            while (path!=""){
                vector<BaseFile*> v =curr.getChildren();
                for( std::vector<BaseFile*>::iterator it = v.begin(); it!=v.end(); ++it) {
                    if (it.operator*()->getName() == path) {
                        if (it.operator*()->isFile()) {
                            cout << "The system cannot find the path specified";
                            return;
                        }
                        curr = (Directory &) it.operator*();
                    }
                }


            }
        }
    }
    string toString();
};

class LsCommand : public BaseCommand {
private:
public:
    LsCommand(string args);
    void execute(FileSystem & fs);
    string toString();
};

class MkdirCommand : public BaseCommand {
private:
public:
    MkdirCommand(string args);
    void execute(FileSystem & fs);
    string toString();
};

class MkfileCommand : public BaseCommand {
private:
public:
    MkfileCommand(string args);
    void execute(FileSystem & fs);
    string toString();
};

class CpCommand : public BaseCommand {
private:
public:
    CpCommand(string args);
    void execute(FileSystem & fs);
    string toString();
};

class MvCommand : public BaseCommand {
private:
public:
    MvCommand(string args);
    void execute(FileSystem & fs);
    string toString();
};

class RenameCommand : public BaseCommand {
private:
public:
    RenameCommand(string args);
    void execute(FileSystem & fs);
    string toString();
};

class RmCommand : public BaseCommand {
private:
public:
    RmCommand(string args);
    void execute(FileSystem & fs);
    string toString();
};

class HistoryCommand : public BaseCommand {
private:
    const vector<BaseCommand *> & history;
public:
    HistoryCommand(string args, const vector<BaseCommand *> & history);
    void execute(FileSystem & fs);
    string toString();
};


class VerboseCommand : public BaseCommand {
private:
public:
    VerboseCommand(string args);
    void execute(FileSystem & fs);
    string toString();
};

class ErrorCommand : public BaseCommand {
private:
public:
    ErrorCommand(string args);
    void execute(FileSystem & fs);
    string toString();
};

class ExecCommand : public BaseCommand {
private:
    const vector<BaseCommand *> & history;
public:
    ExecCommand(string args, const vector<BaseCommand *> & history);
    void execute(FileSystem & fs);
    string toString();
};

//
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

