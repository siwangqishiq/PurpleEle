#include "utils.hpp"

std::string& StringTrim(std::string &s, std::string suffix){
    if (s.empty()) 
        return s;
    
    s.erase(0,s.find_first_not_of(suffix));
    s.erase(s.find_last_not_of(suffix) + 1);
    return s;
}

//split字符串
std::vector<std::string> StringSplit(std::string str , std::string delimiter){
    std::vector<std::string> splits;
    if(str.empty()) 
        return splits;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        //std::cout << token << std::endl;
        splits.push_back(token);
        str.erase(0, pos + delimiter.length());
    }//end while

    if(!str.empty()){
        splits.push_back(str);
    }
    return splits;
}

//从文件路径中得到文件夹路径
std::string FindDirectoryPath(std::string path){
    std::string directory;
    const size_t lastSlashIdx = path.rfind('\\');
    if (std::string::npos != lastSlashIdx)
    {
        directory = path.substr(0, lastSlashIdx);
    }
    return directory;
}