#pragma once
#include "FIleManager.h"

using namespace std;
struct buffer
{
    path Path;
    bool is_folder;
};

class Menu {
private:
    buffer copybuffer;
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

public:
    Menu();
    void mainMenu();
};

