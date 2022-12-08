#include <iostream>
#include <string>
#include "utest.hpp"
#include "json.hpp"

void testJsonObject(){
    Test("Test JsonObject construct" , [](){
        auto json = JsonObject::create();
        // std::wcout << json->toJsonString() << std::endl;
        EqualWString(L"{}" , json->toJsonString());
    });

    Test("Test JsonObject set Int" , [](){
        auto json = JsonObject::create();
        json->put("key1" , 1);
        json->put("key2" , 2);
        json->put("key3" , 303);

        Equal(1 , json->getInt("key1"));
        Equal(2 , json->getInt("key2"));
        Equal(303 , json->getInt("key3"));
    });

    Test("Test JsonObject set Float" , [](){
        auto json = JsonObject::create();
        json->put("key1" , 1.0f);
        json->put("key2" , 2.0f);
        json->put("key3" , 3.0f);

        Equal(1.0f , json->getFloat("key1"));
        Equal(2.0f , json->getFloat("key2"));
        Equal(3.0f , json->getFloat("key3"));
    });

    Test("Test JsonObject set String" ,[](){
        auto json = JsonObject::create();

        json->put("key1" , L"key1_value");
        json->put("key2" , L"key2_value");
        json->put("key3" , L"key3_值");

        EqualWString(json->getString("key1") , L"key1_value");
        EqualWString(json->getString("key2") , L"key2_value");
        EqualWString(json->getString("key3") , L"key3_值");
    });
}

//json array 
void testJsonArray(){
    Test("Test JsonArray" ,[](){
        auto jsonArray = JsonArray::create();
        EqualWString(jsonArray->toJsonString() , L"[]");
    });

    Test("Test JsonArray1" ,[](){
        auto jsonArray = JsonArray::create();
        
        jsonArray->pushInt(1);
        jsonArray->pushInt(2);
        jsonArray->pushInt(3);

        EqualWString(jsonArray->toJsonString() , L"[1,2,3]");
    });

    Test("Test JsonArray String" , [](){
        auto jsonArray = JsonArray::create();
        jsonArray->push(L"你好");
        jsonArray->push(L"世界");
        jsonArray->push(L"Hello");
        jsonArray->push(L"World");

        EqualWString(L"[\"你好\",\"世界\",\"Hello\",\"World\"]" , jsonArray->toJsonString());
    });
}

