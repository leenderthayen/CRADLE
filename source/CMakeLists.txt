cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(CRADLE++)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_MATH} "${CMAKE_SOURCE_DIR}")

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

FIND_PACKAGE(GSL REQUIRED)
include_directories(${GSL_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
#file(COPY RadiationData DESTINATION .)
#file(COPY GammaData DESTINATION .)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread")

add_executable(CRADLE++ CRADLE++.cc ${sources} ${headers})

target_link_libraries(CRADLE++ ${Boost_LIBRARIES})
target_link_libraries(CRADLE++ ${GSL_LIBRARIES})

set(CRADLE++_SCRIPTS
  config.txt
  visualisation.py
)

foreach (_script ${CRADLE++_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

install(TARGETS CRADLE++ DESTINATION bin)
