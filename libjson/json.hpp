//
// json解析库
//
#pragma once

#ifndef __JSON_H__
#define __JSON_H__

#include <iostream>
#include <fstream>
#include <codecvt>
#include <streambuf>
#include <locale>
#include <string>
#include <map>
#include <memory>
#include <vector>

inline std::wstring ToWideString(const std::string& input){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(input);
}

inline std::string ToByteString(const std::wstring& input){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(input);
}

inline void WriteStringToFile(const char *path , std::wstring content){
    std::ofstream file(path);
    file << ToByteString(content);
    file.close();
}

inline std::wstring ReadTextFileAsWstring(const char *path){
    std::ifstream file(path);
    std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
    return ToWideString(str);
}

class JsonObject;
class JsonArray;


class IJson{
public:
    virtual std::wstring toJsonString() = 0;
};

enum JsonValueType{
    Int,
    Float,
    String,
    JSONOBJ,
    JSONARRAY,
    None
};

class JsonValue{

public:
    JsonValueType valueType = None;

    ~JsonValue(){
    }

    JsonValue(int val) : intValue_(val){
        valueType = JsonValueType::Int;
    }

    JsonValue(float val) : floatValue_(val){
        valueType = JsonValueType::Float;
    }

    JsonValue(std::wstring val) : strValue_(val){
        valueType = JsonValueType::String;
    }

    JsonValue(std::shared_ptr<JsonObject> val) {
        jsonObjValue_ = val;
        valueType = JsonValueType::JSONOBJ;
    }

    JsonValue(std::shared_ptr<JsonArray> val) {
        jsonArrayValue_ = val;
        valueType = JsonValueType::JSONARRAY;
    }

    int getIntValue(){
        return intValue_;
    }

    float getFloatValue() {
        return floatValue_;
    }

    std::wstring getStringValue() {
        return strValue_;
    }

    std::shared_ptr<JsonObject> getJsonObjectValue(){
        return jsonObjValue_;
    }

    std::shared_ptr<JsonArray> getJsonArrayValue(){
        return jsonArrayValue_;
    }

    std::wstring displayJsonString();
private:
    int intValue_ = 0;
    float floatValue_;
    std::wstring strValue_;
    std::shared_ptr<JsonObject> jsonObjValue_;
    std::shared_ptr<JsonArray> jsonArrayValue_;
};

///
/// json对象
///
class JsonObject :public IJson{
public:
    JsonObject();

    ~JsonObject(){
        // std::cout << "destory json object" << std::endl;
    }

    static std::shared_ptr<JsonObject> create(){
        return std::make_shared<JsonObject>();
    }

    virtual std::wstring toJsonString() override;

    void putInt(std::string key , int value);
    void putFloat(std::string key , float value);
    void putString(std::string key , std::wstring value);
    void putJsonObject(std::string key , std::shared_ptr<JsonObject> value);
    void putJsonArray(std::string key , std::shared_ptr<JsonArray> value);

    void put(std::string key , int value){
        putInt(key , value);
    }
    
    void put(std::string key , float value){
        putFloat(key , value);
    }

    void put(std::string key , std::wstring value){
        putString(key , value);
    }

    void put(std::string key , std::shared_ptr<JsonObject> value){
        putJsonObject(key , value);
    }

    void put(std::string key , std::shared_ptr<JsonArray> value){
        putJsonArray(key , value);
    }

    int size(){
        return mapData_.size();
    }

    int getInt(std::string key);
    
    float getFloat(std::string key);

    std::wstring getString(std::string key);

    std::shared_ptr<JsonObject> getJsonObject(std::string key);

    std::shared_ptr<JsonArray> getJsonArray(std::string key);
private:
    std::map<std::string , std::shared_ptr<JsonValue>> mapData_;

    void parseJsonString(std::wstring &jsonStr);

    std::wstring displayJsonStringValue(std::wstring originValue);
};

///
//json数组
//
class JsonArray : public IJson{
public:
    JsonArray(){
    }

