#pragma once

#include "be_gameObject.hpp"
#include "be_transform.hpp"
#include "be_errorHandler.hpp"

#include <cstdint>

namespace be{

/**
 * A type representing a collider layer
*/
using ColliderLayer = uint32_t;

/**
 * Forward declaration of the collider class
 * @see Collider
*/
class Collider;

/**
 * A smart pointer to a collider
*/
using ColliderPtr = std::shared_ptr<Collider>;

enum ColliderType{
    NONE,
    BOX_COLLIDER,
};

class BoxCollider;

/**
 * A class representing a collider
*/
class Collider{
    public:
        const static uint32_t NB_COLLIDER_LAYERS = 8;
        static std::array<std::array<bool, NB_COLLIDER_LAYERS>, NB_COLLIDER_LAYERS> _ColliderLayerMatrix;

    public:
        /**
        * The id of the collision layer
        */
        ColliderLayer _Layer = 0;

        /**
        * The collider transform
        */
        Transform _Transform{};

    protected:
        ColliderType _Type = NONE;


    private:
        /**
         * Check if a layer is correct
         * @param layer The layer to check
        */
        static void isLayerCorrect(ColliderLayer layer){
            if(layer >= NB_COLLIDER_LAYERS){
                ErrorHandler::handle(
                    ErrorCode::BAD_VALUE_ERROR,
                    "Can't initiate a collider with a layer greater than "
                    + std::to_string(NB_COLLIDER_LAYERS)
                    + ", given layer id = "
                    + std::to_string(layer)
                    + " !\n"
                );
            }
        }

    protected:
        /**
         * A basic constructor
         * @param layer The layer id
         * @param transform The transform
        */
        Collider(ColliderLayer layer, Transform transform)
            : _Layer(layer), _Transform(transform){
            isLayerCorrect(layer);
        };

    public:
        /**
         * Set the collision entry for a given layer
         * @param layer The current layer
         * @param collisionLayers The array of collision layers
         * @see _ColliderLayerMatrix
        */
        static void setCollisionLayerEntry(ColliderLayer layer, const std::vector<ColliderLayer>& collisionLayers){
            for(ColliderLayer l = 0; l<NB_COLLIDER_LAYERS; l++){
                if(std::find(collisionLayers.begin(), collisionLayers.end(), l) != collisionLayers.end()){
                    _ColliderLayerMatrix[layer][l] = true;
                    _ColliderLayerMatrix[l][layer] = true;
                } else {
                    _ColliderLayerMatrix[layer][l] = false;
                    _ColliderLayerMatrix[l][layer] = false;
                }
            }
        }

        ColliderType getType() const {
            return _Type;
        }

        /**
         * Tells if two collider can collide
        */
        bool canCollide(ColliderPtr collider){
            return collider && _ColliderLayerMatrix[_Layer][collider->_Layer];
        }

        /**
         * A function to test if two collider objects are in collision
         * @param object The current object
        */
        virtual bool isColliding(GameObject object) = 0;

    protected:
        static bool collidingBoxVsBox(const BoxCollider* box1, const BoxCollider* box2);
};

class BoxCollider: public Collider{
    public:
        /**
         * A basic constructor
         * @param layer The layer id
         * @param transform The transform
        */
        BoxCollider(ColliderLayer layer, Transform transform)
            : Collider(layer, transform){
            _Type = BOX_COLLIDER;
        };

    public:
        /**
         * A function to test if two collider objects are in collision
         * @param object The current object
        */
        virtual bool isColliding(GameObject object) override;
};

}