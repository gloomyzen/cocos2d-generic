#include "physicsShapeCache.h"

using namespace generic::coreModule;

physicsShapeCache* currentShapesInstance = nullptr;

physicsShapeCache::physicsShapeCache() {}


physicsShapeCache::~physicsShapeCache() {
    removeAllShapes();
}


physicsShapeCache& physicsShapeCache::getInstance() {
    if (currentShapesInstance == nullptr) {
        currentShapesInstance = new physicsShapeCache();
    }
    return *currentShapesInstance;
}

void physicsShapeCache::cleanup() {
    if (currentShapesInstance) {
        currentShapesInstance->removeAllShapes();
        delete currentShapesInstance;
    }
    currentShapesInstance = nullptr;
}


bool physicsShapeCache::addShapesWithFile(const std::string& plist) {
    float scaleFactor = cocos2d::Director::getInstance()->getContentScaleFactor();
    return addShapesWithFile(plist, scaleFactor);
}


bool physicsShapeCache::addShapesWithFile(const std::string& plist, float scaleFactor) {
    CCASSERT(bodiesInFile.find(plist) == bodiesInFile.end(), "file already loaded");

    cocos2d::ValueMap dict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(plist);
    if (dict.empty()) {
        // plist file not found
        return false;
    }

    cocos2d::ValueMap& metadata = dict["metadata"].asValueMap();
    int format = metadata["format"].asInt();
    if (format != 1) {
        CCASSERT(format == 1, "format not supported!");
        return false;
    }

    cocos2d::ValueMap& bodydict = dict.at("bodies").asValueMap();

    std::vector<BodyDef*> bodies(bodydict.size());
    int num = 0;

    for (auto iter = bodydict.cbegin(); iter != bodydict.cend(); ++iter) {
        const cocos2d::ValueMap& bodyData = iter->second.asValueMap();
        std::string bodyName = iter->first;
        BodyDef* bodyDef = new BodyDef();
        bodies[num++] = bodyDef;
        bodyDefs.insert(std::make_pair(bodyName, bodyDef));
        bodyDef->anchorPoint = cocos2d::PointFromString(bodyData.at("anchorpoint").asString());
        bodyDef->isDynamic = bodyData.at("is_dynamic").asBool();
        bodyDef->affectedByGravity = bodyData.at("affected_by_gravity").asBool();
        bodyDef->allowsRotation = bodyData.at("allows_rotation").asBool();
        bodyDef->linearDamping = bodyData.at("linear_damping").asFloat();
        bodyDef->angularDamping = bodyData.at("angular_damping").asFloat();
        bodyDef->velocityLimit = bodyData.at("velocity_limit").asFloat();
        bodyDef->angularVelocityLimit = bodyData.at("angular_velocity_limit").asFloat();

        const cocos2d::ValueVector& fixtureList = bodyData.at("fixtures").asValueVector();
        for (auto& fixtureitem : fixtureList) {
            FixtureData* fd = new FixtureData();
            bodyDef->fixtures.push_back(fd);
            auto& fixturedata = fixtureitem.asValueMap();
            fd->density = fixturedata.at("density").asFloat();
            fd->restitution = fixturedata.at("restitution").asFloat();
            fd->friction = fixturedata.at("friction").asFloat();
            fd->tag = fixturedata.at("tag").asInt();
            fd->group = fixturedata.at("group").asInt();
            fd->categoryMask = fixturedata.at("category_mask").asInt();
            fd->collisionMask = fixturedata.at("collision_mask").asInt();
            fd->contactTestMask = fixturedata.at("contact_test_mask").asInt();

            std::string fixtureType = fixturedata.at("fixture_type").asString();
            if (fixtureType == "POLYGON") {
                fd->fixtureType = FIXTURE_POLYGON;
                const cocos2d::ValueVector& polygonsArray = fixturedata.at("polygons").asValueVector();
                for (auto& polygonitem : polygonsArray) {
                    Polygon* poly = new Polygon();
                    fd->polygons.push_back(poly);
                    auto& polygonArray = polygonitem.asValueVector();
                    poly->numVertices = (int)polygonArray.size();
                    auto* vertices = poly->vertices = new cocos2d::Point[poly->numVertices];
                    int vindex = 0;
                    for (auto& pointString : polygonArray) {
                        auto offset = cocos2d::PointFromString(pointString.asString());
                        vertices[vindex].x = offset.x / scaleFactor;
                        vertices[vindex].y = offset.y / scaleFactor;
                        vindex++;
                    }
                }
            } else if (fixtureType == "CIRCLE") {
                fd->fixtureType = FIXTURE_CIRCLE;
                const cocos2d::ValueMap& circleData = fixturedata.at("circle").asValueMap();
                fd->radius = circleData.at("radius").asFloat() / scaleFactor;
                fd->center = cocos2d::PointFromString(circleData.at("position").asString()) / scaleFactor;
            } else {
                // unknown type
                return false;
            }
        }
    }

    bodiesInFile[plist] = bodies;

    return true;
}


