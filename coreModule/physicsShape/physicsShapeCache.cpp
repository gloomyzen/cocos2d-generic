#include "physicsShapeCache.h"

using namespace generic::coreModule;

const std::vector<std::string> obligatoryDataKeys = {
    "anchorpoint", "is_dynamic", "affected_by_gravity", "allows_rotation",
    "linear_damping", "angular_damping", "velocity_limit", "angular_velocity_limit", "fixtures"
};
const std::vector<std::string> obligatoryFixtureKeys = {
    "density", "restitution", "friction", "tag", "group", "category_mask", "collision_mask", "contact_test_mask", "fixture_type", "polygons"
};

physicsShapeCache* currentShapesInstance = nullptr;

physicsShapeCache::physicsShapeCache() = default;


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
    if (bodiesInFile.count(plist)) {
        return false;
    }

    cocos2d::ValueMap dict = cocos2d::FileUtils::getInstance()->getValueMapFromFile(plist);
    if (dict.empty() || !dict.count("metadata") || dict.count("bodies")) {
        // plist file not found
        return false;
    }

    cocos2d::ValueMap& metadata = dict["metadata"].asValueMap();
    int format = metadata["format"].asInt();
    if (format != 1) {
        CCASSERT(format == 1, "format not supported!");
        return false;
    }

    cocos2d::ValueMap& bodyDict = dict["bodies"].asValueMap();

    std::vector<BodyDef*> bodies(bodyDict.size());
    int num = 0;

    for (const auto& [bodyName, data] : bodyDict) {
        cocos2d::ValueMap bodyData = data.asValueMap();
        auto findDataKey = std::find_if(obligatoryDataKeys.begin(), obligatoryDataKeys.end(), [&bodyData](const std::string& s){
            return !bodyData.count(s);
        });
        if (findDataKey == obligatoryDataKeys.end()) {
            auto bodyDef = new BodyDef();
            bodies[num++] = bodyDef;
            bodyDefs.insert(std::make_pair(bodyName, bodyDef));
            bodyDef->anchorPoint = cocos2d::PointFromString(bodyData["anchorpoint"].asString());
            bodyDef->isDynamic = bodyData["is_dynamic"].asBool();
            bodyDef->affectedByGravity = bodyData["affected_by_gravity"].asBool();
            bodyDef->allowsRotation = bodyData["allows_rotation"].asBool();
            bodyDef->linearDamping = bodyData["linear_damping"].asFloat();
            bodyDef->angularDamping = bodyData["angular_damping"].asFloat();
            bodyDef->velocityLimit = bodyData["velocity_limit"].asFloat();
            bodyDef->angularVelocityLimit = bodyData["angular_velocity_limit"].asFloat();

            cocos2d::ValueVector& fixtureList = bodyData["fixtures"].asValueVector();
            for (auto& fixtureItem : fixtureList) {
                auto& fixtureData = fixtureItem.asValueMap();
                auto findFixtureKey = std::find_if(obligatoryFixtureKeys.begin(), obligatoryFixtureKeys.end(), [&fixtureData](const std::string& s){
                    return !fixtureData.count(s);
                });
                if (findFixtureKey == obligatoryFixtureKeys.end()) {
                    auto fd = new FixtureData();
                    bodyDef->fixtures.push_back(fd);
                    fd->density = fixtureData["density"].asFloat();
                    fd->restitution = fixtureData["restitution"].asFloat();
                    fd->friction = fixtureData["friction"].asFloat();
                    fd->tag = fixtureData["tag"].asInt();
                    fd->group = fixtureData["group"].asInt();
                    fd->categoryMask = fixtureData["category_mask"].asInt();
                    fd->collisionMask = fixtureData["collision_mask"].asInt();
                    fd->contactTestMask = fixtureData["contact_test_mask"].asInt();

                    std::string fixtureType = fixtureData["fixture_type"].asString();
                    if (fixtureType == "POLYGON") {
                        fd->fixtureType = FIXTURE_POLYGON;
                        const cocos2d::ValueVector& polygonsArray = fixtureData["polygons"].asValueVector();
                        for (auto& polygonItem : polygonsArray) {
                            auto poly = new Polygon();
                            fd->polygons.push_back(poly);
                            auto& polygonArray = polygonItem.asValueVector();
                            poly->numVertices = (int)polygonArray.size();
                            auto* vertices = poly->vertices = new cocos2d::Point[poly->numVertices];
                            int vIndex = 0;
                            for (auto& pointString : polygonArray) {
                                auto offset = cocos2d::PointFromString(pointString.asString());
                                vertices[vIndex].x = offset.x / scaleFactor;
                                vertices[vIndex].y = offset.y / scaleFactor;
                                vIndex++;
                            }
                        }
                    } else if (fixtureType == "CIRCLE") {
                        fd->fixtureType = FIXTURE_CIRCLE;
                        cocos2d::ValueMap& circleData = fixtureData["circle"].asValueMap();
                        fd->radius = circleData["radius"].asFloat() / scaleFactor;
                        fd->center = cocos2d::PointFromString(circleData["position"].asString()) / scaleFactor;
                    } else {
                        // unknown type
                        return false;
                    }
                }
            }
        }
    }

    bodiesInFile[plist] = bodies;

    return true;
}


physicsShapeCache::BodyDef* physicsShapeCache::getBodyDef(const std::string& name) {
    if (bodyDefs.count(name)) {
        return bodyDefs[name];
    } else if (auto key = name.substr(0, name.rfind('.')); bodyDefs.count(key)) {
        // remove file suffix and try again...
        return bodyDefs[key];
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
    if (bodiesInFile.count(plist)) {
        for (auto& item : bodiesInFile[plist]) {
            safeDeleteBodyDef(item);
        }

        bodiesInFile.erase(plist);
    }
}


void physicsShapeCache::removeAllShapes() {
    for (const auto& bodyDef : bodyDefs) {
        safeDeleteBodyDef(bodyDef.second);
    }
    bodyDefs.clear();
    bodiesInFile.clear();
}


void physicsShapeCache::safeDeleteBodyDef(BodyDef* bodyDef) {
    for (auto fixtureData : bodyDef->fixtures) {
        for (auto polygon : fixtureData->polygons) {
            CC_SAFE_DELETE_ARRAY(polygon->vertices);
            CC_SAFE_DELETE(polygon);
        }
        fixtureData->polygons.clear();
        CC_SAFE_DELETE(fixtureData);
    }
    bodyDef->fixtures.clear();
    CC_SAFE_DELETE(bodyDef);
}