#include "debug.hpp"

void debugInit(){
	spdlog::set_level(spdlog::level::debug);
	spdlog::set_level(spdlog::level::trace);
	spdlog::set_pattern("[%d-%m-%Y %H:%M:%S.%e] [%8l] [%s:%#:%!] %v");
//	SPDLOG_TRACE("TRACE!");
//	SPDLOG_CRITICAL("CRITICAL");
}
