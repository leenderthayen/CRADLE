#-----------------------------------------------------------------------
# - Enforce an out-of-source builds before anything else
#
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "CRADLE++ requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()

cmake_minimum_required(VERSION 3.1)

project(CRADLE++ VERSION 1.0 LANGUAGES CXX)

# - Prepend our own CMake Modules to the search path
# NB: if our custom modules include others that we don't supply, those in
# the base path will be used, so watch for incompatibilities!!
#
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})
include(CMakeDependentOption)
include(GNUInstallDirs)
include(CMakeUninstallTarget)

#---------------------------------------------------------------------------------------
# compiler config
#---------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
    add_compile_options("-Wconversion")
    add_compile_options("-pedantic")
    add_compile_options("-Wfatal-errors")
    add_compile_options("-pthread")
endif()

# Offer the user the choice of overriding the installation directories
# set(INSTALL_LIB_DIR ${PROJECT_BINARY_DIR}/lib CACHE PATH "Installation directory for libraries")
# set(INSTALL_BIN_DIR ${PROJECT_BINARY_DIR}/bin CACHE PATH "Installation directory for executables")
# set(INSTALL_INCLUDE_DIR ${PROJECT_BINARY_DIR}/include CACHE PATH
#   "Installation directory for header files")

add_library(Cradle src/ConfigParser.cc src/Cradle.cc src/ReactionEngine.cc src/ReactionMode.cc src/SpectrumGenerator.cc)
add_executable(CRADLE++ src/CRADLE++.cc)

find_package(Boost)
find_package(GSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(Threads)

option(USE_BSG "Use the external BSG library for the calculation of beta spectrum shapes" OFF)
option(CRADLE_BUILD_TESTS "Build tests" ON)

set(NHL_INCLUDE_DIRS "/Users/leenderthayen/git/NHL/include")
set(PDS_INCLUDE_DIRS "/Users/leenderthayen/git/PDS/include")

target_include_directories(Cradle PUBLIC ${NHL_INCLUDE_DIRS})
target_include_directories(Cradle PUBLIC ${PDS_INCLUDE_DIRS})
target_include_directories(Cradle PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(Cradle PUBLIC ${GSL_INCLUDE_DIRS})
target_include_directories(Cradle PUBLIC ${spdlog_INCLUDE_DIRS})

target_include_directories(
    Cradle
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
set(HEADER_BASE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (USE_BSG)
find_package(BSG REQUIRED)
target_include_directories(CRADLE++ PUBLIC ${BSG_INCLUDE_DIRS})
set(EXTRA_LIBS ${EXTRA_LIBS} bsg nme)
endif()

if(CRADLE_BUILD_TESTS)
    include(CTest)
    add_subdirectory(tests)
endif()

target_link_libraries(Cradle PUBLIC ${Boost_LIBRARIES})
target_link_libraries(Cradle PUBLIC ${GSL_LIBRARIES})
target_link_libraries(Cradle PUBLIC ${EXTRA_LIBS})
target_link_libraries(Cradle PUBLIC ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(CRADLE++ PRIVATE Cradle)

# add_custom_command(TARGET CRADLE++
#                    POST_BUILD
# 		   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:CRADLE++> ${PROJECT_BINARY_DIR}/bin/$<TARGET_FILE_NAME:CRADLE++>)
#
# install(TARGETS CRADLE++ DESTINATION bin)
