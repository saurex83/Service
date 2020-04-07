#include "debug.hpp"

void debugInit(){
	spdlog::set_pattern("[%d-%m-%Y %H:%M:%S.%e] [thread %t, process %P] [%s:%#] %v");
}
