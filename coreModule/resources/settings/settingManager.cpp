#include "settingManager.h"
#include "json/document.h"
#include "json/ostreamwrapper.h"
#include "debugModule/logManager.h"

using namespace common::coreModule;

settingManager::settingManager(cocos2d::Size frameResolution, const cocos2d::Size largeResolution, const bool showDisplayStats) :
		frameResolutionSize(frameResolution )
		, largeResolutionSize(largeResolution )
		, showDisplayStats( showDisplayStats )
{}

settingManager settingManager::load()
{
	// default
	cocos2d::Size frameResolution( 1024, 768 );
	cocos2d::Size largeResolution( 480, 320 );
	auto stats = false;

	// load json
	const std::string& regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile( "config/setting.json" );
	rapidjson::Document doc;
	doc.Parse<0>( regionStr.c_str() );

	if( doc.HasParseError() )
	{
		LOG_ERROR("settingManager::load: json parse error");
		return settingManager(frameResolution, largeResolution, stats);
	}

	if( doc.IsNull() )
	{
		LOG_ERROR("settingManager::load: json is empty");
		return settingManager(frameResolution, largeResolution, stats);
	}

	const auto frameResolutionItr = doc.FindMember( "frameResolution" );
	if( doc.MemberEnd() != frameResolutionItr )
	{
		const auto xItr = frameResolutionItr->value.FindMember( "x" );
		const auto yItr = frameResolutionItr->value.FindMember( "y" );
		if( frameResolutionItr->value.MemberEnd() != xItr
			&& xItr->value.IsInt()
			&& frameResolutionItr->value.MemberEnd() != yItr
			&& yItr->value.IsInt() )
			frameResolution.setSize( xItr->value.GetInt(), yItr->value.GetInt() );
	}

	const auto largeResolutionItr = doc.FindMember( "largeResolution" );
	if( doc.MemberEnd() != largeResolutionItr )
	{
		const auto xItr = largeResolutionItr->value.FindMember( "x" );
		const auto yItr = largeResolutionItr->value.FindMember( "y" );
		if( largeResolutionItr->value.MemberEnd() != xItr
			&& xItr->value.IsInt()
			&& frameResolutionItr->value.MemberEnd() != yItr
			&& yItr->value.IsInt() )
			largeResolution.setSize( xItr->value.GetInt(), yItr->value.GetInt() );
	}

	const auto showDisplayStatsItr = doc.FindMember( "showDisplayStats" );
	if( doc.MemberEnd() != showDisplayStatsItr )
		stats = showDisplayStatsItr->value.GetBool();

	return settingManager( frameResolution, largeResolution, stats );
}