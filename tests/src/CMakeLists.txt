file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(utest ${SRCS})

set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)

find_package(PostgreSQL REQUIRED)
find_package(Boost COMPONENTS log system REQUIRED )
include_directories(${Boost_INCLUDE_DIRS} ${PostgreSQL_INCLUDE_DIRS})
message("LIBRA ${Boost_LIBRARIES}")

TARGET_LINK_LIBRARIES(
	utest 
	Service_lib
    libgtest
	libgmock
	${PostgreSQL_LIBRARIES}
	${Boost_LIBRARIES}
)

add_test(NAME utest
         COMMAND utest)
