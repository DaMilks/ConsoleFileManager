#include "FIleManager.h";

MyObject::MyObject(std::filesystem::directory_entry folder)
{
    name = folder.path().filename().string();
    Path = folder.path();
    time = folder.last_write_time();
};
MyObject::MyObject() {};
std::string MyObject::getName() {
    return name;
}
path MyObject::getPath() {
    return Path;
}
void MyObject::print() {
    cout << name << "\t" <<"\t" << time << "\n";
}

MyDirectory::MyDirectory(std::filesystem::directory_entry folder) :MyObject(folder)
{
    size = -1;
}
MyDirectory::MyDirectory(std::filesystem::directory_entry folder, string name)
{
    this->name = name;
    this->Path = name;
    this->size = space(Path).capacity;
    this->time = folder.last_write_time();
}
int MyDirectory::getSize() {
    if (size > -1)
        return size;
    else
    {
        size = 0;
        for (auto const& dir_entry : recursive_directory_iterator(Path))
        {
            size += dir_entry.file_size();
        }
        return size;
    }
}
bool MyDirectory::i_folder() {
    return true;
}

MyFile::MyFile(directory_entry folder) :MyObject(folder)
{
    size = folder.file_size();
}
bool MyFile::i_folder(){
    return false;
}
int MyFile::getSize(){
    return size;
}
