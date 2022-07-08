#include "Menu.h"
void Menu::initList() {
    listob.clear();
    for (const auto& entry : directory_iterator(nowPath))
    {
        if (entry.is_directory())
            listob.push_back(new MyDirectory(entry));
        else listob.push_back(new MyFile(entry));
    }
}

void Menu::printFolder() {
    int k = 1;
    cout << "0) ../" << endl;
    for (auto n : listob)
    {
        cout << k << ")";
        k++;
        n->print();
    }
}

void Menu::findDisks() {
    for (char drive = 'A'; drive < 'Z'; ++drive)
    {
        //if (drive == 'E')
        //    continue;
        path p = string(1, drive) + ":\\";
        if (exists(p))
        {
            directory_entry d = directory_entry(p);
            string nm = string(1, drive) + ":";
            listob.push_back(new MyDirectory(d, nm));
        }
    }
}

void Menu::toUp() {
    if (nowPath == nowPath.root_path())
    {
        listob.clear();
        findDisks();
        nowPath = "";
    }
    else {
        nowPath = nowPath.parent_path();
        initList();
    }

}

void Menu::openF(int number)
{
    list<MyObject*>::iterator itr = listob.begin();
    advance(itr, number - 1);
    if ((*itr)->i_folder())
        nowPath = (**itr).getPath();
    else {

        system(("start " + (*itr)->getPath().string()).c_str());
    }
    initList();
}

void Menu::Copy(int number) {
    list<MyObject*>::iterator itr = listob.begin();
    advance(itr, number - 1);
    copybuffer.is_folder = (**itr).i_folder();
    copybuffer.Path = (**itr).getPath();
}

void Menu::insert() {
    path p = nowPath;
    const auto copyOptions = copy_options::overwrite_existing | copy_options::recursive;
    if (copybuffer.is_folder)
    {
        p /= copybuffer.Path.filename();
        create_directory(p);
    }
    copy(copybuffer.Path, p, copyOptions);
    listob.clear();
    initList();
}

void Menu::getsize(int number) {
    list<MyObject*>::iterator itr = listob.begin();
    advance(itr, number - 1);
    double size= (*itr)->getSize();
    string units = "B";
    if (size > 512)
    {
        size /= 1024;
        units = "KB";
        if (size > 512)
        {
            size /= 1024;
            units = "MB";
            if (size > 512)
            {
                size /= 1024;
                units = "GB";
            }
                
        }
    }
    cout << "Size of " << (*itr)->getName() << "=" << size<<units << endl;
}

void Menu::open(int number)
{
    if (number == 0)
        toUp();
    else
        openF(number);
}

void Menu::myDelete(int number) {
    list<MyObject*>::iterator itr = listob.begin();
    advance(itr, number - 1);
    remove_all((*itr)->getPath());
    listob.remove(*itr);
}

void Menu::Myrename(int number, string newname)
{
    list<MyObject*>::iterator itr = listob.begin();
    advance(itr, number - 1);
    rename(nowPath / (**itr).getName(), nowPath / newname);
    initList();
}

void Menu::printHelp()
{
    cout << "copy <number> - copy file or folder in custom buffer" << endl
        << "open <number> - open file or folder" << endl
        << "delete <number> - delete file or folder" << endl
        << "insert obj - pate file or folder in current folder" << endl
        << "getsize <number> - print size of file or folder" << endl
        << "createFile <name> - create file" << endl
        << "createFolder <name> - create folder" << endl
        << "rename <number> - rename file or folder" << endl
        << "help page - print this list of commands" << endl << endl;

}

void Menu::createFile(string filename) {
    std::ofstream out;
    path p = nowPath / filename;
    out.open(p.c_str());
    out.close();
    initList();

}

Menu::Menu() {
    path p = "C:\\";
    nowPath = p;
    initList();
}

void Menu::mainMenu() {

    bool quit = false;
    string command, param, param2;
    printHelp();
    while (!quit)
    {
        cout << nowPath.string() << endl << endl;
        printFolder();
        cin >> command >> param;
        if (command == "open")
        {
            open(stoi(param));
        }
        if (command == "copy")
        {
            Copy(stoi(param));
        }
        if (command == "insert")
        {
            insert();
        }
        if (command == "getsize")
        {
            getsize(stoi(param));
            system("pause");
        }
        if (command == "delete")
        {
            myDelete(stoi(param));
        }
        if (command == "quit")
        {
            quit = true;
        }
        if (command == "help")
        {
            printHelp();
            system("pause");
        }
        if (command == "createFile")
        {
            createFile(param);
        }
        if (command == "rename")
        {
            cout << "Inter new name:";
            cin >> param2;
            Myrename(stoi(param), param2);
        }
        system("cls");
    }
}