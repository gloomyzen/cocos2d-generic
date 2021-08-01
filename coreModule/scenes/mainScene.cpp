#include "mainScene.h"
#include "generic/coreModule/gameManager.h"
#include "generic/coreModule/scenes/scenesFactory/scenesFactoryInstance.h"
#include "generic/debugModule/logManager.h"
#include "ui/CocosGUI.h"

using namespace generic::coreModule;
using namespace cocos2d;

mainScene::mainScene() {}

mainScene::~mainScene() {}

Scene* mainScene::createScene() {
    return mainScene::create();
}

bool mainScene::init() {
    if (!Scene::init()) {
        return false;
    }
    LOG_INFO("mainScene::init()");

    auto bg = new LayerColor();
    bg->setColor(Color3B::BLACK);
    bg->setContentSize(Director::getInstance()->getVisibleSize());
    this->addChild(bg);

    windowViewer = new windowSystem();
    this->addChild(windowViewer, eGameLayers::WINDOW);

#ifdef DEBUG
    /// insert debug layer
    imGuiLayer = generic::debugModule::imGuiLayer::create();
    this->addChild(imGuiLayer, eGameLayers::DEBUG_LAYER);
#endif

    return true;
}

void mainScene::setRoom(const std::string& state) {
    auto director = Director::getInstance();
    auto visibleSize = director->getVisibleSize();
    if (nodes.empty()) {
        auto currentNode = scenesFactoryInstance::getInstance().getStateRoot(state);
        this->addChild(currentNode, eGameLayers::ROOT);
        initTaskLoading(currentNode);
        nodes.push_back(currentNode);
        return;
    }
    nodes.reserve(2);

    nodes.front()->setLocalZOrder(nodes.front()->getLocalZOrder() - 1);
    auto nextNode = scenesFactoryInstance::getInstance().getStateRoot(state);
    nextNode->setCascadeOpacityEnabled(true);
    nextNode->setOpacity(0);
    auto seq = Sequence::create(FadeIn::create(0.5f),
                                CallFunc::create([this]() {
                                    nodes.front()->removeFromParentAndCleanup(true);
                                    nodes.erase(nodes.begin());
                                }),
                                nullptr);
//    nextNode->setContentSize(visibleSize);
    this->addChild(nextNode, eGameLayers::ROOT);
    initTaskLoading(nextNode);
    nodes.push_back(nextNode);
    nextNode->runAction(seq);
//    director->getRunningScene()->getDefaultCamera()->setPosition(visibleSize.width / 2, visibleSize.height / 2);
}

void mainScene::initTaskLoading(cocos2d::Node* node) {
    if (auto item = dynamic_cast<taskHolder*>(node)) {
        auto atp = cocos2d::AsyncTaskPool::getInstance();
        atp->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_OTHER, [](void*) {}, nullptr, [item]() {
              item->executeTasks();
        });
    }
}
