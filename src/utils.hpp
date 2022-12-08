#include <string>
#include <vector>

//rect
struct Rect{
    float left;
    float top;
    float width;
    float height;
};

std::string& StringTrim(std::string &s, std::string suffix = " ");

//split字符串
std::vector<std::string> StingSplit(std::string str , std::string delimiter);

//从文件路径中得到文件夹路径
std::string FindDirectoryPath(std::string path);
