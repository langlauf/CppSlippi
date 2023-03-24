cmake_minimum_required(VERSION 3.15...3.24)

set(VERSION 1.0.3.14)

project(CppSlippi
	VERSION ${VERSION}
	DESCRIPTION "Slippi replay file parsing library for C++."
	LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CTest)

## Build the CppSlippi library.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(CppSlippi
	CppSlippi/CppSlippi.cpp
	CppSlippi/Model/Enums/ActionState.cpp
	CppSlippi/Model/Enums/Costume.cpp
	CppSlippi/Parsing/FrameEventProcessor.cpp
	CppSlippi/Parsing/Parse.cpp
	CppSlippi/Parsing/ShiftJis.cpp
	CppSlippi/Reading/FileReader.cpp
	CppSlippi/Reading/FixedBufferReader.cpp
	CppSlippi/Reading/Reader.cpp
	CppSlippi/Utils/Exceptions.cpp)
target_include_directories(CppSlippi PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_compile_features(CppSlippi PUBLIC cxx_std_20)
set_target_properties(CppSlippi PROPERTIES CXX_EXTENSIONS OFF)

if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_options(CppSlippi PRIVATE /GS-)
endif()

if(BUILD_SHARED_LIBS)
	target_compile_definitions(CppSlippi
		PRIVATE CPP_SLIPPI_EXPORT
		INTERFACE CPP_SLIPPI_SHARED)
endif ()

target_link_libraries(CppSlippi PUBLIC nlohmann_json::nlohmann_json)

## Tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	add_subdirectory(Test)
endif()

## Install
install(TARGETS CppSlippi
	EXPORT CppSlippiTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY CppSlippi DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	FILES_MATCHING PATTERN "*.h")
install(EXPORT CppSlippiTargets
	FILE CppSlippiTargets.cmake
	NAMESPACE CppSlippi::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CppSlippi)

## Configure
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/CppSlippi-config.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CppSlippi)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/CppSlippi-config-version.cmake"
	VERSION "${VERSION}"
	COMPATIBILITY AnyNewerVersion)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/CppSlippi-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/CppSlippi-config-version.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CppSlippi)

# Release
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND MAKE_RELEASE)
	add_subdirectory(Release)
endif()
