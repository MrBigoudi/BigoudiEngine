#pragma once

#include "be_model.hpp"
#include "be_ray.hpp"
#include "be_vector3.hpp"


namespace be{

/**
 * Forward declaration of the Bounding class
 * @see Bounding
*/
class Bounding;

/**
 * Smart pointer to the Bounding class
 * @see Bounding
*/
using BoundingPtr = std::shared_ptr<Bounding>;

/**
 * Forward declaration of the AABB class
 * @see AxisAlignedBoundingBox
*/
class AxisAlignedBoundingBox;

/**
 * Smart pointer to the AABB class
 * @see AxisAlignedBoundingBox
*/
using AxisAlignedBoundingBoxPtr = std::shared_ptr<AxisAlignedBoundingBox>;

/**
 * Forward declaration of the Bounding Sphere class
 * @see BoundingSphere
*/
class BoundingSphere;

/**
 * Smart pointer to the Bounding Sphere class
 * @see BoundingSphere
*/
using BoundingSpherePtr = std::shared_ptr<BoundingSphere>;

/**
 * Forward declaration of the BSH class
 * @see BSH
*/
class BSH;

/**
 * Smart pointer to the BSH class
 * @see BSH
*/
using BSHPtr = std::shared_ptr<BSH>;

/**
 * Forward declaration of the BVH class
 * @see BVH
*/
class BVH;

/**
 * Smart pointer to the BVH class
 * @see BVH
*/
using BVHPtr = std::shared_ptr<BVH>;

/**
 * An abstract class to represent a bounding
*/
class Bounding{
    public:
        /**
         * A basic constructor
         * @param triangles The list of triangles that we want to bound
        */
        Bounding(const std::vector<Triangle>& triangles);

        /**
         * An empty constructor
        */
        Bounding(){};
};

/**
 * A class representing a bounding cone (usefull for oriented lights)
 * @extends Bounding
 * @see OrientedLight
*/
class BoundingCone : public Bounding{
    public:
        /**
         * The tip of the cone
        */
        Vector3 _Tip = {};

        /**
         * The axis of the cone
        */
        Vector3 _Axis = {};

        /**
         * The angle of the cone opening
        */
        float _AngularSpan = 0.f;

    public:
        /**
         * An empty constructor
        */
        BoundingCone(){};

        /**
         * A getter for the half of the cone
         * @return Half of the angular span
        */
        float getHalfAngle() const{return _AngularSpan / 2.f;}

        /**
         * Merge two bounding cones
         * @param bc1 The first bounding cone
         * @param bc2 The second bounding cone
         * @return The merged bounding cone
        // TODO:
        */
        static BoundingCone merge(const BoundingCone& bc1, const BoundingCone& bc2){
            return bc1._AngularSpan > bc2._AngularSpan ? bc1 : bc2;
        }
};

/**
 * An axis aligned bounding box
 * @extends Bounding
*/
class AxisAlignedBoundingBox : public Bounding{
    public:
        /**
         * The minimum in the global frame x axis
        */
        float _MinX = 0.f;
        
        /**
         * The maximum in the global frame x axis
        */
        float _MaxX = 0.f;
        
        /**
         * The minimum in the global frame y axis
        */
        float _MinY = 0.f;
        
        /**
         * The maximum in the global frame y axis
        */
        float _MaxY = 0.f;
        
        /**
         * The minimum in the global frame z axis
        */
        float _MinZ = 0.f;
        
        /**
         * The maximum in the global frame z axis
        */
        float _MaxZ = 0.f;

    public:
        /**
         * An enumeration to represent an axis
        */
        enum Axis{
            X,Y,Z
        };

    public:
        /**
         * An empty constructor
        */
        AxisAlignedBoundingBox(){};

        /**
         * A basic constructor
         * @param minX bounding value
         * @param maxX bounding value
         * @param minY bounding value
         * @param maxY bounding value
         * @param minZ bounding value
         * @param maxZ bounding value
        */
        AxisAlignedBoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
            :_MinX(minX), _MaxX(maxX), _MinY(minY), _MaxY(maxY), _MinZ(minZ), _MaxZ(maxZ){
        };

        /**
         * A basic constructor
         * @param triangles The list of triangles that we want to bound
        */
        AxisAlignedBoundingBox(const std::vector<Triangle>& triangles);

        /**
         * Merge two AABB
         * @param aabb1 The first AABB
         * @param aabb2 The second AABB
         * @return The merged bounding box
        */
        static AxisAlignedBoundingBox merge(const AxisAlignedBoundingBox& aabb1, const AxisAlignedBoundingBox& aabb2);

    public:
        /**
         * Get the center of the box
         * @return The center as a Vector3
        */
        Vector3 getCenter() const;

        /**
         * Get the dominant axis (i.e. the axis where the box is the biggest)
         * @return The axis
         * @see Axis
        */
        Axis getDominantAxis() const;

