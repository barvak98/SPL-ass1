
// ... You may not change this file

#include "Environment.h"

unsigned int verbose = 0;

int main(int , char **) {
	//Environment env;
	//env.start();
    Directory* dir1 = new Directory("dir1",nullptr);
    File* f1= new File("f1",24);
    dir1->addFile(f1);

    Directory* dir2 = new Directory("dir2",dir1);
    File* f2 = new File("f2",26);
    dir2->addFile(f2);
    dir1->addFile(dir2);
    File* f3 = new File("f3", 1000);


    Directory* dir3 = new Directory("dir3",dir2);
    dir2->addFile(dir3);
    dir3->addFile(f3);



    FileSystem* fs = new FileSystem();
    dir1->setParent(&fs->getRootDirectory());
    fs->getRootDirectory().addFile(dir1);
    fs->setWorkingDirectory(dir3);
     //change the current directory to root




    MkdirCommand* mkdir = new MkdirCommand("dir3");
    mkdir->execute(*fs);                // should print "file already exist"
    /*mkdir = new MkdirCommand ("/dir1/dir2/dir4");
    cout <<"\n";
    mkdir->execute(*fs);                   // should print "No such file or directory"
    cout <<"\n";
    CdCommand* cd = new CdCommand("/dir1/dir2");
    cd->execute(*fs);
    MkfileCommand* mkfile = new MkfileCommand("Eilon 150");
    mkfile->execute(*fs);
     */

    LsCommand* ls = new LsCommand("");
    ls->execute(*fs);
    CpCommand* cp = new CpCommand("/dir1/dir2/dir3 /dir1");
   // cp->execute(*fs);
    PwdCommand* pwd = new PwdCommand("");
    pwd->execute(*fs);

	return 0;
}
