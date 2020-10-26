#ifndef COMMON_JSONLOADER_H
#define COMMON_JSONLOADER_H

#include <fstream>
#include "json/document.h"
#include "json/istreamwrapper.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/ostreamwrapper.h"
#include <string>

namespace common {

	namespace coreModule {
		class jsonLoader {
		public:
			jsonLoader();
			~jsonLoader();

			/**
			 * Method for load unparsed json, as is, with is object check
			 * @param path Path to json in resources folder
			 * @param prefix @only for gtest
			 * @return rapidjson::Document
			 */
			rapidjson::Document loadJson(const std::string &path);
		};
	}
}//common::coreModule


#endif //COMMON_JSONLOADER_H
