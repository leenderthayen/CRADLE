set(CRADLE_VERSION "0.1")
set(CRADLE_VERSION_MAJOR "1")
set(CRADLE_VERSION_MINOR "0")
set(CRADLE_LAST_UPDATE "April 29, 2019")

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/source/include)

configure_file("${PROJECT_SOURCE_DIR}/cmake/Templates/CRADLEConfig.h.in" "${PROJECT_BINARY_DIR}/CRADLEConfig.h")
include_directories("${PROJECT_BINARY_DIR}")

if (USE_BSG)
find_package(BSG REQUIRED)
include_directories(${BSG_INCLUDE_DIRS})
set(EXTRA_LIBS ${BSG_LIBRARIES} ${EXTRA_LIBS})
endif()

#Change to local install directory of BSG
set(BSG_INSTALL_DIR "/Users/leenderthayen/BSG")

include_directories("${BSG_INSTALL_DIR}/include")

file(GLOB sources ${PROJECT_SOURCE_DIR}/source/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/source/include/*.hh)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread -L/usr/local/Cellar/gsl/2.5/lib")

add_executable(CRADLE++ CRADLE++.cc ${sources} ${headers})

set(BSG_LIBRARIES "${BSG_INSTALL_DIR}/lib/nme/libnme.dylib;${BSG_INSTALL_DIR}/lib/bsg/libbsg.dylib")
message("${BSG_LIBRARIES}")

target_link_libraries(CRADLE++ ${Boost_LIBRARIES})
target_link_libraries(CRADLE++ ${GSL_LIBRARIES})
target_link_libraries(CRADLE++ ${EXTRA_LIBS})

message("${GSL_LIBRARY}")

add_custom_command(TARGET CRADLE++
                   POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:CRADLE++> ${PROJECT_BINARY_DIR}/bin/$<TARGET_FILE_NAME:CRADLE++>)

install(TARGETS CRADLE++ DESTINATION bin)