void testJsonParse(){
    Test("Test json parse1", [](){
        std::wstring jsonString = L"{  \"name\" : \"maolilan\" ,  \"age\":17  , \"count\": 123.333 ,\"desc\":\"xxxxyyyyyzzzzz\"}";
        JsonParser parser;
        auto json = parser.parseJsonObject(jsonString);

        Equal(17 , json->getInt("age"));
        Equal(123.333f , json->getFloat("count"));
        EqualWString(L"maolilan" , json->getString("name"));
    });

    Test("Test json parse2", [](){
        auto subJson = JsonObject::create();
        subJson->put("name" , L"毛利兰");
        subJson->put("age" , 17);
        subJson->put("desc" , L"头上有犄角");

        EqualWString(L"头上有犄角" , subJson->getString("desc"));

        auto jsonStr = subJson->toJsonString();

        JsonParser parser;
        auto result = parser.parseJsonObject(jsonStr);
        EqualWString(L"头上有犄角" , result->getString("desc"));
        EqualWString(L"毛利兰" , result->getString("name"));
        Equal(17 , result->getInt("age"));
    });

    Test("Test json parse circular" , [](){
        auto _subJson = JsonObject::create();
        _subJson->put("name" , L"毛利兰");
        _subJson->put("age" , 17);
        _subJson->put("desc" , L"头上有犄角");
        
        auto _json = JsonObject::create();
        _json->put("friend" , _subJson);
        _json->put("score" , 27);
        _json->put("name" , L"工藤新一");
        _json->put("age" , 18);
        _json->put("score2" , 123456);
        _json->put("my" , L"HelloWorld");


        auto originJson = _json;
        std::wstring jsonStr = originJson->toJsonString();
        WriteStringToFile("out.json" , jsonStr);
        
        JsonParser parser;
        auto json = parser.parseJsonObject(jsonStr);

        auto originWifeJson = originJson->getJsonObject("friend");
        Equal(originWifeJson->getInt("age") , 17);

        auto wifeJson = json->getJsonObject("friend");
        Equal(originWifeJson->getInt("age") , wifeJson->getInt("age"));
        EqualWString(originWifeJson->getString("desc") , wifeJson->getString("desc"));
        EqualWString(originWifeJson->getString("name") , wifeJson->getString("name"));


        Equal(originJson->getInt("age") , json->getInt("age"));
        // Equal(originJson->getInt("score") , json->getInt("score"));
        EqualWString(originJson->getString("name") , json->getString("name"));
    });
    
    Test("Test parse json from string" , [](){
        std::wstring str = L"{\"age\":18,\"friend\":{   \"age\":17,\"desc\":\"头上有犄角\",\"name\":\"毛利兰\"},\"my\":\"HelloWorld\",\"name\":\"工藤新一\",\"score\":27, \"add\":\"test add!!\",\"score2\":123456}";

        // std::wstring str = L"{\"age\":18,\"my\":\"HelloWorld\",\"name\":\"工藤新一\",\"score\":27, \"add\":\"test add!!\",\"score2\":123456}";

        JsonParser parser;
        auto json = parser.parseJsonObject(str);
        Equal(18 , json->getInt("age"));
        Equal(27 , json->getInt("score"));
        Equal(123456 , json->getInt("score2"));
        EqualWString(L"test add!!" , json->getString("add"));
    });

    Test("Test json parse circular2" , [](){
        auto _subJson = JsonObject::create();
        _subJson->put("name" , L"毛利兰");
        _subJson->put("age" , 17);
        _subJson->put("desc" , L"头上有犄角");
        
        auto _json = JsonObject::create();
        _json->put("wife" , _subJson);
        _json->put("score" , 27);
        _json->put("name" , L"工藤新一");
        _json->put("age" , 18);
        _json->put("score2" , 123456);
        _json->put("my" , L"HelloWorld");


        auto originJson = _json;
        std::wstring jsonStr = originJson->toJsonString();
        WriteStringToFile("out.json" , jsonStr);
        
        JsonParser parser;
        auto json = parser.parseJsonObject(jsonStr);

        auto originWifeJson = originJson->getJsonObject("wife");
        Equal(originWifeJson->getInt("age") , 17);

        auto wifeJson = json->getJsonObject("wife");
        Equal(originWifeJson->getInt("age") , wifeJson->getInt("age"));
        EqualWString(originWifeJson->getString("desc") , wifeJson->getString("desc"));
        EqualWString(originWifeJson->getString("name") , wifeJson->getString("name"));
        
        Equal(originJson->getInt("age") , json->getInt("age"));
        EqualWString(originJson->getString("name") , json->getString("name"));
    });


    Test("Test json parse circular3" , [](){
        auto _subJson1 = JsonObject::create();
        _subJson1->put("name" , L"毛利兰");
        _subJson1->put("age" , 17);
        _subJson1->put("desc" , L"头上有犄角");

        auto _subJson2 = JsonObject::create();
        _subJson2->put("name" , L"毛利兰2");
        _subJson2->put("age" , 17);
        _subJson2->put("desc" , L"头上有犄角2");
        
        auto _json = JsonObject::create();
        _json->put("friend1" , _subJson1);
        _json->put("friend2" , _subJson2);
        _json->put("my" , L"HelloWorld");

        auto originJson = _json;
        std::wstring jsonStr = originJson->toJsonString();
        WriteStringToFile("out.json" , jsonStr);
        
        JsonParser parser;
        auto json = parser.parseJsonObject(jsonStr);

        auto originWifeJson = originJson->getJsonObject("friend2");
        Equal(originWifeJson->getInt("age") , 17);

        auto wifeJson = json->getJsonObject("friend2");
        Equal(originWifeJson->getInt("age") , wifeJson->getInt("age"));
        EqualWString(originWifeJson->getString("desc") , wifeJson->getString("desc"));
        EqualWString(originWifeJson->getString("name") , wifeJson->getString("name"));

        Equal(originJson->getInt("age") , json->getInt("age"));
        // Equal(originJson->getInt("score") , json->getInt("score"));
        EqualWString(originJson->getString("name") , json->getString("name"));
    });
}

