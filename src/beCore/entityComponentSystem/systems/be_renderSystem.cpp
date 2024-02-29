#include "be_renderSystem.hpp"
#include "be_components.hpp" // IWYU pragma: keep
#include "be_gameCoordinator.hpp"

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

    if(!renderSubSystem->needPerObjectBind()){
        renderSubSystem->cmdBindDescriptoSets(frameInfo);
    }

    auto instance = get();

    for(auto const& object : instance->_Objects){
        auto rss = GameCoordinator::getComponent<ComponentRenderSubSystem>(object);
        if(rss._RenderSubSystem.get() == renderSubSystem){
            if(renderSubSystem->needPerObjectBind()){
                renderSubSystem->updateDescriptorSets(object, frameInfo);
                renderSubSystem->cmdBindDescriptoSets(frameInfo);
            }
            renderSubSystem->renderingFunction(object);
        }
    }
        
}

void RenderSystem::init(){
    if(_System != nullptr){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::BAD_VALUE_ERROR,
            "The render system has already been initialized!\n",
            ErrorLevel::WARNING
        );
        return;
    }
    _System = GameCoordinator::registerSystem<RenderSystem>();
    if(_System == nullptr){
        ErrorHandler::handle(__FILE__, __LINE__, 
            ErrorCode::NOT_INITIALIZED_ERROR,
            "Failed to initialize the ECS render system!\n"
        );
    }
    GameObjectSignature signature;
    signature.set(GameCoordinator::getComponentType<ComponentModel>());
    signature.set(GameCoordinator::getComponentType<ComponentTransform>());
    signature.set(GameCoordinator::getComponentType<ComponentRenderSubSystem>());
    signature.set(GameCoordinator::getComponentType<ComponentMaterial>());
    GameCoordinator::setSystemSignature<RenderSystem>(signature);
}

GameObject RenderSystem::createRenderableObject(
        ComponentRenderSubSystem renderSubSystem,
        ComponentModel model,
        ComponentTransform transform,
        ComponentMaterial material
    ){

    auto newGameObject = GameCoordinator::createObject();
    GameCoordinator::addComponent(
        newGameObject, 
        model
    );
    GameCoordinator::addComponent(
        newGameObject, 
        transform
    );
    GameCoordinator::addComponent(
        newGameObject, 
        material
    );
    GameCoordinator::addComponent(
        newGameObject, 
        renderSubSystem
    );

    return newGameObject;
}

};