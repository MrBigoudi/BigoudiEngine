#include "frontEnd.hpp"

namespace beRenderer{


/**
 * Initiate the frontend
*/
void FrontEnd::init(){

}

/**
 * Shut down the frontend
*/
void FrontEnd::shutdown(){

}

/**
 * Render using the packet infos
 * @param packet The packet containing info about the rendering
 * @param backend The backend to use
 * @see frontEndTypes
*/
void FrontEnd::render(const RenderPacket& packet, const BackEndPtr& backend){
    backend->render(packet);
}

}