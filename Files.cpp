#include iostream
#include <vector>
#include <string>
#include <algorithm>
#include Files.h
using namespace std;
class BaseFile {
    string name;
public:
    BaseFile(string name){name = name;}
    string getName() const{
        return name;
    }
    void setName(string newName){
        name = newName;
    }
    virtual int getSize() const = 0;
};
class File : public BaseFile {
private:
    int size;
public:
    File(string name, int size):BaseFile(name){
        this->size=size;
    }
    int  getSize() const override{
        return size;
    }
};
class Directory : public BaseFile {
private:
    vector<BaseFile*> children;
    Directory *parent;
    struct compByName {
        bool compName(const BaseFile &f1, const BaseFile &f2) {
            return f1.getName() < f2.getName();
        }
    };
    struct compBySize {
        bool compSize(const BaseFile& f1, const BaseFile& f2) {
            return f1.getSize() < f2.getSize();
        }
    };
    struct Sum{
        Sum():sum {0} {}
        void operator()(const BaseFile& file ){sum+=file.getSize();}
        int sum;
    };

public:
    Directory(string name, Directory *parent) :BaseFile(name), children(), parent(parent){}// Constructor
    Directory *getParent() const{
        return parent;
    } // Return a pointer to the parent of this directory
    void setParent(Directory *newParent){
        this->parent=parent;
    } // Change the parent of this directory
    void addFile(BaseFile* file){
        children.push_back(file);
        sortByName();
    } // Add the file to children
    void removeFile(string name){
        //children.erase(std::remove(children.begin(),children.end(),name),children.end());
    }// Remove the file with the specified name from children
    void removeFile(BaseFile* file){
        children.erase(std::remove(children.begin(),children.end(),file),children.end());
    }// Remove the file from children
    void sortByName(){
        std:: sort(children.begin(), children.end(), compByName());
    } // Sort children by name alphabetically (not recursively)
    void sortBySize(){
        sort(children.begin(), children.end(), compBySize());
    } // Sort children by size (not recursively)
    vector<BaseFile*> getChildren(); // Return children
    int getSize() const{
        Sum s=for_each(children.begin(),children.end(), Sum());
        return s.sum;

    } // Return the size of the directory (recursively)

    int stamfunktzia(){
        return 0;
    }
    string getAbsolutePath(){
        if (parent!=nullptr)
            return parent->getAbsolutePath() + "/" +getName();
        return "/" +getName();
    }  //Return the path from the root to this
};
// asjbfasfab
// Created by eilonben@wincs.cs.bgu.ac.il on 11/14/17
//

