#include "physicsShapeCache.h"
#include "generic/utilityModule/stringUtility.h"
#include "generic/coreModule/scenes/scenesFactoryInstance.h"
#include "generic/coreModule/scenes/sceneInterface.h"

using namespace generic::coreModule;

const std::vector<std::string> obligatoryDataKeys = {
    "anchorpoint", "is_dynamic", "affected_by_gravity", "allows_rotation",
    "linear_damping", "angular_damping", "velocity_limit", "angular_velocity_limit", "fixtures"
};
const std::vector<std::string> obligatoryFixtureKeys = {
    "density", "restitution", "friction", "tag", "group", "category_mask", "collision_mask", "contact_test_mask", "fixture_type", "polygons"
};

const std::string shapesFolder = "shapes/%s.plist";
const std::string findKeyPattern = "%s.%s";

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
    float scaleFactor = ax::Director::getInstance()->getContentScaleFactor();
    return addShapesWithFile(plist, scaleFactor);
}


bool physicsShapeCache::addShapesWithFile(const std::string& plist, float scaleFactor) {
    if (bodiesInFile.count(plist)) {
        return false;
    }
    auto path = STRING_FORMAT(shapesFolder.c_str(), plist.c_str());
    ax::ValueMap dict = ax::FileUtils::getInstance()->getValueMapFromFile(path);
    if (dict.empty() || !dict.count("metadata") || !dict.count("bodies")) {
        // plist file not found
        return false;
    }

    ax::ValueMap& metadata = dict["metadata"].asValueMap();
    int format = metadata["format"].asInt();
    if (format != 1) {
        AXASSERT(format == 1, "format not supported!");
        return false;
    }

    ax::ValueMap& bodyDict = dict["bodies"].asValueMap();

    std::vector<BodyDef*> bodies(bodyDict.size());
    int num = 0;

    for (const auto& [bodyName, data] : bodyDict) {
        ax::ValueMap bodyData = data.asValueMap();
        auto findDataKey = std::find_if(obligatoryDataKeys.begin(), obligatoryDataKeys.end(), [&bodyData](const std::string& s){
            return !bodyData.count(s);
        });
        if (findDataKey == obligatoryDataKeys.end()) {
            auto bodyDef = new BodyDef();
            bodies[num++] = bodyDef;
            bodyDefs.insert(std::make_pair(bodyName, bodyDef));
            bodyDef->anchorPoint = ax::PointFromString(bodyData["anchorpoint"].asString());
            bodyDef->isDynamic = bodyData["is_dynamic"].asBool();
            bodyDef->affectedByGravity = bodyData["affected_by_gravity"].asBool();
            bodyDef->allowsRotation = bodyData["allows_rotation"].asBool();
            bodyDef->linearDamping = bodyData["linear_damping"].asFloat();
            bodyDef->angularDamping = bodyData["angular_damping"].asFloat();
            bodyDef->velocityLimit = bodyData["velocity_limit"].asFloat();
            bodyDef->angularVelocityLimit = bodyData["angular_velocity_limit"].asFloat();

            ax::ValueVector& fixtureList = bodyData["fixtures"].asValueVector();
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
                        const ax::ValueVector& polygonsArray = fixtureData["polygons"].asValueVector();
                        for (auto& polygonItem : polygonsArray) {
                            auto poly = new Polygon();
                            fd->polygons.push_back(poly);
                            auto& polygonArray = polygonItem.asValueVector();
                            poly->numVertices = (int)polygonArray.size();
                            auto* vertices = poly->vertices = new ax::Point[poly->numVertices];
                            int vIndex = 0;
                            for (auto& pointString : polygonArray) {
                                auto offset = ax::PointFromString(pointString.asString());
                                vertices[vIndex].x = offset.x / scaleFactor;
                                vertices[vIndex].y = offset.y / scaleFactor;
                                vIndex++;
                            }
                        }
                    } else if (fixtureType == "CIRCLE") {
                        fd->fixtureType = FIXTURE_CIRCLE;
                        ax::ValueMap& circleData = fixtureData["circle"].asValueMap();
                        fd->radius = circleData["radius"].asFloat() / scaleFactor;
                        fd->center = ax::PointFromString(circleData["position"].asString()) / scaleFactor;
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
    auto key = name;
    if (GET_CURRENT_SCENE() && !GET_CURRENT_SCENE()->getName().empty()) {
        key = STRING_FORMAT(findKeyPattern.c_str(), GET_CURRENT_SCENE()->getName().data(), name.c_str());
    }
    if (bodyDefs.count(key)) {
        return bodyDefs[key];
    } else if (auto normalizedKey = key.substr(0, key.rfind('.')); bodyDefs.count(key)) {
        // remove file suffix and try again...
        return bodyDefs[normalizedKey];
    }

    return nullptr;
}


void physicsShapeCache::setBodyProperties(ax::PhysicsBody* body, BodyDef* bd) {
    body->setGravityEnable(bd->affectedByGravity);
    body->setDynamic(bd->isDynamic);
    body->setRotationEnable(bd->allowsRotation);
    body->setLinearDamping(bd->linearDamping);
    body->setAngularDamping(bd->angularDamping);
    body->setVelocityLimit(bd->velocityLimit);
    body->setAngularVelocityLimit(bd->angularVelocityLimit);
}


void physicsShapeCache::setShapeProperties(ax::PhysicsShape* shape, FixtureData* fd) {
    shape->setGroup(fd->group);
    shape->setCategoryBitmask(fd->categoryMask);
    shape->setCollisionBitmask(fd->collisionMask);
    shape->setContactTestBitmask(fd->contactTestMask);
    shape->setTag(fd->tag);
}


ax::PhysicsBody* physicsShapeCache::createBodyWithName(const std::string& name) {
    BodyDef* bd = getBodyDef(name);
    if (!bd) {
        AXLOG("WARNING: PhysicsBody with name \"%s\", not found!", name.c_str());
        return nullptr;// body not found
    }
    ax::PhysicsBody* body = ax::PhysicsBody::create();
    setBodyProperties(body, bd);

    for (auto fd : bd->fixtures) {
        ax::PhysicsMaterial material(fd->density, fd->restitution, fd->friction);
        if (fd->fixtureType == FIXTURE_CIRCLE) {
            auto shape = ax::PhysicsShapeCircle::create(fd->radius, material, fd->center);
            setShapeProperties(shape, fd);
            body->addShape(shape);
        } else if (fd->fixtureType == FIXTURE_POLYGON) {
            for (auto polygon : fd->polygons) {
                auto shape = ax::PhysicsShapePolygon::create(polygon->vertices, polygon->numVertices, material, fd->center);
                setShapeProperties(shape, fd);
                body->addShape(shape);
            }
        }
    }
    return body;
}


bool physicsShapeCache::setBodyOnSprite(const std::string& name, ax::Sprite* sprite) {
    ax::PhysicsBody* body = createBodyWithName(name);
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
            AX_SAFE_DELETE_ARRAY(polygon->vertices);
            AX_SAFE_DELETE(polygon);
        }
        fixtureData->polygons.clear();
        AX_SAFE_DELETE(fixtureData);
    }
    bodyDef->fixtures.clear();
    AX_SAFE_DELETE(bodyDef);
}