void testJsonArrayParse(){
    Test("Test JsonArray parse" , [](){
        auto _jsonArray = JsonArray::create();
        _jsonArray->push(L"AAA");
        _jsonArray->push(L"BBB");
        _jsonArray->push(L"CCC");

        std::wstring arrayStr = _jsonArray->toJsonString();
        // std::wcout << arrayStr << std::endl;

        JsonParser parser;
        auto jsonArray = parser.parseJsonArray(arrayStr);

        EqualWString(L"AAA" , jsonArray->getString(0));
        EqualWString(L"BBB" , jsonArray->getString(1));
        EqualWString(L"CCC" , jsonArray->getString(2));
    });

    Test("Test JsonArray parse2" , [](){
        auto _jsonArray = JsonArray::create();
        _jsonArray->push(L"你好");
        _jsonArray->push(L"世界");
        _jsonArray->push(L"毛利兰");
        _jsonArray->push(L"工藤新一");

        std::wstring arrayStr = _jsonArray->toJsonString();
        // std::wcout << arrayStr << std::endl;

        JsonParser parser;
        auto jsonArray = parser.parseJsonArray(arrayStr);

        EqualWString(L"你好" , jsonArray->getString(0));
        EqualWString(L"世界" , jsonArray->getString(1));
        EqualWString(L"工藤新一" , jsonArray->getString(3));
        EqualWString(L"毛利兰" , jsonArray->getString(2));
    });

    Test("Test JsonArray parse int value" , [](){
        std::wstring str = L"  [ 1,  -2,33,4, 5, 6,  7] ";
        JsonParser parser;
        auto jsonArray = parser.parseJsonArray(str);

        Equal(7 , jsonArray->size());
        
        Equal(1 , jsonArray->getInt(0));
        Equal(-2 , jsonArray->getInt(1));
        Equal(33 , jsonArray->getInt(2));
        Equal(4 , jsonArray->getInt(3));
        Equal(5 , jsonArray->getInt(4));
        Equal(6 , jsonArray->getInt(5));
        Equal(7 , jsonArray->getInt(6));
    });

    Test("Test JsonArray parse float value" , [](){
        auto _json = JsonArray::create();
        for(int i = 1 ; i <= 200 ; i++){
            _json->push(i + 0.33f);
        }//end for i

        JsonParser parser;
        auto str = _json->toJsonString();
        auto json = parser.parseJsonArray(str);

        for(int i = 0; i < json->size() ; i++){
            Equal(_json->getFloat(i) , json->getFloat(i));
        }//end for i
    });

    Test("Test JsonArray parse JsonObject" , [](){
        auto _json1 = JsonObject::create();
        _json1->put("name" , L"maolilan");
        _json1->put("age" , 17);
        auto _json2 = JsonObject::create();
        _json2->put("name" , L"xinyi");
        _json2->put("age" , 18);
        auto _json3 = JsonObject::create();
        _json3->put("name" , L"huiyuanai");
        _json3->put("age" , 19);

        std::shared_ptr<JsonArray> _jsonArray = JsonArray::create();
        _jsonArray->push(_json1);
        _jsonArray->push(_json2);
        _jsonArray->push(_json3);

        // WriteStringToFile("out.json" , _jsonArray->toJsonString());

        std::wstring str = _jsonArray->toJsonString();
        JsonParser parser;
        auto jsonArray = parser.parseJsonArray(str);

        Equal(3 , jsonArray->size());
        auto json1 = jsonArray->getJsonObject(0);
        auto json2 = jsonArray->getJsonObject(1);
        auto json3 = jsonArray->getJsonObject(2);

        Equal(_json1->getInt("age") , json1->getInt("age"));
        Equal(_json2->getInt("age") , json2->getInt("age"));
        Equal(_json3->getInt("age") , json3->getInt("age"));
        EqualWString(_json1->getString("name") , json1->getString("name"));
        EqualWString(_json2->getString("name") , json2->getString("name"));
        EqualWString(_json3->getString("name") , json3->getString("name"));
    });

    Test("Test JsonArray parse from string" , [](){
        std::wstring str = L"[   {\"age\":17 , \"name\":\"maolilan\"} ,{\"age\":18,\"name\":\"xinyi\"}, {\"age\":19,\"name\":\"huiyuanai2\"}]";

        JsonParser parser;
        auto jsonArray = parser.parseJsonArray(str);

        Equal(3 , jsonArray->size());
        auto json1 = jsonArray->getJsonObject(0);
        auto json2 = jsonArray->getJsonObject(1);
        auto json3 = jsonArray->getJsonObject(2);

        Equal(17 , json1->getInt("age"));
        Equal(18 , json2->getInt("age"));
        Equal(19 , json3->getInt("age"));
        EqualWString(L"maolilan", json1->getString("name"));
        EqualWString(L"xinyi", json2->getString("name"));
        EqualWString(L"huiyuanai2", json3->getString("name"));
    });

    Test("Test json object contain jsonarray" , [](){
        auto _json1 = JsonObject::create();
        _json1->put("name" , L"maolilan");
        _json1->put("age" , 17);
        auto _json2 = JsonObject::create();
        _json2->put("name" , L"xinyi");
        _json2->put("age" , 18);
        auto _json3 = JsonObject::create();
        _json3->put("name" , L"huiyuanai");
        _json3->put("age" , 19);

        std::shared_ptr<JsonArray> _jsonArray = JsonArray::create();
        _jsonArray->push(_json1);
        _jsonArray->push(_json2);
        _jsonArray->push(_json3);

        auto _json = JsonObject::create();
        _json->putString("name" , L"Hello");
        _json->putInt("id" , 1001);
        _json->putJsonArray("friends" , _jsonArray);

        WriteStringToFile("out.json" , _json->toJsonString());
        JsonParser parser;
        auto str = _json->toJsonString();
        auto json = parser.parseJsonObject(str);

        EqualWString(_json->getString("name") , json->getString("name"));
        Equal(_json->getInt("age") , json->getInt("age"));

        std::shared_ptr<JsonArray> friendJsonArray = json->getJsonArray("friends");
        Equal(_jsonArray->size() , friendJsonArray->size());

        auto json1 = friendJsonArray->getJsonObject(0);
        auto json2 = friendJsonArray->getJsonObject(1);
        auto json3 = friendJsonArray->getJsonObject(2);

        Equal(_json1->getInt("age") , json1->getInt("age"));
        Equal(_json2->getInt("age") , json2->getInt("age"));
        Equal(_json3->getInt("age") , json3->getInt("age"));
        EqualWString(_json1->getString("name") , json1->getString("name"));
        EqualWString(_json2->getString("name") , json2->getString("name"));
        EqualWString(_json3->getString("name") , json3->getString("name"));
    });
}

