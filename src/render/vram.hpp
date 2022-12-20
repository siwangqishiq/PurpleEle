#pragma once

#include "glheader.hpp"
#include <memory>
#include <vector>
#include "log.hpp"

struct MemoryAllocatorInfo{
    unsigned int bufferId = -1;
    unsigned int vao = -1;
    int size = 0;
    int offset = 0;
};

//显存分配器
class VRamAllcator{
private:
    std::vector<std::shared_ptr<MemoryAllocatorInfo>> allocatedList_; //已分配的显存记录表

    unsigned int currentBufferIdIndex_ = -1;
    
    unsigned int genBuffer();

    void createNewBuffer();
public:
    const int ALLOCATOR_SIZE = 2 * 1024 * 1024;//一次分配2M缓存

    int fetchVideoMemory(int requestSize ,
            unsigned int &bufferId , unsigned int &vao,
            int &offset , int &size);

    void recycleAllMemory();

    void freeMemory();
};

//显存管理
class VRamManager{
public:
    const char *TAG = "VRamManager";
    
    static std::shared_ptr<VRamManager> getInstance();
    
    static std::shared_ptr<VRamManager> instance_;

    VRamManager();

    ~VRamManager(){
        Logi(TAG , "~VRamManager deconstruct");
    }

    void onPostRender();

    void clear();

    //获取一块显存 用于写入内容
    int fetchVideoMemory(int requestSize , 
            unsigned int &bufferId , 
            unsigned int &vao,
            int &offset , int &size);
private:
    std::shared_ptr<VRamAllcator> allocator_;
};