physicsShapeCache::BodyDef* physicsShapeCache::getBodyDef(const std::string& name) {
    try {
        return bodyDefs.at(name);
    } catch (std::out_of_range&) {
    }

    try {
        return bodyDefs.at(name.substr(0, name.rfind('.')));// remove file suffix and try again...
    } catch (std::out_of_range&) {
    }

    return nullptr;
}


void physicsShapeCache::setBodyProperties(cocos2d::PhysicsBody* body, BodyDef* bd) {
    body->setGravityEnable(bd->affectedByGravity);
    body->setDynamic(bd->isDynamic);
    body->setRotationEnable(bd->allowsRotation);
    body->setLinearDamping(bd->linearDamping);
    body->setAngularDamping(bd->angularDamping);
    body->setVelocityLimit(bd->velocityLimit);
    body->setAngularVelocityLimit(bd->angularVelocityLimit);
}


void physicsShapeCache::setShapeProperties(cocos2d::PhysicsShape* shape, FixtureData* fd) {
    shape->setGroup(fd->group);
    shape->setCategoryBitmask(fd->categoryMask);
    shape->setCollisionBitmask(fd->collisionMask);
    shape->setContactTestBitmask(fd->contactTestMask);
    shape->setTag(fd->tag);
}


cocos2d::PhysicsBody* physicsShapeCache::createBodyWithName(const std::string& name) {
    BodyDef* bd = getBodyDef(name);
    if (!bd) {
        CCLOG("WARNING: PhysicsBody with name \"%s\", not found!", name.c_str());
        return nullptr;// body not found
    }
    cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
    setBodyProperties(body, bd);

    for (auto fd : bd->fixtures) {
        cocos2d::PhysicsMaterial material(fd->density, fd->restitution, fd->friction);
        if (fd->fixtureType == FIXTURE_CIRCLE) {
            auto shape = cocos2d::PhysicsShapeCircle::create(fd->radius, material, fd->center);
            setShapeProperties(shape, fd);
            body->addShape(shape);
        } else if (fd->fixtureType == FIXTURE_POLYGON) {
            for (auto polygon : fd->polygons) {
                auto shape = cocos2d::PhysicsShapePolygon::create(polygon->vertices, polygon->numVertices, material, fd->center);
                setShapeProperties(shape, fd);
                body->addShape(shape);
            }
        }
    }
    return body;
}


bool physicsShapeCache::setBodyOnSprite(const std::string& name, cocos2d::Sprite* sprite) {
    cocos2d::PhysicsBody* body = createBodyWithName(name);
    if (body) {
        sprite->setPhysicsBody(body);
        sprite->setAnchorPoint(getBodyDef(name)->anchorPoint);
    }
    return body != nullptr;
}


void physicsShapeCache::removeShapesWithFile(const std::string& plist) {
    auto bodies = bodiesInFile.at(plist);

    for (auto iter = bodies.begin(); iter != bodies.end(); ++iter) {
        safeDeleteBodyDef(*iter);
    }

    bodiesInFile.erase(plist);
}


void physicsShapeCache::removeAllShapes() {
    for (auto iter = bodyDefs.cbegin(); iter != bodyDefs.cend(); ++iter) {
        safeDeleteBodyDef(iter->second);
    }
    bodyDefs.clear();
    bodiesInFile.clear();
}


void physicsShapeCache::safeDeleteBodyDef(BodyDef* bodyDef) {
    for (auto fixturedata : bodyDef->fixtures) {
        for (auto polygon : fixturedata->polygons) {
            CC_SAFE_DELETE_ARRAY(polygon->vertices);
            CC_SAFE_DELETE(polygon);
        }
        fixturedata->polygons.clear();
        CC_SAFE_DELETE(fixturedata);
    }
    bodyDef->fixtures.clear();
    CC_SAFE_DELETE(bodyDef);
}