    ~JsonArray(){
    }

    static std::shared_ptr<JsonArray> create(){
        return std::make_shared<JsonArray>();
    }

    void push(int value){
        pushInt(value);
    }

    void push(float value){
        pushFloat(value);
    }

    void push(std::wstring value){
        pushString(value);
    }

    void push(std::shared_ptr<JsonObject> value){
        pushJsonObject(value);
    }

    int size(){
        return arrayData_.size();
    }

    std::shared_ptr<JsonValue> get(int position){
        return arrayData_[position];
    }

    std::shared_ptr<JsonValue> operator[](int position){
        return get(position);
    }

    int getInt(int position){
        return get(position)->getIntValue();
    }

    float getFloat(int position){
        return get(position)->getFloatValue();
    }

    std::wstring getString(int position){
        return get(position)->getStringValue();
    }

    std::shared_ptr<JsonObject> getJsonObject(int position){
        return get(position)->getJsonObjectValue();
    }

    void pushInt(int value);
    void pushFloat(float value);
    void pushString(std::wstring value);
    void pushJsonObject(std::shared_ptr<JsonObject> value);

    virtual std::wstring toJsonString() override;
private:
    std::vector<std::shared_ptr<JsonValue>> arrayData_;
};

// 解析json字符串
enum ParserState{
    INIT = 0,
    WAIT_KEY = 1,
    READ_KEY = 2,
    END_READ_KEY = 3,
    WAIT_VALUE = 4,
    READ_STRING_VALUE = 5,
    READ_VALUE = 6,
    END_READ_VALUE = 7,

    WAIT_ARRAY_VALUE = 8,
    READ_ARRAY_VALUE = 9,
    READ_ARRAY_STRING_VALUE = 10,
    END_READ_ARRAY_VALUE = 11,
    
    END = 400
};

class JsonParser{
public:
    std::shared_ptr<JsonObject> parseJsonObject(std::wstring &jsonStr);

    std::shared_ptr<JsonArray> parseJsonArray(std::wstring &jsonArrayStr);

    static bool isFloatValue(std::wstring &value){
        return !(value.find(L".") == std::wstring::npos && value.find(L"f") == std::wstring::npos);
    }

    int readEndPosition = -1;
private:
    ParserState state = INIT;

    std::shared_ptr<JsonObject> currentJsonObject = nullptr;
    std::shared_ptr<JsonArray> currentJsonArray = nullptr;

    std::wstring keyBuf;
    std::wstring currentKey;

    std::wstring valueBuf;

    int doParseObject(std::wstring &jsonStr , int beginPostion);

    int createNewJsonObject();

    int createNewJsonArray();

    int onReadArrayStringItem(std::wstring &value , int &position);

    int onReadArrayNumItem(std::wstring &value ,int &position);

    int onReadArrayJsonObjectItem(std::shared_ptr<JsonObject> jsonObject , 
            int &position , int offsetPosition);

    int onReadNumItem(std::wstring &key , std::wstring &value , int &position);

    int onReadStringItem(std::wstring &key , std::wstring &value , int &position);

    int onReadJsonObjectItem(std::wstring &key , std::shared_ptr<JsonObject> jsonObject , 
            int &position , int offsetPosition);
    
    int onReadJsonArrayItem(std::wstring &key ,
        std::shared_ptr<JsonArray>  jsonArray, int &position , int offsetPosition);

    static float strToFloat(std::wstring &str){
        try{
            return std::stof(ToByteString(str));
        }catch(std::exception &e){
            std::wcerr << "exception parese " << e.what() << str << std::endl;
            return 0.0f;
        }
    }

    static int strToInt(std::wstring &str){
        try{
            // std::wcout << str << std::endl;
            return std::stoi(ToByteString(str));
        }catch(std::exception &e){
            std::wcerr << "exception parese " << e.what() << str << std::endl;
            return 0.0f;
        }
    }
};


#endif
