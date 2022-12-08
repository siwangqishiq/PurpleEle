#pragma once

#ifndef _UTEST_
#define _UTEST_

#include <iostream>
#include <functional>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <string.h>

class TestHandler{
private:
    std::list<std::pair<std::string , std::function<void(void)>>> testCases_;
public:
    int testAll(){
        int successCount = 0;
        int failedCount = 0;
        for(auto &entry : testCases_){
            std::cout << "run : " << stringFillWidth(64 , entry.first);
            try{
                entry.second();
                std::cout << "[OK]";
                successCount++;
            }catch(std::exception &e){
                std::cout << "[ERROR!]"; 
                std::cout << e.what();
                failedCount++;
            }
            std::cout << std::endl;
        }//end for each

        if(successCount == testCases_.size()){
            std::cout << "SUCCESS! all test case passed!" << std::endl;
        }else{
            std::cout << "ERROR! PASSED:" << successCount << " FAILED:" << failedCount << std::endl;
        }
        return 0;
    }

    void addTestCase(std::string name , std::function<void(void)> testFunc){
        testCases_.push_back(std::make_pair(name , testFunc));
    }

    std::string stringFillWidth(int width , std::string &str){
        if(str.length() >= width){
            return str;
        }

        std::string output = str;
        int blankCount = width - str.length();
        for(int i = 0 ; i < blankCount;i++){
            output += " ";
        }
        return output;
    }
};

static TestHandler utest;

template <typename T>
inline void Equal(T except , T actual){
    if(except != actual){
        std::stringstream output;
        output << "except " << except << " actual "<< actual;
        throw std::invalid_argument(output.str());
    }
}


inline void EqualWString(std::wstring except , std::wstring actual){
    if(except != actual){
        throw std::invalid_argument("wstring not equal");
    }
}

inline void Test(std::string name , std::function<void(void)> func){
    utest.addTestCase(name , func);
}

#endif