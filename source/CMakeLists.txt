set(CRADLE_VERSION "0.1")
set(CRADLE_VERSION_MAJOR "1")
set(CRADLE_VERSION_MINOR "0")
set(CRADLE_LAST_UPDATE "April 29, 2019")

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

find_package(spdlog REQUIRED)
include_directories(${spdlog_INCLUDE_DIRS})

set(NHL_INCLUDE_DIRS "/mnt/ksf9/H2/user/u0111340/linux/Code/LeendertProject/NHL/include")
set(PDS_INCLUDE_DIRS "/mnt/ksf9/H2/user/u0111340/linux/Code/LeendertProject/ParticleDataStructure/include")


#include_directories(${PROJECT_SOURCE_DIR}/source/include)
include_directories(NHL_INCLUDE_DIRS)
include_directories(PDS_INCLUDE_DIRS)

configure_file("${PROJECT_SOURCE_DIR}/cmake/Templates/CRADLEConfig.h.in" "${PROJECT_BINARY_DIR}/CRADLEConfig.h")
include_directories("${PROJECT_BINARY_DIR}")

if (USE_BSG)
find_package(BSG REQUIRED)
include_directories(${BSG_INCLUDE_DIRS})
set(EXTRA_LIBS ${EXTRA_LIBS} bsg nme)
endif()

file(GLOB sources ${PROJECT_SOURCE_DIR}/source/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/source/include/*.hh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread")

add_executable(CRADLE++ CRADLE++.cc ${sources} ${headers})

target_link_libraries(CRADLE++ ${Boost_LIBRARIES})
target_link_libraries(CRADLE++ ${GSL_LIBRARIES})
target_link_libraries(CRADLE++ ${EXTRA_LIBS})

add_custom_command(TARGET CRADLE++
                   POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:CRADLE++> ${PROJECT_BINARY_DIR}/bin/$<TARGET_FILE_NAME:CRADLE++>)

install(TARGETS CRADLE++ DESTINATION bin)
