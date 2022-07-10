#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

using namespace std;
using namespace std::filesystem;
using namespace std::chrono_literals;

class MyObject {
protected:
    std::string name;
    path Path;
    file_time_type time;
    int size;
public:
    MyObject(std::filesystem::directory_entry folder);
    MyObject();
    std::string getName();
    virtual int getSize() = 0;//функция для получения размера
    virtual bool i_folder() = 0;
    path getPath();
    void print();
};

class MyDirectory :public MyObject {
public:
    MyDirectory(directory_entry folder);
    //второй конструктор у папки нужен для того случая, когда будем инициализировать не
    //просто папку, а том (диск C: D: и тд)
    MyDirectory(directory_entry folder, string name);
    int getSize() override;//размер папки будет считаться только при вызове этой функции, по умолчанию -1
    bool i_folder() override;
};

class MyFile :public MyObject {
public:
    MyFile(directory_entry folder);
    bool i_folder() override;
    int getSize() override;//размер файла просто возвращает значение локальной переменной size
};


