
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
            if (args.length()>2)
                return getLegalPath(wd->getParent(), fs , args.substr(3));
            else
                return getLegalPath(wd->getParent(), fs , "");
        }
        int index = args.find("/");
        if (index!=string::npos) {
            string name = args.substr(0, index);
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
        if (&fs.getWorkingDirectory()==&fs.getRootDirectory()){
            cout <<"/";
            return;
        }
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
        return "cd " + getArgs();
    }


    LsCommand::LsCommand(string args):BaseCommand(args){}
    void LsCommand::execute(FileSystem &fs) {
        if (getArgs().length()==0){
            vector<BaseFile *> v = fs.getWorkingDirectory().getChildren();
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end(); ++it) {
                cout << it.operator*()->toString()+"\n";
            }
        }
        else
        if (getArgs()=="-s"){
            fs.getWorkingDirectory().sortBySize();
            vector<BaseFile *> v = fs.getWorkingDirectory().getChildren();
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end(); ++it) {
                cout << it.operator*()->toString()+"\n";
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
                        cout << it.operator*()->toString()+"\n";
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
                        cout << it.operator*()->toString()+"\n";
                    }
                }
            }

        }

    }

    string LsCommand::toString() {
        return "ls "+getArgs();
    }


    MkdirCommand::MkdirCommand(string args):BaseCommand(args){

    }

    void MkdirCommand::execute(FileSystem & fs) {
        if (getArgs() == "/") {
            cout << "The directory already exists";
            return;
        }
        int index = getArgs().find("/");
        if (index == string::npos) {
            for (BaseFile *c: fs.getWorkingDirectory().getChildren()) {
                if (c->getName() == getArgs()) {
                    cout << "The Directory already exists";
                }
            }
            Directory *newdir = new Directory(getArgs(), &fs.getWorkingDirectory());
            fs.getWorkingDirectory().addFile(newdir);
            return;
        }

            Directory *isPath;
            Directory *curr;
            string path = getArgs();
            if (getArgs().substr(0, 1) == "/") {
                isPath = getLegalPath(&fs.getRootDirectory(), fs, getArgs().substr(1));
                curr = &fs.getRootDirectory();
                path = path.substr(1);
            } else {
                isPath = getLegalPath(&fs.getWorkingDirectory(), fs, getArgs());
                curr = &fs.getWorkingDirectory();
            }
            if (isPath == nullptr) {
                cout << "The system cannot find the path specified";
                return;
            }

            while (path.length() != 0) {
                int index = path.find("/");
                if (index == string::npos) {
                    for (BaseFile *c: curr->getChildren()) {
                        if (c->getName() == path) {
                            cout << "The Directory already exists";
                        }
                    }
                    Directory *newdir = new Directory(path, curr);
                    curr->addFile(newdir);
                    path = "";
                } else {
                    string name = path.substr(0, index);
                    path = path.substr(index + 1);
                    vector<BaseFile *> v = curr->getChildren();
                    bool found = false;
                    for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end() && !found; ++it) {
                        if (it.operator*()->getName() == name) {
                            if (it.operator*()->isFile()) {
                                cout << "The system cannot find the path specified";
                                return;
                            } else {
                                curr = (Directory *) it.operator*();
                                found = true;
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
        return "mkdir ";
    }

    MkfileCommand::MkfileCommand(string args):BaseCommand(args){}

    void MkfileCommand::execute(FileSystem &fs) {
        int index = getArgs().find_last_of("/");
        if (index == string::npos) {
            for(BaseFile* c: fs.getWorkingDirectory().getChildren()) {
                if (c->getName() == getArgs()) {
                    cout << "File already exists";
                    return;
                }
                string file = getArgs().substr((index + 1));

                int index1 = file.find(" ");
                string size="0";
                string fileName;
                if (index1 == string::npos) {
                    fileName = file;
                }
                else {
                    fileName = file.substr(0, file.find(" "));
                    size = file.substr(file.find(" ") + 1);
                }
                File* toAdd = new File(fileName,std::stoi(size));
                fs.getWorkingDirectory().addFile(toAdd);
                return;

            }

        } else {
            string path = getArgs().substr(0, index);
            string file = getArgs().substr((index + 1));

            int index1 = file.find(" ");
            string size="0";
            string fileName;
            if (index1 == string::npos) {
                fileName = file;
            }
            else {
                fileName = file.substr(0, file.find(" "));
                size = file.substr(file.find(" ") + 1);
            }

            if (fileName == "/") {
                cout << "Cannot name file as the root directory";
            }
            Directory *dir;
            if (getArgs().substr(0, 1) == "/") {
                dir = getLegalPath(&fs.getRootDirectory(), fs, path.substr(1));
            } else {
                dir = getLegalPath(&fs.getWorkingDirectory(), fs, path);
            }
            if (dir == nullptr){
                cout << "The system cannot find the path specified";
                return;
            }


            vector<BaseFile *> v = dir->getChildren();
            bool found = false;
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end() && !found; ++it)
                if (it.operator*()->getName() == fileName) {
                    cout << "The file already exists";
                    found = true;
                        }
            if (!found) {
                File *newFile = new File(fileName, std::stoi(size));
                dir->addFile(newFile);
            }



            }

        }

        string MkfileCommand::toString() {
            return "mkfile " ;
        }



    CpCommand ::CpCommand(string args): BaseCommand(args){}

    void CpCommand:: execute(FileSystem & fs){
    int index = getArgs().find(" ");
    if(index ==string::npos)
        cout<< " No such file or directory";

    else{
        string src = getArgs().substr(0, index);
        string des = getArgs().substr((index+1));
        int index1=src.find_last_of("/");
        BaseFile* fileToCopy;
        if(index1==string::npos) {

            vector<BaseFile *> v = fs.getWorkingDirectory().getChildren();
            bool found = false;
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end() && !found; ++it)
                if (it.operator*()->getName() == src) {
                    found = true;
                    if (it.operator*()->isFile())
                        (File &) fileToCopy = File((File &) it.operator*());

                    else {
                        (Directory &) fileToCopy = Directory((Directory &) it.operator*());
                    }
                }
            if (!found) {
                cout << " No such file or directory";
                return;
            }
        }
            if(src=="/")
                fileToCopy=&fs.getRootDirectory();
            else {
                string path = src.substr(0, index);
                Directory *isPath;
                if (path.substr(0, 1) == "/")
                    isPath = getLegalPath(&fs.getRootDirectory(), fs, path.substr(1));
                else
                    isPath = getLegalPath(&fs.getWorkingDirectory(), fs, path);
                if (isPath == nullptr)
                    cout << "No such file or directory";
                vector<BaseFile *> v = isPath->getChildren();
                bool found = false;
                for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end() && !found; ++it)
                    if (it.operator*()->getName() == src.substr(index1 + 1)) {
                        found = true;
                        if (it.operator*()->isFile())
                            (File &) fileToCopy = File((File &) it.operator*());
                        else
                            (Directory &) fileToCopy = Directory((Directory &) it.operator*());
                    }

                if (!found) {
                    cout << " No such file or directory";
                    return;
                }

            }


        Directory* destination;
       if (des.substr(0, 1) == "/")
            destination = getLegalPath(&fs.getRootDirectory(), fs, des);

        else
            destination = getLegalPath(&fs.getWorkingDirectory(), fs, des);

            if (destination == nullptr) {
                cout << "No such file or directory";
                return;
            }
            if(!fileToCopy->isFile()){
                ((Directory&)fileToCopy).setParent(destination);
            }
            destination->addFile(fileToCopy);
        }
    }


    string CpCommand:: toString(){
        return "CpCommand";
    }

    RenameCommand::RenameCommand(string args):BaseCommand(args){}

    void RenameCommand::execute(FileSystem & fs){
        int index = getArgs().find(" ");
        if(index ==string::npos) {
            cout << " No such file or directory";
            return;
        }
        string oldpath = getArgs().substr(0, index);
        string newname = getArgs().substr((index+1));
        Directory* path;
        int index1= oldpath.find_last_of("/");
        if (index1==string::npos){
            path = &fs.getWorkingDirectory();
            for (BaseFile* c:path->getChildren()){
                if (c->getName()==oldpath){
                    c->setName(newname);
                    return;
                }
            }
           cout<< "No such file or directory";
        }
        else{
            string thePath = oldpath.substr(0,index1);
            string oldname = oldpath.substr(index1+1);
            if (thePath.substr(0,1)=="/")
                path = getLegalPath(&fs.getRootDirectory(),fs,thePath.substr(1));
            else
                path = getLegalPath(&fs.getWorkingDirectory(),fs,thePath);

            if (path== nullptr){
                cout << "No such file or directory";
                return;
            }
            for (BaseFile* c:path->getChildren()) {
                if (c->getName() == oldname) {
                    c->setName(newname);
                    return;
                }
            }
        }
        cout<< "No such file or directory";

    }
    string RenameCommand::toString(){
        return "rename";
    }

//
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

