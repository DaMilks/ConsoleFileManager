#pragma once
#include "FIleManager.h"

using namespace std;
struct boofer
{
    path Path;
    bool is_folder;
};

class Menu {
private:
    boofer copyboofer;
    list<MyObject*> listob;
    path nowPath;

    void initList();

    void printFolder();

    void findDisks();

    void toUp();

    void openF(int number);

    void Copy(int number);

    void insert();

    void getsize(int number);

    void open(int number);

    void myDelete(int number);

    void Myrename(int number, string newname);

    void printHelp();

    void createFile(string filename);

    bool match(const char* s, const char* p);

    list<string> search(string mask);

    void printList(list<string>);
    
    void createFolder(string foldername);

public:
    Menu();
    void mainMenu();
};

