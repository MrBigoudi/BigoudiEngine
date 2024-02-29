#pragma once

#include "be_buffer.hpp"

namespace be{

struct UboData{};

class UboContainer{
    protected:
        std::vector<be::BufferPtr> _Ubos{};
        uint32_t _Size = 0;

    protected:
        void checkFrameIndex(uint32_t frameIndex){
            if(frameIndex >= _Size){
                ErrorHandler::handle(__FILE__, __LINE__, 
                    ErrorCode::OUT_OF_RANGE_ERROR,
                    "Can't access frame index " 
                    + std::to_string(frameIndex)
                    + ", ubo container is too small!\n"
                );
            } 
        }

    public:
        virtual void init(uint32_t size, VulkanAppPtr vulkanApp[[maybe_unused]]){
            _Size = size;
            _Ubos.resize(size);
        };

        virtual void update(uint32_t frameIndex) = 0;

        virtual void cleanUp(){
            for(uint32_t i=0; i<_Size; i++){
                if(_Ubos[i]){
                    _Ubos[i]->cleanUp();
                }
            }
        }

        VkDescriptorBufferInfo getDescriptorInfo(uint32_t frameIndex){
            checkFrameIndex(frameIndex);
            return _Ubos[frameIndex]->descriptorInfo();
        }
};

}