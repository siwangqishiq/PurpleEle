#include <iostream>
#include "json.h"

int main(){
    JsonObjectParser parser;
    std::wstring str = ReadTextFileAsWstring("out.json");
    // std::wcout << str << std::endl;
    auto obj = parser.parseJsonObject(str);

    std::wcout << "json:" << obj->toJsonString() << std::endl;
    auto wife2 = obj->getJsonObject("wife");
    // std::cout << wife2 << std::endl;
    // std::wcout <<"name " << wife2->getString("name") << std::endl;
    // std::wcout << wife2->getInt("age") << std::endl;
    return 0;
}

void testCursiveParse(){
    auto person = JsonObject::create();
    person->put("name" , L"xinyi");
    person->put("age" , 18);
    person->put("weight" , 23.33f);
    auto wife = JsonObject::create();
    person->put("wife" , wife);

    wife->put("name" , L"maolilan2");
    wife->put("age" , 28);
    wife->put("weight" , 23.33f);
    WriteStringToFile("out.json" , person->toJsonString());
    
    JsonObjectParser parser;
    std::wstring str = ReadTextFileAsWstring("out.json");
    auto obj = parser.parseJsonObject(str);

    std::wcout << obj->toJsonString() << std::endl;

    std::wcout << obj->getString("name") << std::endl;
    std::wcout << obj->getFloat("weight") << std::endl;
    std::wcout << obj->getInt("age") << std::endl;

    auto wife2 = obj->getJsonObject("wife");
    std::cout << wife2 << std::endl;
    std::wcout <<"name " << wife2->getString("name") << std::endl;
    std::wcout << wife2->getInt("age") << std::endl;
}

void testJsonParse2(){
    auto content = ReadTextFileAsWstring("test.txt");
    JsonObjectParser parser;
    auto obj = parser.parseJsonObject(content);
    WriteStringToFile("out1.txt" , obj->getString("desc"));
    WriteStringToFile("out2.txt" , obj->getString("name"));

    std::cout << obj->getFloat("count") << std::endl;
    std::cout << obj->getFloat("weight") << std::endl;
}

void testJsonParse(){
    std::wstring data = L"{  \"name\" : \"maolilan   xx \" ,  \"age\":17  , \"count\": 123.333 ,\"desc\":\"xxxxyyyyyzzzzz\"}";
    JsonObjectParser jsonParser;
    auto obj = jsonParser.parseJsonObject(data);

    std::wcout << "name = " << obj->getString("name") << std::endl;
    std::wcout << "age = " << obj->getInt("age") << std::endl;
    std::wcout << "count = " << obj->getFloat("count") << std::endl;
    std::wcout << "desc = " << obj->getString("desc") << std::endl;
}

void testJsonArrayPut(){
    auto jsonArray = JsonArray::create();
    for(int i = 0; i<10; i++){
        std::wstring str = L"id=";
        str += std::to_wstring(i);

        jsonArray->pushString(str);
    }

    auto childList = JsonArray::create();
    for(int i = 0;i<10;i++){
        std::wstring str = L"毛利兰_";
        str += std::to_wstring(i);

        auto jObj = JsonObject::create();
        childList->push(jObj);
        
        jObj->put("name" , str);
        jObj->put("age" , 17);
    }

    auto jObj = JsonObject::create();
    jObj->put("name" , L"你好世界 Muhaha");
    jObj->put("age",17);
    jObj->put("weight" , 56.33f);
    jObj->put("desc" , L"汝等踏入此地者，接受所有命运吧~!");
    jObj->put("data" , jsonArray);
    jObj->put("childList" , childList);

    WriteStringToFile("test.txt" , jObj->toJsonString());
}

void testJsonObjectPut(){
    auto jObj = JsonObject::create();
    
    jObj->put("name" , L"你好世界 Muhaha");
    jObj->put("age",17);
    jObj->put("weight" , 56.33f);
    jObj->put("desc" , L"汝等踏入此地者，接受所有命运吧~!");

    std::shared_ptr<JsonObject> childObj = JsonObject::create();
    childObj->put("name" , L"毛利兰");
    childObj->put("age",15);
    jObj->put("child" , childObj);
    jObj->put("child2" , childObj);
    
    WriteStringToFile("test.txt" , jObj->toJsonString());
}
