#include "settingManager.h"
#include "json/document.h"
#include "json/ostreamwrapper.h"
#include "common/debugModule/logManager.h"

using namespace common::coreModule;

settingManager::settingManager(const sDisplaySize frameResolution, const sDisplaySize largeResolution,
							   const bool showDisplayStats) :
		frameResolutionSize(frameResolution), largeResolutionSize(largeResolution),
		showDisplayStats(showDisplayStats) {}

settingManager settingManager::load() {
	// default
	sDisplaySize frameResolution(1024.f, 768.f, 1.f);
	sDisplaySize largeResolution(480.f, 320.f, 1.f);
	auto stats = false;

	// load json
	const std::string &regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile("config/setting.json");
	rapidjson::Document doc;
	doc.Parse<0>(regionStr.c_str());

	if (doc.HasParseError()) {
		LOG_ERROR("settingManager::load: json parse error");
		return settingManager(frameResolution, largeResolution, stats);
	}

	if (doc.IsNull()) {
		LOG_ERROR("settingManager::load: json is empty");
		return settingManager(frameResolution, largeResolution, stats);
	}

	const auto frameResolutionItr = doc.FindMember("frameResolution");
	if (doc.MemberEnd() != frameResolutionItr) {
		const auto xItr = frameResolutionItr->value.FindMember("x");
		const auto yItr = frameResolutionItr->value.FindMember("y");
		const auto scaleItr = frameResolutionItr->value.FindMember("desktopScale");
		if (frameResolutionItr->value.MemberEnd() != xItr
			&& xItr->value.IsInt()
			&& frameResolutionItr->value.MemberEnd() != yItr
			&& yItr->value.IsInt()
			&& frameResolutionItr->value.MemberEnd() != scaleItr
			&& scaleItr->value.IsFloat()
				)
			frameResolution = sDisplaySize(xItr->value.GetInt(), yItr->value.GetInt(), scaleItr->value.GetFloat());
	}

	const auto largeResolutionItr = doc.FindMember("largeResolution");
	if (doc.MemberEnd() != largeResolutionItr) {
		const auto xItr = largeResolutionItr->value.FindMember("x");
		const auto yItr = largeResolutionItr->value.FindMember("y");
		const auto scaleItr = largeResolutionItr->value.FindMember("desktopScale");
		if (largeResolutionItr->value.MemberEnd() != xItr
			&& xItr->value.IsInt()
			&& frameResolutionItr->value.MemberEnd() != yItr
			&& yItr->value.IsInt()
			&& frameResolutionItr->value.MemberEnd() != scaleItr
			&& scaleItr->value.IsFloat()
				)
			largeResolution = sDisplaySize(xItr->value.GetInt(), yItr->value.GetInt(), scaleItr->value.GetFloat());
	}

	const auto showDisplayStatsItr = doc.FindMember("showDisplayStats");
	if (doc.MemberEnd() != showDisplayStatsItr)
		stats = showDisplayStatsItr->value.GetBool();

	return settingManager(frameResolution, largeResolution, stats);
}