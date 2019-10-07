#-----------------------------------------------------------------------
# - Enforce an out-of-source builds before anything else
#
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "BSG requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()

cmake_minimum_required(VERSION 2.8)

project(CRADLE++)

# - Prepend our own CMake Modules to the search path
# NB: if our custom modules include others that we don't supply, those in
# the base path will be used, so watch for incompatibilities!!
#
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})

include(CMakeUninstallTarget)

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

set(HEADER_BASE "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_executable(CRADLE++ src/CRADLE++.cc src/ConfigParser.cc src/DecayManager.cc src/ReactionEngine.cc src/ReactionMode.cc src/SpectrumGenerator.cc)

find_package(Boost)
target_include_directories(CRADLE++ PUBLIC ${Boost_INCLUDE_DIRS})

find_package(GSL REQUIRED)
target_include_directories(CRADLE++ PUBLIC ${GSL_INCLUDE_DIRS})

find_package(spdlog REQUIRED)
target_include_directories(CRADLE++ PUBLIC ${spdlog_INCLUDE_DIRS})

find_package(Threads)

set(NHL_INCLUDE_DIRS "/mnt/ksf9/H2/user/u0111340/linux/Code/LeendertProject/NHL/include")
set(PDS_INCLUDE_DIRS "/mnt/ksf9/H2/user/u0111340/linux/Code/LeendertProject/ParticleDataStructure/include")

target_include_directories(CRADLE++ PUBLIC ${NHL_INCLUDE_DIRS})
target_include_directories(CRADLE++ PUBLIC ${PDS_INCLUDE_DIRS})
target_include_directories(CRADLE++ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

option(USE_BSG "Use the external BSG library for the calculation of beta spectrum shapes" OFF)
if (USE_BSG)
find_package(BSG REQUIRED)
include_directories(${BSG_INCLUDE_DIRS})
set(EXTRA_LIBS ${EXTRA_LIBS} bsg nme)
endif()

set(CRADLE++_SCRIPTS
  config/filter.py
  config/config.txt
  config/visualisation.py
)

foreach (_script ${CRADLE++_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

target_link_libraries(CRADLE++ ${Boost_LIBRARIES})
target_link_libraries(CRADLE++ ${GSL_LIBRARIES})
target_link_libraries(CRADLE++ ${EXTRA_LIBS})
target_link_libraries(CRADLE++ ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(TARGET CRADLE++
                   POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:CRADLE++> ${PROJECT_BINARY_DIR}/bin/$<TARGET_FILE_NAME:CRADLE++>)

install(TARGETS CRADLE++ DESTINATION bin)
