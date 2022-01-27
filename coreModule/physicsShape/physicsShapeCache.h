#ifndef GENERIC_PHYSICSSHAPECACHE_H
#define GENERIC_PHYSICSSHAPECACHE_H

#include "cocos2d.h"

#define GET_PHYSICS_SHAPE() generic::coreModule::physicsShapeCache::getInstance()

namespace generic::coreModule {
    class physicsShapeCache {
    public:
        /**
         * Get pointer to the physicsShapeCache singleton instance
         *
         * @return physicsShapeCache*
         */
        static physicsShapeCache& getInstance();

        /**
         * Removes all shapes and cleanup
         */
        static void cleanup();

        /**
         * Adds all physics shapes from a plist file.
         * Shapes are scaled by contentScaleFactor
         *
         * @param plist name of the shape definitions file to load
         *
         * @retval true if ok
         * @retval false on error
         */
        bool addShapesWithFile(const std::string& plist);

        /**
         * Adds all physics shapes from a plist file.
         *
         * @param plist name of the shape definitions file to load
         * @param scaleFactor scale factor to apply for all shapes
         *
         * @retval true if ok
         * @retval false on error
         */
        bool addShapesWithFile(const std::string& plist, float scaleFactor);

        /**
         * Removes all shapes loaded from the given file
         *
         * @param plist name of the body definitions file
         */
        void removeShapesWithFile(const std::string& plist);

        /**
         * Removes all shapes
         */
        void removeAllShapes();

        /**
         * Creates a PhysicsBody with the given name
         *
         * @param name name of the body to create
         *
         * @return new PhysicsBody
         * @retval nullptr if body is not found
         */
        cocos2d::PhysicsBody* createBodyWithName(const std::string& name);

        /**
         * Creates a new PhysicsBody and attaches it to the given sprite
         *
         * @param name name of the body to attach
         * @param sprite sprite to attach the body to
         *
         * @retval true if body was attached to the sprite
         * @retval false if body was not found
         */
        bool setBodyOnSprite(const std::string& name, cocos2d::Sprite* sprite);

    private:
        typedef enum { FIXTURE_POLYGON, FIXTURE_CIRCLE } FixtureType;


        class Polygon {
        public:
            cocos2d::Point* vertices;
            int numVertices;
        };


        class FixtureData {
        public:
            FixtureType fixtureType;

            float density;
            float restitution;
            float friction;

            int tag;
            int group;
            int categoryMask;
            int collisionMask;
            int contactTestMask;

            // for circles
            cocos2d::Point center;
            float radius;


            std::vector<Polygon*> polygons;
        };


        class BodyDef {
        public:
            cocos2d::Point anchorPoint;
            std::vector<FixtureData*> fixtures;

            bool isDynamic;
            bool affectedByGravity;
            bool allowsRotation;

            float linearDamping;
            float angularDamping;
            float velocityLimit;
            float angularVelocityLimit;
        };

        physicsShapeCache();
        ~physicsShapeCache();
        void safeDeleteBodyDef(BodyDef* bodyDef);
        BodyDef* getBodyDef(const std::string& name);
        void setBodyProperties(cocos2d::PhysicsBody* body, BodyDef* bd);
        void setShapeProperties(cocos2d::PhysicsShape* shape, FixtureData* fd);

        std::map<std::string, BodyDef*> bodyDefs;
        std::map<std::string, std::vector<BodyDef*>> bodiesInFile;
    };
}// namespace generic::coreModule


#endif// GENERIC_PHYSICSSHAPECACHE_H
