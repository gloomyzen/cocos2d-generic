#ifdef DEBUG
#include "imGuiLayer.h"
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "common/coreModule/nodes/nodeProperties.h"

using namespace common;
using namespace common::debugModule;

void imGuiLayer::_onStart() {
	std::string layerName = "ImGUILayer";
	auto order = INT_MAX;
	auto layer = ImGuiLayer::create();
	this->addChild(layer, order, layerName);

	//Buttons
	CCIMGUI::getInstance()->addCallback([this](){
		static bool nodeEditorOpened = false;
//		static bool engineInfoOpened = false;
//		static bool devicesListOpened = false;

		if (ImGui::Button("Debug")) {
			nodeEditorOpened = !nodeEditorOpened;
		}
//		if (ImGui::Button("Engine")) {
//			engineInfoOpened = !engineInfoOpened;
//		}
//		if (ImGui::Button("Devices")) {
//			devicesListOpened = !devicesListOpened;
//		}

		if (nodeEditorOpened) showNodeEditor(&nodeEditorOpened);
//		if (engineInfoOpened) showEngineInfo(&engineInfoOpened);
//		if (devicesListOpened) showDevicesList(&devicesListOpened);

		/*if (nodeEditorOpened) {
			CCIMGUI::getInstance()->addCallback([=](){
				ImGui::Text("Hello, world!");
				// create button with Sprite, auto pushID / popID with texture id
//				CCIMGUI::getInstance()->imageButton(sp, ImVec2(0, 0));
			}, "hello");
		} else {
			CCIMGUI::getInstance()->removeCallback("hello");
		}*/
	}, "buttons");

//	_addArmature();

}

void imGuiLayer::showNodeEditor(bool *nodeEditorOpened) {
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(nodeEditorW), static_cast<float>(nodeEditorH)),
							 ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Property editor", reinterpret_cast<bool *>(nodeEditorOpened))) {
		ImGui::End();
		return;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	ImGui::Columns(2);
	ImGui::Separator();

	renderTree(cocos2d::Director::getInstance()->getRunningScene()->getChildren());
	ImGui::NextColumn();
	renderPreferences(lastTarget);

	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::PopStyleVar();
	ImGui::End();
};

ImRect imGuiLayer::renderTree(cocos2d::Vector<Node *> n) {
	const ImRect nodeRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	if (lastTarget == nullptr) {
		lastTarget = n.front();
	}
	for (auto &node : n) {
		if (node->getName() == "debugNode") continue;
		ImGui::AlignTextToFramePadding();
		std::string className = {};
//		auto test = typeid(*node).name(); // need only for debug
		if (classList.find(typeid(*node).name()) != classList.end()) {
			className = classList[typeid(*node).name()] + " ";
		}
		const std::string name = className + node->getName() + (node->isVisible() ? "" : " #inactive");
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
									   ImGuiTreeNodeFlags_SpanAvailWidth;
		if (lastTarget == node) {
			nodeFlags |= ImGuiTreeNodeFlags_Selected;
		}
		if (node->getChildren().empty()) {
			nodeFlags |= ImGuiTreeNodeFlags_Leaf;
		} else if (node->getChildren().size() == 1 && node->getChildren().front()->getName() == "debugNode") {
			nodeFlags |= ImGuiTreeNodeFlags_Leaf;
		}
		bool nodeOpen = ImGui::TreeNodeEx((void *) (intptr_t) node->_ID, nodeFlags, "%s", name.c_str());
		if (ImGui::IsItemClicked()) {
			//id of clicked element
			lastTarget = node;
		}
		if (nodeOpen) {
			ImGui::PushID(node);

			renderTree(node->getChildren());
			ImGui::PopID();
			ImGui::TreePop();
		}
	}


	return nodeRect;
}

