#include "imguiController.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/misc/cpp/imgui_stdlib.h"


USING_NS_AX;
USING_NS_AX_EXT;
using namespace generic;
using namespace generic::debugModule;

imguiController* imguiController::pInstance = nullptr;
bool imguiController::destroyed = false;

imguiController::~imguiController() {}

imguiController& imguiController::getInstance() {
    if (!pInstance) {
        if (destroyed) {
            onDeadReference();
        } else {
            create();
        }
    }
    return *pInstance;
}

void imguiController::cleanup() {
    destroyed = true;
    pInstance = nullptr;
}

void imguiController::create() {
    static imguiController instance;
    pInstance = &instance;

//    if (auto scene = Director::getInstance()->getRunningScene()) {
//        ImGuiPresenter::getInstance()->addRenderLoop("#imguiController", [](){ GET_IMGUI_CONTROLLER().draw(); }, scene);
//    }

    auto e = Director::getInstance()->getEventDispatcher();
    e->addCustomEventListener(Director::EVENT_BEFORE_SET_NEXT_SCENE, [](EventCustom*){
        ImGuiPresenter::getInstance()->removeRenderLoop("#imguiController");
    });
    e->addCustomEventListener(Director::EVENT_AFTER_SET_NEXT_SCENE, [&](EventCustom*){
        if (auto scene = Director::getInstance()->getRunningScene()) {
            ImGuiPresenter::getInstance()->addRenderLoop("#imguiController", [](){ GET_IMGUI_CONTROLLER().draw(); }, scene);
        }
        GET_IMGUI_CONTROLLER().getEmitter().onSceneChanging.emit();
    });
}

void imguiController::onDeadReference() {
    AXASSERT(false, "Founded dead reference!");
}

sImguiModuleSignals& imguiController::getEmitter() {
    return emitter;
}

void imguiController::draw() {
    // clear things from last frame
    usedCCRefIdMap.clear();
    usedCCRef.clear();
    // drawing commands
    auto iter = _callPiplines.begin();
    for (; iter != _callPiplines.end(); ++iter) {
        iter->second();
    }
    // commands will be processed after update
}

void imguiController::addCallback(const std::string& name, const std::function<void()>& callBack) {
    if (_callPiplines.count(name)) {
        _callPiplines.erase(name);
    }
    _callPiplines[name] = callBack;
}

void imguiController::removeCallback(const std::string& name) {
    const auto iter = _callPiplines.find(name);
    if (iter != _callPiplines.end())
        _callPiplines.erase(iter);
}

static std::tuple<ImVec2, ImVec2> getTextureUV(Sprite* sp) {
    ImVec2 uv0, uv1;
    if (!sp || !sp->getTexture())
        return { uv0, uv1 };
    const auto rect = sp->getTextureRect();
    const auto tex = sp->getTexture();
    const float atlasWidth = (float)tex->getPixelsWide();
    const float atlasHeight = (float)tex->getPixelsHigh();
    uv0.x = rect.origin.x / atlasWidth;
    uv0.y = rect.origin.y / atlasHeight;
    uv1.x = (rect.origin.x + rect.size.width) / atlasWidth;
    uv1.y = (rect.origin.y + rect.size.height) / atlasHeight;
    return { uv0, uv1 };
}

void imguiController::image(Texture2D* tex, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col) {
    if (!tex)
        return;
    auto size_ = size;
    if (size_.x <= 0.f)
        size_.x = tex->getPixelsWide();
    if (size_.y <= 0.f)
        size_.y = tex->getPixelsHigh();
    ImGui::PushID(getCCRefId(tex));
    ImGui::Image((ImTextureID)tex, size_, uv0, uv1, tint_col, border_col);
    ImGui::PopID();
}

void imguiController::image(Sprite* sprite, const ImVec2& size, const ImVec4& tint_col, const ImVec4& border_col) {
    if (!sprite || !sprite->getTexture())
        return;
    auto size_ = size;
    const auto& rect = sprite->getTextureRect();
    if (size_.x <= 0.f)
        size_.x = rect.size.width;
    if (size_.y <= 0.f)
        size_.y = rect.size.height;
    ImVec2 uv0, uv1;
    std::tie(uv0, uv1) = getTextureUV(sprite);
    ImGui::PushID(getCCRefId(sprite));
    ImGui::Image((ImTextureID)sprite->getTexture(), size_, uv0, uv1, tint_col, border_col);
    ImGui::PopID();
}

bool imguiController::imageButton(Texture2D* tex, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col) {
    if (!tex)
        return false;
    auto size_ = size;
    if (size_.x <= 0.f)
        size_.x = tex->getPixelsWide();
    if (size_.y <= 0.f)
        size_.y = tex->getPixelsHigh();
    ImGui::PushID(getCCRefId(tex));
    const auto ret = ImGui::ImageButton((ImTextureID)tex, size_, uv0, uv1, frame_padding, bg_col, tint_col);
    ImGui::PopID();
    return ret;
}

