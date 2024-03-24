#include "be_scene.hpp"
#include "be_gameCoordinator.hpp" // IWYU pragma: keep
#include "be_components.hpp" // IWYU pragma: keep
#include "be_renderSystem.hpp"

namespace be{

/**
 * Add a game object to the scene
 * @param obj The object to add
*/
void Scene::addGameObject(GameObject obj){
    // do not add twice the same object
    for(auto& curObj : _GameObjects){
        if(curObj == obj){
            ErrorHandler::handle(
                __FILE__, __LINE__,
                ErrorCode::BAD_VALUE_ERROR,
                "Can't add twice the same object to the scene",
                ErrorLevel::WARNING
            );
            return;
        }
    }
    _GameObjects.push_back(obj);
}

/**
* Add a point light to the scene
* @param light The light to add
*/
void Scene::addGamePointLight(PointLightPtr light){
    // do not add twice the same light
    for(auto& curPointLight : _PointLights){
        if(curPointLight == light){
            ErrorHandler::handle(
                __FILE__, __LINE__,
                ErrorCode::BAD_VALUE_ERROR,
                "Can't add twice the same point light to the scene",
                ErrorLevel::WARNING
            );
            return;
        }
    }
    _PointLights.push_back(light);
}

/**
* Add a directional light to the scene
* @param light The light to add
*/
void Scene::addGameDirectionalLight(DirectionalLightPtr light){
    // do not add twice the same light
    for(auto& curDirectionalLight : _DirectionalLights){
        if(curDirectionalLight == light){
            ErrorHandler::handle(
                __FILE__, __LINE__,
                ErrorCode::BAD_VALUE_ERROR,
                "Can't add twice the same directional light to the scene",
                ErrorLevel::WARNING
            );
            return;
        }
    }
    _DirectionalLights.push_back(light);
}

/**
* Add a point light to the scene
* @param position The light position
* @param color The light color
* @param intensity The light intensity
*/
void Scene::addGamePointLight(const Vector3& position, const Vector3& color, float intensity, bool makeVisible){
    PointLightPtr newLight = PointLightPtr(new PointLight());
    newLight->_Color = Vector4(color, 1.f);
    newLight->_Intensity = intensity;
    newLight->_Position = Vector4(position, 1.f);
    _PointLights.push_back(newLight);
    if(makeVisible){
        // TODO:
    }
}

/**
* Add a directional light to the scene
* @param direction The light direction
* @param color The light color
* @param intensity The light intensity
*/
void Scene::addGameDirectionalLight(const Vector3& direction, const Vector3& color, float intensity){
    DirectionalLightPtr newLight = DirectionalLightPtr(new DirectionalLight());
    newLight->_Color = Vector4(color, 1.f);
    newLight->_Intensity = intensity;
    newLight->_Direction = Vector4(direction, 0.f);
    _DirectionalLights.push_back(newLight);
}

/**
* Getter to the scene point lights
* @return The vector of point lights
*/
std::vector<PointLightPtr> Scene::getPointLights() const {
    return _PointLights;
}

/**
* Getter to the scene directional lights
* @return The vector of directional lights
*/
std::vector<DirectionalLightPtr> Scene::getDirectionalLights() const {
    return _DirectionalLights;
}




GameObject Scene::addCubeOfLight(IRenderSubSystemPtr rss, const Vector3& center, float length, const Vector3& color, float intensity, const Vector3& rotation, float step){
    float halfLength = length / 2.f;
    // left square light
    Vector3 topLeftCorner = center - halfLength*Vector3::ones();
    Vector3 bottomRightCorner = center + halfLength*Vector3::ones();
    for(float dx=step; dx<std::fabs(bottomRightCorner.x() - topLeftCorner.x()); dx+=step){
        for(float dy=step; dy<std::fabs(bottomRightCorner.y() - topLeftCorner.y()); dy+=step){
            for(float dz=step; dz<std::fabs(bottomRightCorner.z() - topLeftCorner.z()); dz+=step){
                Vector3 pos = {topLeftCorner.x() + dx, topLeftCorner.y() + dy, topLeftCorner.z() + dz};
                addGamePointLight(
                    pos,
                    color,
                    intensity
                );
            }
        }
    }


    // make the light visible
    ModelPtr model = ModelPtr(
        new Model(_VulkanApp, 
            VertexDataBuilder::primitiveCube(
                length, Vector4(color, 1.f)
            )
        )
    );

    TransformPtr transform = TransformPtr(
        new Transform()
    );
    transform->_Position = center;
    transform->_Rotation = rotation;

    be::GameObject object = RenderSystem::createRenderableObject(
        {._RenderSubSystem = rss},
        {._Model = model}, 
        {._Transform = transform},
        {},
        {._IsLight = true}
    );
    _GameObjects.push_back(object);
    return object;
}

GameObject Scene::addCubeOfLight(IRenderSubSystemPtr rss, const Vector3& center, const Vector3& scale, const Vector3& color, float intensity, const Vector3& rotation, float step){
    float halfX = scale.x() / 2.f;
    float halfY = scale.y() / 2.f;
    float halfZ = scale.z() / 2.f;
    // left square light
    Vector3 topLeftCorner = center - Vector3(halfX, halfY, halfZ)*Vector3::ones();
    Vector3 bottomRightCorner = center + Vector3(halfX, halfY, halfZ)*Vector3::ones();
    for(float dx=step; dx<std::fabs(bottomRightCorner.x() - topLeftCorner.x()); dx+=step){
        for(float dy=step; dy<std::fabs(bottomRightCorner.y() - topLeftCorner.y()); dy+=step){
            for(float dz=step; dz<std::fabs(bottomRightCorner.z() - topLeftCorner.z()); dz+=step){
                Vector3 pos = {topLeftCorner.x() + dx, topLeftCorner.y() + dy, topLeftCorner.z() + dz};
                addGamePointLight(
                    pos,
                    color,
                    intensity
                );
            }
        }
    }


    // make the light visible
    ModelPtr model = ModelPtr(
        new Model(_VulkanApp, 
            VertexDataBuilder::primitiveCube(
                1.f,
                Vector4(color, 1.f)
            )
        )
    );

    TransformPtr transform = TransformPtr(
        new Transform()
    );
    transform->_Position = center;
    transform->_Rotation = rotation;
    transform->_Scale = scale;

    be::GameObject object = RenderSystem::createRenderableObject(
        {._RenderSubSystem = rss},
        {._Model = model}, 
        {._Transform = transform},
        {},
        {._IsLight = true}
    );
    _GameObjects.push_back(object);
    return object;
}


GameObject Scene::addCubeOfLight(IRenderSubSystemPtr rss, const Vector3& center, const Vector3& scale, const Vector3& color, float intensity, const Vector3& rotation, const Vector3& step){
    float halfX = scale.x() / 2.f;
    float halfY = scale.y() / 2.f;
    float halfZ = scale.z() / 2.f;
    // left square light
    Vector3 topLeftCorner = center - Vector3(halfX, halfY, halfZ)*Vector3::ones();
    Vector3 bottomRightCorner = center + Vector3(halfX, halfY, halfZ)*Vector3::ones();
    for(float dx=step.x(); dx<std::fabs(bottomRightCorner.x() - topLeftCorner.x()); dx+=step.x()){
        for(float dy=step.y(); dy<std::fabs(bottomRightCorner.y() - topLeftCorner.y()); dy+=step.y()){
            for(float dz=step.z(); dz<std::fabs(bottomRightCorner.z() - topLeftCorner.z()); dz+=step.z()){
                Vector3 pos = {topLeftCorner.x() + dx, topLeftCorner.y() + dy, topLeftCorner.z() + dz};
                addGamePointLight(
                    pos,
                    color,
                    intensity
                );
            }
        }
    }


    // make the light visible
    ModelPtr model = ModelPtr(
        new Model(_VulkanApp, 
            VertexDataBuilder::primitiveCube(
                1.f,
                Vector4(color, 1.f)
            )
        )
    );

    TransformPtr transform = TransformPtr(
        new Transform()
    );
    transform->_Position = center;
    transform->_Rotation = rotation;
    transform->_Scale = scale;

    be::GameObject object = RenderSystem::createRenderableObject(
        {._RenderSubSystem = rss},
        {._Model = model}, 
        {._Transform = transform},
        {},
        {._IsLight = true}
    );
    _GameObjects.push_back(object);
    return object;
}

}