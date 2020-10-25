#ifndef MERCENARY_BATTLES_LOGMANAGER_H
#define MERCENARY_BATTLES_LOGMANAGER_H

#include <string>
#include <iostream>
#include <vector>
#include <string>

#define GET_LOGGER() mb::debugModule::loggerInstance::getInstance()
#define LOG_INFO(W) GET_LOGGER().info(W)
#define LOG_WARNING(W) GET_LOGGER().warning(W)
#define LOG_ERROR(W) GET_LOGGER().error(W)

namespace mb {
	namespace debugModule {

		enum eLogTypes : size_t {
			INFO, WARNING, ERROR
		};

		struct sLogMessage {
			eLogTypes type;
			std::string message;

			sLogMessage(eLogTypes _type, std::string _message) {
				type = _type;
				message = _message;
			}
		};

		/***
		 * Данный класс нужен для логирования исключительных событий
		 */
		class logManager {
		public:
			void info(const std::string &message);

			void warning(const std::string &message);

			void error(const std::string &message);

		private:
			/**
			 * Получение префикса типа события
			 * @return std::string
			 */
			std::string getType(const eLogTypes &);

			void addLogMessage(sLogMessage *);

			std::vector<sLogMessage *> log{};
		};

		class loggerInstance {
		public:
			static logManager &getInstance();
		};
	}

}//mb::debugModule

#endif// MERCENARY_BATTLES_LOGMANAGER_H