bool imguiController::imageButton(Sprite* sprite, const ImVec2& size, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col) {
    if (!sprite || !sprite->getTexture())
        return false;
    auto size_ = size;
    const auto rect = sprite->getTextureRect();
    if (size_.x <= 0.f)
        size_.x = rect.size.width;
    if (size_.y <= 0.f)
        size_.y = rect.size.height;
    ImVec2 uv0, uv1;
    std::tie(uv0, uv1) = getTextureUV(sprite);
    ImGui::PushID(getCCRefId(sprite));
    const auto ret = ImGui::ImageButton((ImTextureID)sprite->getTexture(), size_, uv0, uv1, frame_padding, bg_col, tint_col);
    ImGui::PopID();
    return ret;
}

void imguiController::node(Node* node, const ImVec4& tint_col, const ImVec4& border_col) {
    if (!node)
        return;
    const auto size = node->getContentSize();
    const auto pos = ImGui::GetCursorScreenPos();
    Mat4 tr;
    tr.m[5] = -1;
    tr.m[12] = pos.x;
    tr.m[13] = pos.y + size.height;
    if (border_col.w > 0.f) {
        tr.m[12] += 1;
        tr.m[13] += 1;
    }
    node->setNodeToParentTransform(tr);
    ImGui::PushID(getCCRefId(node));
    ImGui::Image((ImTextureID)node, ImVec2(size.width, size.height), ImVec2(0, 0), ImVec2(1, 1), tint_col, border_col);
    ImGui::PopID();
}

bool imguiController::nodeButton(Node* node, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col) {
    if (!node)
        return false;
    const auto size = node->getContentSize();
    const auto pos = ImGui::GetCursorScreenPos();
    Mat4 tr;
    tr.m[5] = -1;
    tr.m[12] = pos.x;
    tr.m[13] = pos.y + size.height;
    if (frame_padding >= 0) {
        tr.m[12] += (float)frame_padding;
        tr.m[13] += (float)frame_padding;
    } else {
        tr.m[12] += ImGui::GetStyle().FramePadding.x;
        tr.m[13] += ImGui::GetStyle().FramePadding.y;
    }
    node->setNodeToParentTransform(tr);
    ImGui::PushID(getCCRefId(node));
    const auto ret =
      ImGui::ImageButton((ImTextureID)node, ImVec2(size.width, size.height), ImVec2(0, 0), ImVec2(1, 1), frame_padding, bg_col, tint_col);
    ImGui::PopID();
    return ret;
}

std::tuple<ImTextureID, int> imguiController::useTexture(Texture2D* texture) {
    if (!texture)
        return { nullptr, 0 };
    return { (ImTextureID)texture, getCCRefId(texture) };
}

std::tuple<ImTextureID, ImVec2, ImVec2, int> imguiController::useSprite(Sprite* sprite) {
    if (!sprite || !sprite->getTexture())
        return { nullptr, {}, {}, 0 };
    ImVec2 uv0, uv1;
    std::tie(uv0, uv1) = getTextureUV(sprite);
    return { (ImTextureID)sprite->getTexture(), uv0, uv1, getCCRefId(sprite) };
}

std::tuple<ImTextureID, ImVec2, ImVec2, int> imguiController::useNode(Node* node, const ImVec2& pos) {
    if (!node)
        return { nullptr, {}, {}, 0 };
    const auto size = node->getContentSize();
    Mat4 tr;
    tr.m[5] = -1;
    tr.m[12] = pos.x;
    tr.m[13] = pos.y + size.height;
    node->setNodeToParentTransform(tr);
    return { (ImTextureID)node, pos, ImVec2(pos.x + size.width, pos.y + size.height), getCCRefId(node) };
}

void imguiController::setNodeColor(Node* node, const ImVec4& col) {
    if (node) {
        node->setColor({ uint8_t(col.x * 255), uint8_t(col.y * 255), uint8_t(col.z * 255) });
        node->setOpacity(uint8_t(col.w * 255));
    }
}

void imguiController::setNodeColor(Node* node, ImGuiCol col) {
    if (node && 0 <= col && col < ImGuiCol_COUNT)
        setNodeColor(node, ImGui::GetStyleColorVec4(col));
}

void imguiController::setLabelColor(Label* label, const ImVec4& col) {
    if (label) {
        label->setTextColor({ uint8_t(col.x * 255), uint8_t(col.y * 255), uint8_t(col.z * 255), uint8_t(col.w * 255) });
    }
}

void imguiController::setLabelColor(Label* label, bool disabled) {
    if (label)
        setLabelColor(label, ImGui::GetStyleColorVec4(disabled ? ImGuiCol_TextDisabled : ImGuiCol_Text));
}