ImRect imGuiLayer::renderPreferences(Node *node) {
	const ImRect prefRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
	if (node == nullptr || node->getReferenceCount() == 0 || !node->isRunning()) {
		return prefRect;
	}

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("General info")) {
		ImGui::Text("Node Name(ID) %s", node->getName().c_str());
		ImGui::Text("Node GUI %d", node->_ID);
		ImGui::Text("%s", node->getDescription().c_str());
		if (node->isRunning()) {
			auto active = node->isVisible();
			auto tempActive = active;
			ImGui::Checkbox(" Is active", &active);
			if (active != tempActive && node->getName() != "ImGUILayer") {
				node->setVisible(active);
			}
		}
	}

	debugToggleRow(node);

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Transform component")) {
		/**
		 * Height and Width
		 */
		auto width = node->getContentSize().width;
		auto height = node->getContentSize().height;
		float vecWH[2] = {width, height};
		ImGui::DragFloat2("Width/Height", vecWH, 1);
		if (vecWH[0] != width || vecWH[1] != height) {
			auto _size = cocos2d::Size();
			_size.width = vecWH[0];
			_size.height = vecWH[1];
			node->setContentSize(_size);
		}
		/**
		 * Position by X Y Z
		 */
		auto nodePos = node->getPosition3D();
		float vecPos[3] = {nodePos.x, nodePos.y, nodePos.z};
		ImGui::DragFloat3("Position X/Y", vecPos, 1.f);
		if (vecPos[0] != nodePos.x || vecPos[1] != nodePos.y || vecPos[2] != nodePos.z) {
			cocos2d::Vec3 vec3 = {vecPos[0], vecPos[1], vecPos[2]};
			node->setPosition3D(vec3);
		}
		/**
		 * Anchor
		 */
		auto nodeAnchor = node->getAnchorPoint();
		float vecAnchor[2] = {nodeAnchor.x, nodeAnchor.y};
		ImGui::DragFloat2("Anchor X/Y", vecAnchor, 0.1f, -1.f, 1.f);
		if (vecAnchor[0] != nodeAnchor.x || vecAnchor[1] != nodeAnchor.y) {
			node->setAnchorPoint({vecAnchor[0], vecAnchor[1]});
		}
		/**
		 * Scale
		 */
		auto nodeScaleX = node->getScaleX();
		auto nodeScaleY = node->getScaleY();
		auto tempScaleX = nodeScaleX;
		auto tempScaleY = nodeScaleY;
		float vecScale[2] = {nodeScaleX, nodeScaleY};
		ImGui::DragFloat2("Scale X/Y", vecScale, 0.01f);
		if (tempScaleX != vecScale[0] || tempScaleY != vecScale[1]) {
			node->setScaleX(vecScale[0]);
			node->setScaleY(vecScale[1]);
		}
		/**
		 * Skew X and Y
		 */
		auto nodeSkewX = node->getSkewX();
		auto nodeSkewY = node->getSkewY();
		auto tempSkewX = nodeSkewX;
		auto tempSkewY = nodeSkewY;
		float vecSkew[2] = {nodeSkewX, nodeSkewY};
		ImGui::DragFloat2("Skew X/Y", vecSkew, 0.1f);
		if (tempSkewX != vecSkew[0] || tempSkewY != vecSkew[1]) {
			node->setSkewX(vecSkew[0]);
			node->setSkewY(vecSkew[1]);
		}
		/**
		 * Rotation
		 */
		auto nodeRotation = node->getRotation();
		auto tempRotation = nodeRotation;
		float floatRotation[1] = {nodeRotation};
		ImGui::DragFloat("Rotation", floatRotation, 0.1f);
		if(floatRotation[0] != tempRotation) {
			node->setRotation(floatRotation[0]);
		}
		/**
		 * Set cascade opacity
		 */
		auto isCascadeOpacity = node->isCascadeOpacityEnabled();
		auto tempCascadeOpacity = isCascadeOpacity;
		ImGui::Checkbox("Cascade opacity", &tempCascadeOpacity);
		if (tempCascadeOpacity != isCascadeOpacity) {
			node->setCascadeOpacityEnabled(tempCascadeOpacity);
		}
		/**
		 * Set opacity
		 */
		auto nodeOpacity = node->getOpacity();
		auto tempOpacity = nodeOpacity;
		int changedOpacity = static_cast<int>(nodeOpacity);
		ImGui::DragInt("Opacity", &changedOpacity, 1, 0, 255);
		if(changedOpacity != tempOpacity) {
			node->setOpacity(static_cast<uint8_t>(changedOpacity));
		}
	}
	if (ImGui::CollapsingHeader("Color component")) {
		/**
		 * Set cascade opacity
		 */
		auto isCascadeColor = node->isCascadeColorEnabled();
		auto tempCascadeColor = isCascadeColor;
		ImGui::Checkbox("Cascade color", &tempCascadeColor);
		if (tempCascadeColor != isCascadeColor) {
			node->setCascadeColorEnabled(tempCascadeColor);
		}
		/***
		 * Sprite Color
		 */
		auto nodeColor = node->getColor();
		float nodeRed = nodeColor.r / 255.0f;
		float nodeGreen = nodeColor.g / 255.0f;
		float nodeBlue = nodeColor.b / 255.0f;
		float vecColors[3] = {nodeRed, nodeGreen, nodeBlue};
		ImGui::ColorEdit4("Color", (float*)&vecColors, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Uint8);
		if (vecColors[0] != static_cast<float>(nodeColor.r) / 255.0f) {
			nodeColor.r = static_cast<uint8_t>(vecColors[0] * 255.0f);
			if (nodeColor.r < 0) nodeColor.r = 0;
			else if (nodeColor.r >= 255) nodeColor.r = 255;
			node->setColor(nodeColor);
		}
		if (vecColors[1] != static_cast<float>(nodeColor.g) / 255.0f) {
			nodeColor.g = static_cast<uint8_t>(vecColors[1] * 255.0f);
			if (nodeColor.g < 0) nodeColor.g = 0;
			else if (nodeColor.g >= 255) nodeColor.g = 255;
			node->setColor(nodeColor);
		}
		if (vecColors[2] != static_cast<float>(nodeColor.b) / 255.0f) {
			nodeColor.b = static_cast<uint8_t>(vecColors[2] * 255.0f);
			if (nodeColor.b < 0) nodeColor.b = 0;
			else if (nodeColor.b >= 255) nodeColor.b = 255;
			node->setColor(nodeColor);
		}
	}
	if (auto spriteNode = dynamic_cast<cocos2d::Sprite*>(node)) {
		if (ImGui::CollapsingHeader("Sprite component")) {
			ImGui::Text("Image \"%s\"", spriteNode->getResourceName().c_str());
		}
	}
	if (auto labelNode = dynamic_cast<cocos2d::Label*>(node)) {
		if (ImGui::CollapsingHeader("Label component")) {
			//todo lh ls color text font shadow
			/*** Text */
			auto labelText = labelNode->getString();
			char *newLabelText = const_cast<char *>(labelText.c_str());
			ImGui::InputText("Text", newLabelText, IM_ARRAYSIZE(newLabelText)*16);
			if (labelText != newLabelText) {
				labelNode->setString(newLabelText);
			}
			/*** Label Effects todo not working! */
			/*int labelEffectsIdx = 0;
			auto labelEffect = labelNode->getLabelEffectType();
			for (int i = 0; i < labelEffectList.size(); ++i) {
				if (labelEffectList[i].second == labelEffect) {
					labelEffectsIdx = i;
				}
			}
			int currentEffectsIdx = labelEffectsIdx;
			std::string elementsList{};
			for (const auto &item : labelEffectList) {
				elementsList += item.first + '\0';
			}
			if (ImGui::Combo("Style", &labelEffectsIdx, elementsList.c_str())) {
				if (labelEffectsIdx != currentEffectsIdx) {
					//
				}
			}*/
		}
	}


	return prefRect;
}

void imGuiLayer::debugToggleRow(cocos2d::Node* node) {
	if (node->isRunning()) {
		auto active = node->getDebug();
		auto tempActive = active;
		ImGui::Checkbox("debug", &active);
		if (active != tempActive) {
			node->setDebug(active);
		}
	}
}

#endif //DEBUG
