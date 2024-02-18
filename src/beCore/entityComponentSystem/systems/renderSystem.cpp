#include "renderSystem.hpp"
#include "components.hpp" // IWYU pragma: keep
#include "gameCoordinator.hpp"

namespace be{

RenderSystemPtr RenderSystem::_System = nullptr;

RenderSystemPtr RenderSystem::get(){
    if(!_System){
        init();
    }
    return _System;
}

void RenderSystem::renderGameObjects(FrameInfo frameInfo, IRenderSubSystem* renderSubSystem){
    renderSubSystem->getPipeline()->bind(frameInfo._CommandBuffer);

    vkCmdBindDescriptorSets(
        frameInfo._CommandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        renderSubSystem->getPipelineLayout(),
        0,
        static_cast<uint32_t>(renderSubSystem->getDescriptorSets().size()),
        renderSubSystem->getDescriptorSets().data(),
        0,
        nullptr
    );

    auto instance = get();

    for(auto const& object : instance->_Objects){
        auto rss = be::GameCoordinator::getComponent<be::ComponentRenderSubSystem>(object);
        if(rss._RenderSubSystem.get() == renderSubSystem){
            renderSubSystem->renderingFunction(object);
        }
    }
        
}

void RenderSystem::init(){
    if(_System != nullptr){
        ErrorHandler::handle(
            ErrorCode::BAD_VALUE_ERROR,
            "The render system has already been initialized!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    _System = GameCoordinator::registerSystem<be::RenderSystem>();
    if(_System == nullptr){
        ErrorHandler::handle(
            ErrorCode::NOT_INITIALIZED_ERROR,
            "Failed to initialize the ECS render system!\n"
        );
    }
    GameObjectSignature signature;
    signature.set(be::GameCoordinator::getComponentType<ComponentModel>());
    signature.set(be::GameCoordinator::getComponentType<ComponentTransform>());
    signature.set(be::GameCoordinator::getComponentType<ComponentRenderSubSystem>());
    GameCoordinator::setSystemSignature<be::RenderSystem>(signature);
}

};