#pragma once

#include "be_model.hpp"
#include "be_ray.hpp"
#include "be_vector3.hpp"


namespace be{

class Bounding;
using BoundingPtr = std::shared_ptr<Bounding>;

class AxisAlignedBoundingBox;
using AxisAlignedBoundingBoxPtr = std::shared_ptr<AxisAlignedBoundingBox>;

class BoundingSphere;
using BoundingSpherePtr = std::shared_ptr<BoundingSphere>;

class BSH;
using BSHPtr = std::shared_ptr<BSH>;

class BVH;
using BVHPtr = std::shared_ptr<BVH>;


class Bounding{
    public:
        Bounding(const std::vector<Triangle>& triangles);
        Bounding(){};
};

class AxisAlignedBoundingBox : public Bounding{
    public:
        float _MinX = 0.f;
        float _MaxX = 0.f;
        float _MinY = 0.f;
        float _MaxY = 0.f;
        float _MinZ = 0.f;
        float _MaxZ = 0.f;

    public:
        enum Axis{
            X,Y,Z
        };

    public:
        AxisAlignedBoundingBox(const std::vector<Triangle>& triangles);

    public:
        Vector3 getCenter() const;
        Axis getDominantAxis() const;
        float getDistance(Axis axis) const;
        Vector3 getDominantAxisPlaneNormal() const;

        std::string toString() const {
            return "{ minX: " + std::to_string(_MinX) + ", maxX: " + std::to_string(_MaxX)
                + ", minY: " + std::to_string(_MinY) + ", maxY: " + std::to_string(_MaxY)
                + ", minZ: " + std::to_string(_MinZ) + ", maxZ: " + std::to_string(_MaxZ)
                + " }";
        }
};

class BoundingSphere : public Bounding{

    public:
        Vector3 _Center = Vector3();
        float _Radius = 0.f;

    public:
        BoundingSphere(const std::vector<Triangle>& triangles);
        BoundingSphere(const AxisAlignedBoundingBox& aabb);
        std::string toString() const {
            return "{ center: " + _Center.toString() + ", radius: " + std::to_string(_Radius) + " }";
        }
};


class BSH{
    private:
        class BSHNode;
        using BSHNodePtr = std::shared_ptr<BSHNode>;
        class BSHTree;
        using BSHTreePtr = std::shared_ptr<BSHTree>;

    private:
        // for building the tree
        class BSHNode{
            public:
                BoundingSpherePtr _Sphere = nullptr;
                std::vector<uint32_t> _TriangleIndices = {};
                BSHNodePtr _LeftChild = nullptr;
                BSHNodePtr _RightChild = nullptr;

            public:
                BSHNode(const std::vector<Triangle>& triangles, const std::vector<uint32_t>& indices, uint32_t depth = 0);
                bool isLeaf() const {return _LeftChild == nullptr && _RightChild == nullptr;}
                void getIntersections(const std::vector<Triangle>& triangles, const RayPtr& ray, const Vector3& cameraPos, RayHits& hits) const;
        };

        class BSHTree{
            public:
                BSHNodePtr _Root = nullptr;

            public:
                BSHTree(){};
                static BSHTreePtr init(const std::vector<Triangle>& triangles);

                RayHits getIntersections(const std::vector<Triangle>& triangles, const RayPtr& ray, const Vector3& cameraPos) const{
                    RayHits hits{};
                    _Root->getIntersections(triangles, ray, cameraPos, hits);
                    return hits;
                }
        };



    private:
        BSHTreePtr _Tree = nullptr;
        const std::vector<Triangle> _Triangles;

    public:
        BSH(const std::vector<Triangle>& triangles);

        RayHits getIntersections(const RayPtr& ray, const Vector3& cameraPos) const{
            if(_Triangles.empty()){return {};}
            return _Tree->getIntersections(_Triangles, ray, cameraPos);
        }

};


class BVH{
    private:
        class BVHNode;
        using BVHNodePtr = std::shared_ptr<BVHNode>;
        class BVHTree;
        using BVHTreePtr = std::shared_ptr<BVHTree>;

    private:
        // for building the tree
        class BVHNode{
            public:
                AxisAlignedBoundingBoxPtr _AABB = nullptr;
                std::vector<uint32_t> _TriangleIndices = {};
                BVHNodePtr _LeftChild = nullptr;
                BVHNodePtr _RightChild = nullptr;

            public:
                BVHNode(const std::vector<Triangle>& triangles, const std::vector<uint32_t>& indices, uint32_t depth = 0);
                bool isLeaf() const {return _LeftChild == nullptr && _RightChild == nullptr;}
                void getIntersections(const std::vector<Triangle>& triangles, const RayPtr& ray, const Vector3& cameraPos, RayHits& hits) const;
        };

        class BVHTree{
            public:
                BVHNodePtr _Root = nullptr;

            public:
                BVHTree(){};
                static BVHTreePtr init(const std::vector<Triangle>& triangles);

                RayHits getIntersections(const std::vector<Triangle>& triangles, const RayPtr& ray, const Vector3& cameraPos) const{
                    RayHits hits{};
                    _Root->getIntersections(triangles, ray, cameraPos, hits);
                    return hits;
                }
        };



    private:
        BVHTreePtr _Tree = nullptr;
        const std::vector<Triangle> _Triangles;

    public:
        BVH(const std::vector<Triangle>& triangles);

        RayHits getIntersections(const RayPtr& ray, const Vector3& cameraPos) const{
            if(_Triangles.empty()){return {};}
            return _Tree->getIntersections(_Triangles, ray, cameraPos);
        }

};

}