
#include <string>
#include <iostream>
#include "Commands.h"
#include "GlobalVariables.h"
using namespace std;



    BaseCommand::BaseCommand(string args):args(args){}
    string BaseCommand::getArgs(){
        return args;
    }

    void BaseCommand::verb(){
        if (verbose==2 || verbose==3)
            cout<< this->toString() + "\n";
    }

    Directory* BaseCommand::getLegalPath(Directory* wd, FileSystem &fs, string args) {
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
    bool BaseCommand::isWdAncestor(FileSystem& fs, Directory& d) {
        bool found=false;
        for (BaseFile* c: d.getChildren()){
            if (found)
                return true;
            if (c==&fs.getWorkingDirectory())
                return true;
            else
                if (!c->isFile())
                    found = isWdAncestor(fs,(Directory&)c);

        }
        return false;
    }

    PwdCommand::PwdCommand(string args):BaseCommand(args){}
    void PwdCommand::execute(FileSystem & fs){
        if (&fs.getWorkingDirectory()==&fs.getRootDirectory()){
            cout <<"/";
            return;
        }
        cout<<fs.getWorkingDirectory().getAbsolutePath()+ "\n";

    } // Every derived class should implement this function according to the document (pdf)
    string PwdCommand::toString() {
        return "pwdCommand \n";
    }

    CdCommand::CdCommand(string args):BaseCommand(args){}
    void CdCommand::execute(FileSystem & fs) {
        Directory& tempWorkDirectory=fs.getWorkingDirectory();

        if (getArgs().substr(0,1)=="/") {
            Directory *isLegal = getLegalPath(&fs.getRootDirectory(), fs, getArgs().substr(1));
            if (isLegal == nullptr) {
                cout << "The system cannot find the path specified \n ";
            }
            else
            fs.setWorkingDirectory(isLegal);

        }else {
            Directory* isLegal = getLegalPath(&tempWorkDirectory, fs, getArgs());
            if (isLegal == nullptr) {
                cout << "The system cannot find the path specified \n";
            }
            else
            fs.setWorkingDirectory(isLegal);
            // TODO: delete isLegal here
        }


    }
    string CdCommand::toString(){
        return "cd " + getArgs()+"\n";
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
                    cout << "The system cannot find the path specified \n";
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
                    cout << "The system cannot find the path specified \n";
                else{
                    for (BaseFile* c: isLegal->getChildren()){
                        cout <<"" + c -> toString() + "\n";
                    }
                }
            }

        }

    }
    string LsCommand::toString() {
        return "ls "+getArgs()+"\n";
    }

    MkdirCommand::MkdirCommand(string args):BaseCommand(args){

    }
    void MkdirCommand::execute(FileSystem & fs) {
        if (getArgs() == "/") {
            cout << "The directory already exists \n";
            return;
        }
        int index = getArgs().find("/");
        if (index == string::npos) {
            for (BaseFile *c: fs.getWorkingDirectory().getChildren()) {
                if (c->getName() == getArgs()) {
                    cout << "The Directory already exists \n";
                }
            }
            Directory *newdir = new Directory(getArgs(), &fs.getWorkingDirectory());
            fs.getWorkingDirectory().addFile(newdir);
            return;
        }
            Directory *curr;
            string path = getArgs();
            int index1 = getArgs().find_last_of("/");
            if (getArgs().substr(0, 1) == "/") {
                curr = &fs.getRootDirectory();
                path = path.substr(1);
            } else {
                curr = &fs.getWorkingDirectory();
            }

            while (path.length() != 0) {
                int index = path.find("/");
                if (index == string::npos) {
                    for (BaseFile *c: curr->getChildren()) {
                        if (c->getName() == path) {
                            cout << "The Directory already exists \n";
                            return;
                        }
                    }
                    Directory *newdir = new Directory(path, curr);
                    curr->addFile(newdir);
                    path = "";
                } else {
                    string name = path.substr(0, index);
                    path = path.substr(index + 1);

                    bool found = false;
                    for (BaseFile* c: curr->getChildren()) {
                        if (c->getName() == name) {
                            if (c->isFile()) {
                                cout << "The system cannot find the path specified \n";
                                return;
                            } else {
                                curr = (Directory *) c;
                                found = true;
                            }

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
    string MkdirCommand::toString(){
        return "mkdir "+getArgs()+"\n";
    }

    MkfileCommand::MkfileCommand(string args):BaseCommand(args){}
    void MkfileCommand::execute(FileSystem &fs) {
        int index = getArgs().find_last_of("/");
        if (index == string::npos) {
            for(BaseFile* c: fs.getWorkingDirectory().getChildren()) {
                if (c->getName() == getArgs()) {
                    cout << "File already exists \n";
                    return;
                }
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
                cout << "Cannot name file as the root directory \n";
            }
            Directory *dir;
            if (getArgs().substr(0, 1) == "/") {
                dir = getLegalPath(&fs.getRootDirectory(), fs, path.substr(1));
            } else {
                dir = getLegalPath(&fs.getWorkingDirectory(), fs, path);
            }
            if (dir == nullptr){
                cout << "The system cannot find the path specified \n";
                return;
            }


            vector<BaseFile *> v = dir->getChildren();
            bool found = false;
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end() && !found; ++it)
                if (it.operator*()->getName() == fileName) {
                    cout << "The file already exists \n";
                    found = true;
                }
            if (!found) {
                File *newFile = new File(fileName, std::stoi(size));
                dir->addFile(newFile);
            }



            }

        }
    string MkfileCommand::toString() {
            return "mkfile " + getArgs() +"\n" ;
        }

    CpCommand ::CpCommand(string args): BaseCommand(args){}
    void CpCommand:: execute(FileSystem & fs) {
        int index = getArgs().find(" ");
        if (index == string::npos)
            cout << " No such file or directory \n";

        else {
            string src = getArgs().substr(0, index);
            string des = getArgs().substr((index + 1));
            int index1 = src.find_last_of("/");
            BaseFile *fileToCopy;
            if (index1 == string::npos) {

                vector<BaseFile *> v = fs.getWorkingDirectory().getChildren();
                bool found = false;
                for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end() && !found; ++it)
                    if (it.operator*()->getName() == src) {
                        found = true;
                        if (it.operator*()->isFile())
                            fileToCopy = (File *) it.operator*();

                        else {
                            fileToCopy = (Directory *) it.operator*();
                        }
                    }
                if (!found) {
                    cout << " No such file or directory \n";
                    return;
                }
            }
            if (src == "/")
                fileToCopy = &fs.getRootDirectory();
            else {
                string path = src.substr(0, index1);
                Directory *isPath;
                if (path.substr(0, 1) == "/")
                    isPath = getLegalPath(&fs.getRootDirectory(), fs, path.substr(1, index1));
                else
                    isPath = getLegalPath(&fs.getWorkingDirectory(), fs, path.substr(0, index1));
                if (isPath == nullptr) {
                    cout << "No such file or directory \n";
                    return;
                }
                bool found = false;
                for (BaseFile *c: isPath->getChildren()) {
                    if (c->getName() == src.substr(index1 + 1)) {
                        found = true;
                        fileToCopy = c;
                    }
                }

                if (!found) {
                    cout << " No such file or directory \n";
                    return;
                }

            }


            Directory *destination;
            if (des.substr(0, 1) == "/")
                destination = getLegalPath(&fs.getRootDirectory(), fs, des.substr(1));

            else
                destination = getLegalPath(&fs.getWorkingDirectory(), fs, des);

            if (destination == nullptr) {
                cout << "No such file or directory \n";
                return;
            }
            if (fileToCopy->isFile()) {
                File *f1 = new File(fileToCopy->getName(), fileToCopy->getSize());
                destination->addFile(f1);
            }
            else{

                Directory* d1 = new Directory((Directory&)fileToCopy);
                destination->addFile(d1);
            }
        }

    }
    string CpCommand:: toString(){
        return "CpCommand "+getArgs()+"\n";
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
        if(oldpath== "/"){
            cout << "Can't rename directory";
        }
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
                    if(c==&fs.getWorkingDirectory()){
                        cout << "Can't rename directory";
                    }
                    c->setName(newname);
                    return;
                }
            }
        }
        cout<< "No such file or directory";

    }
    string RenameCommand::toString(){
        return "rename "+getArgs()+"\n";
    }

    HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> & history):BaseCommand(args), history(history){}
    void HistoryCommand::execute(FileSystem & fs){
        for(int i=0; i<history.size(); i++){
            cout<< to_string(i) + history[i]->toString() + "\n";
        }
    }
    string HistoryCommand::toString(){
        cout << "history "+getArgs()+"\n";
    }

    ExecCommand::ExecCommand(string args, const vector<BaseCommand *> & history):BaseCommand(args) , history(history){}
    void ExecCommand::execute(FileSystem & fs){
        int cNum = std::stoi(getArgs());
        if (cNum<0 || cNum > history.size() || history.empty()){
            cout<< "Command not found \n";
            return;
        }
        history[cNum]->execute(fs);
    }
    string ExecCommand::toString(){
        cout << "exec "+getArgs()+"\n";
    }

    ErrorCommand::ErrorCommand(string args):BaseCommand(args){}
    void ErrorCommand::execute(FileSystem & fs){
        cout << getArgs() + ": Unknown Command"+"\n";
    }
    string ErrorCommand::toString(){
        cout << "error "+getArgs()+"\n";
    }

    MvCommand::MvCommand(string args):BaseCommand(args){}

    void MvCommand::execute(FileSystem & fs) {
        string src;
        Directory *destination;
        BaseFile *fileToCopy;
        int index = getArgs().find(" ");
        if (index == string::npos) {
            cout << " No such file or directory \n";
            return;
        }

        src = getArgs().substr(0, index);
        string des = getArgs().substr((index + 1));
        int index1 = src.find_last_of("/");
        if (index1 == string::npos) {

            vector<BaseFile *> v = fs.getWorkingDirectory().getChildren();
            bool found = false;
            for (std::vector<BaseFile *>::iterator it = v.begin(); it != v.end() && !found; ++it) {
                if (it.operator*()->getName() == src) {
                    found = true;
                    if (it.operator*()->isFile())
                        fileToCopy = (File *) it.operator*();

                    else {
                        fileToCopy = (Directory *) it.operator*();
                    }
                }
            }
            if (!found) {
                cout << " No such file or directory \n";
                return;
            }
        }
        if (src == "/")
            fileToCopy = &fs.getRootDirectory();
        else {
            string path = src.substr(0, index);
            Directory *isPath;
            if (path.substr(0, 1) == "/")
                isPath = getLegalPath(&fs.getRootDirectory(), fs, path.substr(1, index1));
            else
                isPath = getLegalPath(&fs.getWorkingDirectory(), fs, path.substr(0, index1));
            if (isPath == nullptr) {
                cout << "No such file or directory \n";
                return;
            }
            bool found = false;
            for (BaseFile *c: isPath->getChildren()) {
                if (c->getName() == src.substr(index1 + 1)) {
                    found = true;
                    fileToCopy = c;
                }
            }

            if (!found) {
                cout << " No such file or directory \n";
                return;
            }

        }


        if (des.substr(0, 1) == "/")
            destination = getLegalPath(&fs.getRootDirectory(), fs, des.substr(1));

        else
            destination = getLegalPath(&fs.getWorkingDirectory(), fs, des);

        if (destination == nullptr) {
            cout << "No such file or directory \n";
            return;
        }


        //remove
        if (src == "/" | src == "") {
            cout << "Can't remove directory \n";
            return;
        }

        Directory *curr;
        string name;
        index = src.find_last_of("/");
        if (index == string::npos) {
            curr = &fs.getWorkingDirectory();
            name = src;
        } else {
            string path = src.substr(0, index);
            name = src.substr(index + 1);

            if (path.substr(0, 1) == "/")
                curr = getLegalPath(&fs.getRootDirectory(), fs, path.substr(1));
            else
                curr = getLegalPath(&fs.getWorkingDirectory(), fs, path);

            if (curr == nullptr) {
                cout << "No such file or directory \n";
                return;
            }

        }

        for (BaseFile *c:curr->getChildren()) {
            if (c->getName() == name)
                if (c->isFile()) {
                    File *f1 = new File(fileToCopy->getName(), fileToCopy->getSize());
                    destination->addFile(f1);
                    curr->removeFile(name);
                    return;
                } else if (isWdAncestor(fs, (Directory&) c)) {
                    cout << "Can't remove directory \n";
                    return;
                } else {
                    Directory* d1 = new Directory((Directory&)fileToCopy);
                    destination->addFile(d1);
                    curr->removeFile(name);
                }



        }


    }
    string MvCommand::toString(){
        cout<< "Mv "+ getArgs()+ "\n";
    }

    RmCommand::RmCommand(string args):BaseCommand(args){}
    void RmCommand::execute(FileSystem & fs){
        if (getArgs()=="/" | getArgs()== ""){
            cout<< "Can't remove directory \n";
        }
        Directory* curr;
        string name;
        int index = getArgs().find_last_of("/");
        if (index==string::npos){
            curr=&fs.getWorkingDirectory();
            name=getArgs();
        }
        else {
            string path = getArgs().substr(0,index);
            name=getArgs().substr(index+1);

            if (path.substr(0, 1) == "/")
                curr = getLegalPath(&fs.getRootDirectory(), fs, path.substr(1));
            else
                curr = getLegalPath(&fs.getWorkingDirectory(), fs, path);

            if (curr== nullptr){
                cout <<"No such file or directory \n";
                return;
            }

        }

        for (BaseFile* c:curr->getChildren()){
            if (c->getName()==name)
                if (c->isFile()){
                    curr->removeFile(name);
                    return;
                }
                else
                    if (isWdAncestor(fs,(Directory&)c)){
                        cout << "Can't remove directory \n";
                        return;
                    }else
                        curr->removeFile(name);



        }


    }
    string RmCommand::toString(){
        return "rm"+ getArgs() +"\n";
    }

    VerboseCommand::VerboseCommand(string args):BaseCommand(args){}
    void VerboseCommand::execute(FileSystem & fs){
        if (getArgs()=="1")
            verbose=1;
        else
        if (getArgs()=="2")
            verbose=2;
        else
        if (getArgs()=="3")
            verbose=3;
        else
            cout<< "Wrong verbose input \n";
    }
    string VerboseCommand::toString(){
        return "verbose " +getArgs()+"\n";
    }

//
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

