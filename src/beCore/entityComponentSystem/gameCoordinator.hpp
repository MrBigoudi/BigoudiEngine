#pragma once

#include "gameComponent.hpp"
#include "gameObject.hpp"
#include "gameSystem.hpp"

#include <memory>

namespace be{

/**
 * A static class representing that coordinates the ECS system
 * @see https://austinmorlan.com/posts/entity_component_system/
*/
class GameCoordinator{
    private:
        /**
         * A private constructor to make the class purely virtual
        */
        GameCoordinator(){};

    public:        
        // game object manager methods
        /**
         * Create an object
         * @return The id representing the newly created object
         * @see GameObject
        */
        static GameObject createObject();

        /**
         * Destroy a game object
         * @param object The object to destroy
         * @see GameObject
        */
        static void destroyObject(GameObject object);
        
        // game component manager methods
        /**
         * Register a new component
         * @tparam The type of the component
         * @see GameComponent
        */
        template<typename T>
        static void registerComponent(){
            GameComponentManager::registerComponent<T>();
        }

        /**
         * Add a component to an object
         * @tparam The type of the component
         * @param object The current object
         * @param component The filled component to add to the object
         * @see GameComponent
        */
        template<typename T>
        static void addComponent(GameObject object, T component){
            GameComponentManager::addComponent(object, component);
            auto signature = GameObjectManager::getSignature(object);
            signature.set(GameComponentManager::getComponentType<T>(), true);
            GameObjectManager::setSignature(object, signature);
            GameSystemManager::objectSignatureChanged(object, signature);
        }

        /**
         * Remove a component to an object
         * @tparam The type of the component
         * @param object The current object
         * @see GameComponent
        */
        template<typename T>
        static void removeComponent(GameObject object){
            GameComponentManager::removeComponent<T>(object);
            auto signature = GameObjectManager::getSignature(object);
            signature.set(GameComponentManager::getComponentType<T>(), false);
            GameObjectManager::setSignature(object, signature);
            GameSystemManager::objectSignatureChanged(object, signature);
        }

        /**
         * Get the value of the given object's component
         * @tparam The type of the component
         * @param object The current object
         * @return The content of the component
         * @see GameComponent
        */
        template<typename T>
        static T& getComponent(GameObject object){
            return GameComponentManager::getComponent<T>(object);
        }

        /**
         * Return the component's type - used for creating signatures
         * @tparam The type of the component
         * @return The component's type
         * @see GameComponent
        */
        template<typename T>
        static GameComponentType getComponentType(){
            return GameComponentManager::getComponentType<T>();
        }


        // game system manager methods
        /**
         * Register a system
         * @tparam The type of the system
         * @return A smart pointer to the registered system
         * @see GameSystem
        */
        template<typename T>
        static std::shared_ptr<T> registerSystem(){
            return GameSystemManager::registerSystem<T>();
        }

        /**
         * Set the system signature
         * @tparam The type of the system
         * @param signature The signature to give to the objects in the current system
         * @see GameSystem
        */
        template<typename T>
        static void setSystemSignature(GameObjectSignature signature){
            GameSystemManager::setSignature<T>(signature);
        }

};

};