void testFixBug1(){
    Test("Test object contain list" , [](){
        auto _obj = JsonObject::create();
        auto _list = JsonArray::create();
        _obj->putJsonArray("list" , _list);

        for(int i = 0; i < 5 ;i++){
            auto _item = JsonObject::create();
            _item->putString("value" , L"X");
            _item->putInt("width" , 100);
            _item->putInt("height" , 200);
            _item->putString("texture" , L"test.png");

            _list->pushJsonObject(_item);
        }//end for i

        std::wstring str = _obj->toJsonString();

        WriteStringToFile("out.json" , str);

        JsonParser parser;
        auto json = parser.parseJsonObject(str);
        auto array = json->getJsonArray("list");
        Equal(5 , array->size());
    });

    Test("Test object contain list2" , [](){
        auto _obj = JsonObject::create();
        auto _list = JsonArray::create();
        _obj->putJsonArray("list" , _list);

        int len = 4;
        for(int i = 0; i < len ;i++){
            auto _item = JsonObject::create();
            _item->putString("value" , L"X");
            _item->putInt("width" , 100);
            _item->putInt("height" , 200);
            _item->putString("texture" , L"test.png");
            
            auto _arr = JsonArray::create();
            _arr->pushFloat(1.0f);
            _arr->pushFloat(2.0f);
            _arr->pushFloat(3.0f);
            _arr->pushFloat(4.0f);
            
            _item->putJsonArray("texCoords" , _arr);

            _list->pushJsonObject(_item);
        }//end for i

        std::wstring str = _obj->toJsonString();

        WriteStringToFile("out.json" , str);

        JsonParser parser;
        auto json = parser.parseJsonObject(str);
        auto array = json->getJsonArray("list");
        Equal(len , array->size());
    });

    Test("Test long json string" , [](){
        std::wstring str = ReadTextFileAsWstring("char_config.json");
        // std::cout << "str size " << str.size() << std::endl;
        JsonParser parser;
        auto json = parser.parseJsonObject(str);
        auto jsonList = json->getJsonArray("list");
        // std::cout << "jsonList size " << jsonList->size() << std::endl;

        for(int i = 0 ; i < jsonList->size();i++){
            auto itemJson = jsonList->getJsonObject(i);
            EqualWString(L"font_texture_0.png" , itemJson->getString("texture"));
            // std::wcout << itemJson->getString("texture") << std::endl;
        }//end for i
    });
}

int main(){
    testJsonObject();
    testJsonArray();
    testJsonParse();
    testJsonArrayParse();
    testFixBug1();


   

    utest.testAll();
    return 0;
}