        /**
         * Get the distance on a given axis
         * @return The distance as a float
        */
        float getDistance(Axis axis) const;

        /**
         * Get the normal of the plane passing through the dominant axis
         * @return The normal as a Vector3
        */
        Vector3 getDominantAxisPlaneNormal() const;

        /**
         * Get the diagonal length of the bounding box
         * @return The length as a float
        */
        float getDiagonalLength() const;

        /**
         * Cast a bounding box into a string
         * @return An std::string
        */
        std::string toString() const;

        float getDistance(const Vector3& point) const;
        Vector3 getClosestPoint(const Vector3& point) const;
        AxisAlignedBoundingBox rotate(float angle) const;
};

/**
 * A class representing a bounding sphere
 * @extends Bounding
*/
class BoundingSphere : public Bounding{
    public:
        /**
         * The sphere center
        */
        Vector3 _Center = Vector3();

        /**
         * The sphere radius
        */
        float _Radius = 0.f;

    public:
        /**
         * A basic constructor
         * @param triangles The list of triangles that we want to bound
        */
        BoundingSphere(const std::vector<Triangle>& triangles);

        /**
         * A basic constructor
         * @param aabb The bounding sphere in which should lie the sphere
        */
        BoundingSphere(const AxisAlignedBoundingBox& aabb);
        
        /**
         * Cast a bounding sphere into a string
         * @return An std::string
        */
        std::string toString() const;
};


/**
 * A class representing a bounding sphere hierarchy
 * @see BoundingSphere
*/
class BSH{
    private:
        /**
         * Forward declaration of a BSH node
        */
        class BSHNode;

        /**
         * Smart pointer to a BSH node
        */
        using BSHNodePtr = std::shared_ptr<BSHNode>;
        
        /**
         * Forward declaration of a BSH tree
        */
        class BSHTree;

        /**
         * Smart pointer to a BSH tree
        */
        using BSHTreePtr = std::shared_ptr<BSHTree>;

    private:
        /**
         * A class representing a tree node
        */
        class BSHNode{
            public:
                /**
                 * The node bounding sphere
                */
                BoundingSpherePtr _Sphere = nullptr;

                /**
                 * The list of triangle indices
                */
                std::vector<uint32_t> _TriangleIndices = {};

                /**
                 * The left child of the node
                */
                BSHNodePtr _LeftChild = nullptr;

                /**
                 * The right child of the node
                */
                BSHNodePtr _RightChild = nullptr;

            public:
                /**
                 * A basic constructor
                 * @param triangles The triangles to store
                 * @param indices The indices of the global triangles
                 * @param depth The depth of the node
                */
                BSHNode(const std::vector<Triangle>& triangles, const std::vector<uint32_t>& indices, uint32_t depth = 0);

                /**
                 * Tells if a node is a leaf
                 * @return True if the node is a leaf
                */
                bool isLeaf() const {return _LeftChild == nullptr && _RightChild == nullptr;}

                /**
                 * Get the list of intersections from the given ray
                 * @param triangles The triangles to intersect
                 * @param ray To ray to try
                 * @param cameraPos The camera position
                 * @param hits The hits heap filled if an intersection is found
                */
                void getIntersections(const std::vector<Triangle>& triangles, const RayPtr& ray, const Vector3& cameraPos, RayHits& hits) const;
        };

        class BSHTree{
            public:
                BSHNodePtr _Root = nullptr;

            public:
                BSHTree(){};
                static BSHTreePtr init(const std::vector<Triangle>& triangles);

                void getIntersections(const std::vector<Triangle>& triangles, const RayPtr& ray, const Vector3& cameraPos, RayHits& hits) const{
                    _Root->getIntersections(triangles, ray, cameraPos, hits);
                }
        };



    private:
        BSHTreePtr _Tree = nullptr;
        const std::vector<Triangle> _Triangles;

    public:
        BSH(const std::vector<Triangle>& triangles);

        void getIntersections(const RayPtr& ray, const Vector3& cameraPos, RayHits& hits) const{
            if(_Triangles.empty()){return;}
            _Tree->getIntersections(_Triangles, ray, cameraPos, hits);
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

                void getIntersections(const std::vector<Triangle>& triangles, const RayPtr& ray, const Vector3& cameraPos, RayHits& hits) const{
                    _Root->getIntersections(triangles, ray, cameraPos, hits);
                }
        };



    private:
        BVHTreePtr _Tree = nullptr;
        const std::vector<Triangle> _Triangles;

    public:
        BVH(const std::vector<Triangle>& triangles);

        void getIntersections(const RayPtr& ray, const Vector3& cameraPos, RayHits& hits) const{
            if(_Triangles.empty()){return;}
            _Tree->getIntersections(_Triangles, ray, cameraPos, hits);
        }

};

}