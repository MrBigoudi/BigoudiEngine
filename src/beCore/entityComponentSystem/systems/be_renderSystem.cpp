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
        auto rss = be::GameCoordinator::getComponent<be::ComponentRenderSubSystem>(object);
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
    signature.set(be::GameCoordinator::getComponentType<ComponentMaterial>());
    GameCoordinator::setSystemSignature<be::RenderSystem>(signature);
}

GameObject RenderSystem::createRenderableObject(
        ComponentModel model,
        ComponentTransform transform,
        ComponentMaterial material,
        ComponentRenderSubSystem renderSubSystem
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