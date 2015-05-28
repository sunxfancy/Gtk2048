cmake_minimum_required(VERSION 2.8)
project(gtk2048)
SET(CMAKE_CXX_COMPILER_ENV_VAR "g++")

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(game ${source_files})
target_link_libraries(game ${GTKMM_LIBRARIES})

FIND_PACKAGE(Doxygen)  
OPTION(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})  
  
IF(BUILD_DOCUMENTATION)  
    IF(NOT DOXYGEN_FOUND)  
        MESSAGE(FATAL_ERROR "Doxygen is needed to build the documentation.")  
    ENDIF()  
  
    SET(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)  
    SET(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)  
  
    CONFIGURE_FILE(${doxyfile_in} ${doxyfile} @ONLY)  
  
    ADD_CUSTOM_TARGET(doc  
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}  
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}  
        COMMENT "Generating API documentation with Doxygen"  
        VERBATIM)  
  
    INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)  
ENDIF()  
