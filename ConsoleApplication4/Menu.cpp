#include "Menu.h"
//Инициализирует список с файлами и папками в текущей деректории
void Menu::initList() {
    listob.clear();
    for (const auto& entry : directory_iterator(nowPath))
    {
        if (entry.is_directory())
            listob.push_back(new MyDirectory(entry));
        else listob.push_back(new MyFile(entry));
    }
}
//Проверка подходит ли строка по маске
bool Menu::match(const char* s, const char* p)
{
    const char* rs = nullptr;
    const char* rp = nullptr;

    while (true)
        if (*p == '*')
            rs = s,
            rp = ++p;
        else if (!*s)
            return !*p;
        else if (*s == *p || *p == '?')
            ++s,
            ++p;
        else if (rs)
            s = ++rs,
            p = rp;
        else
            return false;
}
//поиск подходящих файлов по маске
list<string> Menu::search(string mask) {

    list<string> rezult;
    string fname;
    for (auto const& dir_entry : recursive_directory_iterator(nowPath))
    {
        fname=dir_entry.path().filename().string();
        if (match(fname.c_str(), mask.c_str()))
        rezult.push_back(dir_entry.path().string());
    }
    return rezult;
}
//вывод списка файлов
void Menu::printList(list<string> strings)
{
    for (auto n : strings)
        cout << n << endl;
    system("pause");
}
//Выводит список файлов и папок
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
//Выводит список дисков (это нужно, когда мы поднялись выше корня, например C:
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
//Подняться на папку вверх. отдельная функция т.к. вслучае поднятия на папку 
//вверх можно упереться в корень файловой системы, и этонжно правильно отработать
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
//Открыть папку или файл
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
//копировать 
void Menu::Copy(int number) {
    list<MyObject*>::iterator itr = listob.begin();
    advance(itr, number - 1);
    copyboofer.is_folder = (**itr).i_folder();
    copyboofer.Path = (**itr).getPath();
}
//вставить
void Menu::insert() {
    path p = nowPath;
    const auto copyOptions = copy_options::overwrite_existing | copy_options::recursive;
    if (copyboofer.is_folder)
    {
        p /= copyboofer.Path.filename();
        create_directory(p);
    }
    copy(copyboofer.Path, p, copyOptions);
    listob.clear();
    initList();
}
//получить размер файла или папки
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
//открытие файла, папки или поднятся по пути вверх
void Menu::open(int number)
{
    if (number == 0)
        toUp();
    else
        openF(number);
}
//удаление файла или папки
void Menu::myDelete(int number) {
    list<MyObject*>::iterator itr = listob.begin();
    advance(itr, number - 1);
    remove_all((*itr)->getPath());
    listob.remove(*itr);
}
//переименовать файл или папку
void Menu::Myrename(int number, string newname)
{
    list<MyObject*>::iterator itr = listob.begin();
    advance(itr, number - 1);
    rename(nowPath / (**itr).getName(), nowPath / newname);
    initList();
}
//вывод справки по командам
void Menu::printHelp()
{
    cout << "copy <number> - copy file or folder in custom boofer" << endl
        << "open <number> - open file or folder" << endl
        << "delete <number> - delete file or folder" << endl
        << "insert obj - pate file or folder in current folder" << endl
        << "getsize <number> - print size of file or folder" << endl
        << "createFile <name> - create file" << endl
        << "createFolder <name> - create folder" << endl
        << "rename <number> - rename file or folder" << endl
        << "search <mask> - search files by mask" << endl
        << "help page - print this list of commands" << endl << endl;

}
//Создать файл
void Menu::createFile(string filename) {
    std::ofstream out;
    path p = nowPath / filename;
    out.open(p.c_str());
    out.close();
    initList();

}
//создать папку
void Menu::createFolder(string foldername)
{
    create_directory(nowPath / foldername);
}
//конструктор
Menu::Menu() {
    path p = "C:\\";
    nowPath = p;
    initList();
}
//основной цикл
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
        if (command == "createFolder")
        {
            createFolder(param);
        }
        if (command == "rename")
        {
            cout << "Inter new name:";
            cin >> param2;
            Myrename(stoi(param), param2);
        }
        if (command == "search")
        {
            printList(search(param));
        }
        system("cls");
    }
}