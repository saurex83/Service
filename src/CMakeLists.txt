project (Service)


#Настройка BOOST
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
#add_definitions(-DBOOST_LOG_DYN_LINK)

find_package(Poco REQUIRED Util Net Foundation NetSSL JSON)
#find_package(Poco)
find_package(PostgreSQL REQUIRED)
find_package(Boost COMPONENTS log system REQUIRED )
include_directories(${Boost_INCLUDE_DIRS} ${Poco_INCLUDE_DIRS} PRIVATE ${PostgreSQL_INCLUDE_DIRS})

set(SOURCES meta.cpp license.cpp valuechecker.cpp sysconfig.cpp main.cpp ServiceConfig.cpp SerialCom.cpp Transiver.cpp frame.cpp
	debug.cpp database.cpp dbdecthread.cpp agrthread.cpp gwthread.cpp httpserver.cpp)

add_executable(Service ${SOURCES})
add_library(Service_lib STATIC ${SOURCES})

target_link_libraries(Service ${Boost_LIBRARIES})
target_link_libraries(Service PRIVATE ${PostgreSQL_LIBRARIES})
target_link_libraries(Service PUBLIC ${Poco_LIBRARIES} )

message("LIBRA ${Boost_LIBRARIES}")
message("LIBRA ${Poco_LIBRARIES}")
# Автогенерация ctags
set_source_files_properties(tags PROPERTIES GENERATED true)
add_custom_target(tags
    COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_dependencies(Service tags)
