
#include "Environment.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


    Environment::Environment():commandsHistory(), fs(){}
    void Environment::start() {
        string input="";
        while(input!="exit"){
            std::getline (std::cin,input);
            string cmd;
            string arg;
            if(input=="exit")
                break;
            int index = input.find(" ");
            if(index==string::npos){
                cmd=input;
                if( cmd == "pwd"){
                    BaseCommand* pwd = new PwdCommand("");
                    pwd->execute(fs);
                    commandsHistory.push_back(pwd);
                }
                if( cmd =="ls")
                {
                   BaseCommand* ls = new LsCommand("");
                    ls->execute(fs);
                    commandsHistory.push_back(ls);
                }
                if(cmd=="history"){
                    BaseCommand* his =  new HistoryCommand("", commandsHistory);
                    his->execute(fs);
                    commandsHistory.push_back(his);
                }
               BaseCommand* err= new ErrorCommand(input);
                err->execute(fs);
                commandsHistory.push_back(err);

            }
            else{
                cmd = input.substr(0,index);
                arg = input.substr(index+1);
                if( cmd== "cd" ){
                    BaseCommand* cd = new CdCommand(arg);
                    cd->execute(fs);
                    commandsHistory.push_back(cd);

                }
                if( cmd== "ls" ){
                    BaseCommand* ls = new LsCommand(arg);
                    ls->execute(fs);
                    commandsHistory.push_back(ls);

                }
                if( cmd== "mkdir" ){
                    BaseCommand* mkdir = new MkdirCommand(arg);
                    mkdir->execute(fs);
                    commandsHistory.push_back(mkdir);
                }
                if( cmd== "mkfile" ){
                    BaseCommand* mkfile= new MkfileCommand(arg);
                    mkfile->execute(fs);
                    commandsHistory.push_back(mkfile);

                }
                if( cmd== "cp" ){
                    BaseCommand* cp = new CpCommand(arg);
                    cp->execute(fs);
                    commandsHistory.push_back(cp);

                }
                if( cmd== "mv" ){
                    BaseCommand* mv = new MvCommand(arg);
                    mv->execute(fs);
                    commandsHistory.push_back(mv);

                }
                if( cmd== "rename" ){
                    BaseCommand* rename = new LsCommand(arg);
                    rename->execute(fs);
                    commandsHistory.push_back(rename);

                }
                if( cmd== "rm" ){
                    BaseCommand* rm = new LsCommand(arg);
                    rm->execute(fs);
                    commandsHistory.push_back(rm);

                }
                if( cmd== "verbose" ){
                    BaseCommand* verbose = new LsCommand(arg);
                    verbose->execute(fs);
                    commandsHistory.push_back(verbose);

                }
                if( cmd== "exec" ){
                    BaseCommand* exec = new LsCommand(arg);
                    exec->execute(fs);
                    commandsHistory.push_back(exec);

                }

                BaseCommand* err= new ErrorCommand(cmd);
                err->execute(fs);
                commandsHistory.push_back(err);

            }
        }
    }
    FileSystem& Environment::getFileSystem() {
        return fs ;
    } // Get a reference to the file system
    void Environment::addToHistory(BaseCommand *command){
        commandsHistory.push_back(command);
    } // Add a new command to the history
    const vector<BaseCommand*>& Environment::getHistory() const{
        return commandsHistory;
    } // Return a reference to the history of commands





// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17.
//

