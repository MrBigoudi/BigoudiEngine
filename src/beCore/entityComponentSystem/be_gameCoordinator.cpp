#include "be_gameCoordinator.hpp"

#include "be_gameComponent.hpp"
#include "be_gameObject.hpp"
#include "be_gameSystem.hpp"
        
namespace be{

// game object manager methods
GameObject GameCoordinator::createObject(){
    return GameObjectManager::createObject();
}

void GameCoordinator::destroyObject(GameObject object){
    GameObjectManager::destroyObject(object);
    GameComponentManager::gameObjectDestroyed(object);
    GameSystemManager::gameObjectDestroyed(object);
}

};