void imguiController::setLabelColor(Label* label, ImGuiCol col) {
    if (label && 0 <= col && col < ImGuiCol_COUNT)
        setLabelColor(label, ImGui::GetStyleColorVec4(col));
}

ImWchar* imguiController::addGlyphRanges(const std::string& key, const std::vector<ImWchar>& ranges) {
    auto it = glyphRanges.find(key);
    // the pointer must be persistant, do not replace
    if (it != glyphRanges.end())
        return it->second.data();
    glyphRanges[key] = ranges;
    if (ranges.empty())
        glyphRanges[key].push_back(0);
    return glyphRanges[key].data();
}

void imguiController::mergeFontGlyphs(ImFont* dst, ImFont* src, ImWchar start, ImWchar end) {
    if (!dst || !src || start > end)
        return;
    for (auto i = start; i <= end; ++i) {
        const auto g = src->FindGlyphNoFallback(i);
        if (g) {
            dst->AddGlyph(src->ConfigData, (ImWchar)g->Codepoint, g->X0, g->Y0, g->X1, g->Y1, g->U0, g->V0, g->U1, g->V1, g->AdvanceX);
        }
    }
    dst->BuildLookupTable();
}

int imguiController::getCCRefId(Ref* p) {
    int id = 0;
    const auto it = usedCCRefIdMap.find(p);
    if (it == usedCCRefIdMap.end()) {
        usedCCRefIdMap[p] = 0;
        usedCCRef.pushBack(p);
    } else
        id = ++it->second;
    // BKDR hash
    constexpr unsigned int seed = 131;
    unsigned int hash = 0;
    for (auto i = 0u; i < sizeof(void*); ++i)
        hash = hash * seed + ((const char*)&p)[i];
    for (auto i = 0u; i < sizeof(int); ++i)
        hash = hash * seed + ((const char*)&id)[i];
    return (int)hash;
}

bool imguiController::Combo(const char* label, int* current_item, const std::vector<std::string>& items, int items_count, int height_in_items) {
    return ImGui::Combo(label, current_item,[](void* data, int idx, const char** out_text) {
          auto strVec = static_cast<const std::vector<std::string>*>(data);
          *out_text = (*strVec)[idx].c_str();
          return true;
      }, (void*)&items, items_count, height_in_items);
}

void imguiController::dragFloat(const std::string& label, const std::vector<float>& vals, int vSpeed, const std::function<void(const std::vector<float>&)>& changeClb) {
    auto valsCnt = static_cast<int>(vals.size());
    if (vals.empty() || valsCnt > 4) {
        ImGui::Text("Incorrect vector size.");
        return;
    }
    auto tempVals = vals;
    auto changed = false;
    switch (valsCnt) {
    case 1: {
        auto tempVal = vals[0];
        ImGui::DragFloat(label.c_str(), &tempVal, vSpeed);
        if (tempVal != vals[0]) {
            tempVals[0] = tempVal;
            changed = true;
        }
    } break;
    case 2: {
        float tempVal[2] = {vals[0], vals[1]};
        ImGui::DragFloat2(label.c_str(), tempVal, vSpeed);
        if (tempVal[0] != vals[0] || tempVal[1] != vals[1]) {
            tempVals[0] = tempVal[0];
            tempVals[1] = tempVal[1];
            changed = true;
        }
    } break;
    case 3: {
        float tempVal[3] = {vals[0], vals[1], vals[2]};
        ImGui::DragFloat3(label.c_str(), tempVal, vSpeed);
        if (tempVal[0] != vals[0] || tempVal[1] != vals[1] || tempVal[2] != vals[2]) {
            tempVals[0] = tempVal[0];
            tempVals[1] = tempVal[1];
            tempVals[2] = tempVal[2];
            changed = true;
        }
    } break;
    case 4: {;
        float tempVal[4] = {vals[0], vals[1], vals[2], vals[3]};
        ImGui::DragFloat4(label.c_str(), tempVal, vSpeed);
        if (tempVal[0] != vals[0] || tempVal[1] != vals[1] || tempVal[2] != vals[2] || tempVal[3] != vals[3]) {
            tempVals[0] = tempVal[0];
            tempVals[1] = tempVal[1];
            tempVals[2] = tempVal[2];
            tempVals[3] = tempVal[3];
            changed = true;
        }
    } break;
    }
    if (changed) {
        changeClb(tempVals);
    }
}

void imguiController::dragFloat(const std::string& label, const ax::Size& vals, int vSpeed, const std::function<void(const ax::Size&)>& changeClb) {
    std::vector<float> tempVal;
    tempVal.push_back(vals.x);
    tempVal.push_back(vals.y);
    auto tempClb = [&changeClb](const std::vector<float>& vals){
        changeClb({ vals[0], vals[1] });
    };
    dragFloat(label, tempVal, vSpeed, tempClb);
}