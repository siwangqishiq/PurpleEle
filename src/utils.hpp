#pragma once

#include <string>
#include <vector>
#include <random>
#include <chrono>
#include "glm/vec4.hpp"
#include "render/common.hpp"

const double PI = 3.141592653589793238463;

std::string& StringTrim(std::string &s, std::string suffix = " ");

//split字符串
std::vector<std::string> StingSplit(std::string str , std::string delimiter);

//从文件路径中得到文件夹路径
std::string FindDirectoryPath(std::string path);

//颜色转换
glm::vec4 ConvertColor(int red , int green , int blue , int alpha);


inline long long CurrentTimeMillis(){
    using namespace std;
    using namespace std::chrono;

    // get absolute wall time
    auto now = system_clock::now();

    // find the number of milliseconds
    auto ms = duration_cast<milliseconds>(now.time_since_epoch());
    return ms.count();
}


//生成0.0 ~1.0 的随机浮点数
inline float GenRandomFloat(){
    static std::default_random_engine  rndEngine(CurrentTimeMillis());
    static std::uniform_real_distribution<float>ud(0.0f , 1.0f);
    return ud(rndEngine);
}

// min ~ max 随机整数
inline int GenRandomInt(int min , int max){
    static std::default_random_engine  rndEngine(CurrentTimeMillis());
    std::uniform_int_distribution<int> ud(min , max);
    return ud(rndEngine);
}

inline bool ValueInRange(float value, float min, float max){
    return (value >= min) && (value <= max);
}

// check two rectangle is intersect
bool CheckRectIntersect(Rect &rectA , Rect &rectB);


