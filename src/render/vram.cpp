#include "vram.hpp"
#include "log.hpp"
#include "glheader.hpp"

VRamManager& VRamManager::getInstance() {
    static VRamManager instance;
    return instance;
}

VRamManager::VRamManager(){
    Logi(TAG , "vram manager construct");
    allocator_ = std::make_shared<VRamAllcator>();
}

int VRamManager::fetchVideoMemory(int requestSize ,
        unsigned int &bufferId ,
        unsigned int &vao, 
        int &offset , int &size){
    if(allocator_ != nullptr){
        return allocator_ ->fetchVideoMemory(requestSize ,
                    bufferId ,vao , offset , size);
    }
    return 0;
}

void VRamManager::onPostRender(){
    if(allocator_ != nullptr){
        allocator_->recycleAllMemory();
    }
}

void VRamManager::clear(){
    if(allocator_ != nullptr){
        allocator_->freeMemory();
    }
}

int VRamAllcator::fetchVideoMemory(int requestSize ,
            unsigned int &bufferId ,
            unsigned int &vao,
            int &offset , int &size){
    if(requestSize > ALLOCATOR_SIZE){
        Logi("VRamManager" , "allocator %d too large for allocator" , requestSize);
        return -1;
    }

    std::shared_ptr<MemoryAllocatorInfo> mInfo = nullptr;
    
    bool haveFit = false;
    while(currentBufferIdIndex_ < allocatedList_.size()) {
        mInfo = allocatedList_[currentBufferIdIndex_];
        if(mInfo->offset + requestSize <= mInfo->size){
            haveFit = true;
            break;
        }
        currentBufferIdIndex_++;
    } //end while

    if(!haveFit || mInfo == nullptr){
        createNewBuffer();
        currentBufferIdIndex_ = allocatedList_.size() - 1;
        mInfo = allocatedList_[currentBufferIdIndex_];
    }

    bufferId = mInfo->bufferId;
    vao = mInfo->vao;
    offset = mInfo->offset;
    mInfo->offset += requestSize;
    size = requestSize;
    return 0;
}

void VRamAllcator::createNewBuffer(){
    auto newAllocatorBufferId = genBuffer();
    unsigned int vaoValue = -1;
    glGenVertexArrays(1 , &vaoValue);

    auto info = std::make_shared<MemoryAllocatorInfo>();
    info->bufferId = newAllocatorBufferId;
    info->vao = vaoValue;
    info->size = ALLOCATOR_SIZE;
    info->offset = 0;
    
    allocatedList_.push_back(info);
    Logi("allocator" , "create new buffer");
}

void VRamAllcator::recycleAllMemory(){
    currentBufferIdIndex_ = 0;
    for(auto info : allocatedList_){
        info->offset = 0;
    }
}

void VRamAllcator::freeMemory(){
    currentBufferIdIndex_ = -1;
    const int size = allocatedList_.size();
    unsigned int *delIds = new unsigned int[size];
    for(int i = 0 ; i < allocatedList_.size();i++){
        delIds[i] = allocatedList_[i]->bufferId;
    }//end for i
    glDeleteBuffers(size, delIds);
    delete[] delIds;
}

unsigned int VRamAllcator::genBuffer(){
    GLuint bufferIds[1];
    glGenBuffers(1 , bufferIds);
    
    auto result = bufferIds[0];
    glBindBuffer(GL_ARRAY_BUFFER , result);
    glBufferData(GL_ARRAY_BUFFER , ALLOCATOR_SIZE , nullptr , GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
    
    return result;
}



