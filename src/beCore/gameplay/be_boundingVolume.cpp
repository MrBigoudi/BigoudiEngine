#include "be_boundingVolume.hpp"
#include <cassert>

namespace be{

Bounding::Bounding(const std::vector<Triangle>& triangles){
    if(triangles.empty()){
        ErrorHandler::handle(
            __FILE__, __LINE__,
            ErrorCode::BAD_VALUE_ERROR,
            "Can't create a bounding sphere given an empty triangle list!\n"
        );
    }
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const std::vector<Triangle>& triangles)
    :Bounding(triangles){
    // get the corners
    _MinX = triangles[0]._WorldPos0.x();
    _MaxX = triangles[0]._WorldPos0.x();
    _MinY = triangles[0]._WorldPos0.y();
    _MaxY = triangles[0]._WorldPos0.y();
    _MinZ = triangles[0]._WorldPos0.z();
    _MaxZ = triangles[0]._WorldPos0.z();

    for(auto triangle : triangles){
        //p0
        Vector3 p0 = triangle._WorldPos0;
        if(p0.x() < _MinX){_MinX = p0.x();}
        if(p0.x() > _MaxX){_MaxX = p0.x();}
        if(p0.y() < _MinY){_MinY = p0.y();}
        if(p0.y() > _MaxY){_MaxY = p0.y();}
        if(p0.z() < _MinZ){_MinZ = p0.z();}
        if(p0.z() > _MaxZ){_MaxZ = p0.z();}

        //p1
        Vector3 p1 = triangle._WorldPos1;
        if(p1.x() < _MinX){_MinX = p1.x();}
        if(p1.x() > _MaxX){_MaxX = p1.x();}
        if(p1.y() < _MinY){_MinY = p1.y();}
        if(p1.y() > _MaxY){_MaxY = p1.y();}
        if(p1.z() < _MinZ){_MinZ = p1.z();}
        if(p1.z() > _MaxZ){_MaxZ = p1.z();}

        //p2
        Vector3 p2 = triangle._WorldPos2;
        if(p2.x() < _MinX){_MinX = p2.x();}
        if(p2.x() > _MaxX){_MaxX = p2.x();}
        if(p2.y() < _MinY){_MinY = p2.y();}
        if(p2.y() > _MaxY){_MaxY = p2.y();}
        if(p2.z() < _MinZ){_MinZ = p2.z();}
        if(p2.z() > _MaxZ){_MaxZ = p2.z();}
    }
}

Vector3 AxisAlignedBoundingBox::getCenter() const{
    Vector3 min = Vector3(_MinX, _MinY, _MinZ);
    Vector3 max = Vector3(_MaxX, _MaxY, _MaxZ);
    return (max + min) / 2.f;
}

float AxisAlignedBoundingBox::getDiagonalLength() const{
    Vector3 min = Vector3(_MinX, _MinY, _MinZ);
    Vector3 max = Vector3(_MaxX, _MaxY, _MaxZ);
    return (max - min).getNorm();
}

std::string AxisAlignedBoundingBox::toString() const {
    return "{ minX: " + std::to_string(_MinX) + ", maxX: " + std::to_string(_MaxX)
        + ", minY: " + std::to_string(_MinY) + ", maxY: " + std::to_string(_MaxY)
        + ", minZ: " + std::to_string(_MinZ) + ", maxZ: " + std::to_string(_MaxZ)
        + " }";
}

std::string BoundingSphere::toString() const {
    return "{ center: " + _Center.toString() + ", radius: " + std::to_string(_Radius) + " }";
}

AxisAlignedBoundingBox AxisAlignedBoundingBox::merge(const AxisAlignedBoundingBox& aabb1, const AxisAlignedBoundingBox& aabb2){
    return AxisAlignedBoundingBox(
        std::min(aabb1._MinX, aabb2._MinX),
        std::max(aabb1._MaxX, aabb2._MaxX),
        std::min(aabb1._MinY, aabb2._MinY),
        std::max(aabb1._MaxY, aabb2._MaxY),
        std::min(aabb1._MinZ, aabb2._MinZ),
        std::max(aabb1._MaxZ, aabb2._MaxZ)
    );
}

AxisAlignedBoundingBox::Axis AxisAlignedBoundingBox::getDominantAxis() const{
    float distX = getDistance(Axis::X);
    float distY = getDistance(Axis::Y);
    float distZ = getDistance(Axis::Z);

    // case X axis 
    if(distX >= distY && distX >= distZ){
        return Axis::X;
    }

    // case Y axis 
    if(distY >= distX && distY >= distZ){
        return Axis::Y;
    }
    
    // case Z axis 
    if(distZ >= distX && distZ >= distY){
        return Axis::Z;
    }

    return Axis::X;
}

Vector3 AxisAlignedBoundingBox::getDominantAxisPlaneNormal() const{
    auto dominantAxis = getDominantAxis();
    switch(dominantAxis){
        case Axis::X:
            return {1.f, 0.f, 0.f};
        case Axis::Y:
            return {0.f, 1.f, 0.f};
        case Axis::Z:
            return {0.f, 0.f, 1.f};
    }
    assert(false);
}


float AxisAlignedBoundingBox::getDistance(AxisAlignedBoundingBox::Axis axis) const{
    switch(axis){
        case Axis::X:
            return _MaxX - _MinX;
        case Axis::Y:
            return _MaxY - _MinY;
        case Axis::Z:
            return _MaxZ - _MinZ;
    }
    ErrorHandler::handle(
        __FILE__, __LINE__, 
        ErrorCode::UNEXPECTED_VALUE_ERROR,
        "The given AABB axis is not recognized!\n"
    );
    return -1.f;
}

BoundingSphere::BoundingSphere(const AxisAlignedBoundingBox& aabb){
    _Center = aabb.getCenter();
    auto dominantAxis = aabb.getDominantAxis();
    _Radius = aabb.getDistance(dominantAxis) / 2.f;
}


BoundingSphere::BoundingSphere(const std::vector<Triangle>& triangles)
    :BoundingSphere(AxisAlignedBoundingBox(triangles)){
}


BSH::BSHNode::BSHNode(const std::vector<Triangle>& triangles, const std::vector<uint32_t>& indices, uint32_t depth)
    :_TriangleIndices(indices){

    auto aabb = AxisAlignedBoundingBox(triangles);
    _Sphere = BoundingSpherePtr(new BoundingSphere(aabb));
    // fprintf(stdout, "depth=%d, aabb:%s, sphere:%s\n", depth, aabb.toString().c_str(), _Sphere->toString().c_str());

    // only one element
    if(triangles.size() == 1){return;}
    // two elements
    if(triangles.size() == 2){
        _LeftChild = BSHNodePtr(new BSHNode({triangles[0]}, {indices[0]}, depth+1));
        _RightChild = BSHNodePtr(new BSHNode({triangles[1]}, {indices[1]}, depth+1));
        return;
    }


    // split triangles in 2 lists
    std::vector<Triangle> leftList{};
    std::vector<uint32_t> leftIndicesList{};
    std::vector<Triangle> rightList{};
    std::vector<uint32_t> rightIndicesList{};

    // get splitting plane
    Vector3 planeNormal = aabb.getDominantAxisPlaneNormal(); // random in unit sphere
    for(uint32_t k = 0; k<triangles.size(); k++){
        auto& triangle = triangles[k];
        uint32_t index = indices[k];
        if(triangle.isWorldP0LeftOfPlane(_Sphere->_Center, planeNormal)){
            leftList.push_back(triangle);
            leftIndicesList.push_back(index);
        } else {
            rightList.push_back(triangle);
            rightIndicesList.push_back(index);
        }
    }

    assert(triangles.size() == leftList.size() + rightList.size());
    // if can't split triangles, stop bsh
    if(leftList.size() == 0 || rightList.size() == 0){return;}

    // create children
    _LeftChild = BSHNodePtr(new BSHNode(leftList, leftIndicesList, depth+1));
    _RightChild = BSHNodePtr(new BSHNode(rightList, rightIndicesList, depth+1));
}


BSH::BSHTreePtr BSH::BSHTree::init(const std::vector<Triangle>& triangles){
    BSHTreePtr tree = BSHTreePtr(new BSHTree());
    std::vector<uint32_t> indicesList(triangles.size());
    for(uint32_t i=0; i<triangles.size(); i++){
        indicesList[i] = i;
    }
    tree->_Root = BSHNodePtr(new BSHNode(triangles, indicesList));
    return tree;
}

BSH::BSH(const std::vector<Triangle>& triangles): _Triangles(triangles){
    _Tree = BSHTree::init(triangles);
}

void BSH::BSHNode::getIntersections(const std::vector<Triangle>& triangles, const RayPtr& ray, const Vector3& cameraPos, RayHits& hits) const{
    if(ray->raySphereIntersection(_Sphere->_Center, _Sphere->_Radius)){
        if(isLeaf()){
            for(uint32_t triangleIndex : _TriangleIndices){
                auto& triangle = triangles[triangleIndex];
                RayHitOpt hit = ray->rayTriangleIntersection(triangle);
                if(hit.has_value()){
                    hit->setDistanceToPov(cameraPos);
                    hits.addHit(hit.value());
                }
            }
        } else {
            _LeftChild->getIntersections(triangles, ray, cameraPos, hits);
            _RightChild->getIntersections(triangles, ray, cameraPos, hits);
        }
    }
}





BVH::BVHNode::BVHNode(const std::vector<Triangle>& triangles, const std::vector<uint32_t>& indices, uint32_t depth)
    :_TriangleIndices(indices){

    _AABB = AxisAlignedBoundingBoxPtr(new AxisAlignedBoundingBox(triangles));

    // only one element
    if(triangles.size() == 1){return;}
    // two elements
    if(triangles.size() == 2){
        _LeftChild = BVHNodePtr(new BVHNode({triangles[0]}, {indices[0]}, depth+1));
        _RightChild = BVHNodePtr(new BVHNode({triangles[1]}, {indices[1]}, depth+1));
        return;
    }


    // split triangles in 2 lists
    std::vector<Triangle> leftList{};
    std::vector<uint32_t> leftIndicesList{};
    std::vector<Triangle> rightList{};
    std::vector<uint32_t> rightIndicesList{};

    // get splitting plane
    Vector3 planeNormal = _AABB->getDominantAxisPlaneNormal(); // random in unit sphere
    for(uint32_t k = 0; k<triangles.size(); k++){
        auto& triangle = triangles[k];
        uint32_t index = indices[k];
        if(triangle.isWorldP0LeftOfPlane(_AABB->getCenter(), planeNormal)){
            leftList.push_back(triangle);
            leftIndicesList.push_back(index);
        } else {
            rightList.push_back(triangle);
            rightIndicesList.push_back(index);
        }
    }

    assert(triangles.size() == leftList.size() + rightList.size());
    // if can't split triangles, stop bvh
    if(leftList.size() == 0 || rightList.size() == 0){return;}

    // create children
    _LeftChild = BVHNodePtr(new BVHNode(leftList, leftIndicesList, depth+1));
    _RightChild = BVHNodePtr(new BVHNode(rightList, rightIndicesList, depth+1));
}


BVH::BVHTreePtr BVH::BVHTree::init(const std::vector<Triangle>& triangles){
    BVHTreePtr tree = BVHTreePtr(new BVHTree());
    std::vector<uint32_t> indicesList(triangles.size());
    for(uint32_t i=0; i<triangles.size(); i++){
        indicesList[i] = i;
    }
    tree->_Root = BVHNodePtr(new BVHNode(triangles, indicesList));
    return tree;
}

BVH::BVH(const std::vector<Triangle>& triangles): _Triangles(triangles){
    _Tree = BVHTree::init(triangles);
}

void BVH::BVHNode::getIntersections(const std::vector<Triangle>& triangles, const RayPtr& ray, const Vector3& cameraPos, RayHits& hits) const{
    if(ray->rayBoxIntersection(_AABB->_MinX, _AABB->_MaxX, _AABB->_MinY, _AABB->_MaxY, _AABB->_MinZ, _AABB->_MaxZ)){
        if(isLeaf()){
            for(uint32_t triangleIndex : _TriangleIndices){
                auto& triangle = triangles[triangleIndex];
                RayHitOpt hit = ray->rayTriangleIntersection(triangle);
                if(hit.has_value()){
                    hit->setDistanceToPov(cameraPos);
                    hits.addHit(hit.value());
                }
            }
        } else {
            _LeftChild->getIntersections(triangles, ray, cameraPos, hits);
            _RightChild->getIntersections(triangles, ray, cameraPos, hits);
        }
    }
}


}

