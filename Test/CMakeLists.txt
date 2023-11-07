find_package(GTest CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

include(GoogleTest)

enable_testing()

set(This Test)

add_executable(${This}
	CppSlippiTest.cpp
	Model/Enums/ActionStateTest.cpp
	Model/Enums/CostumeTest.cpp
	Model/PrintTo.cpp
	Parsing/BlockReaderTest.cpp
	Parsing/FrameEventProcessorTest.cpp
	Parsing/ShiftJisTest.cpp
	Reading/ReaderTest.cpp
	Utils/EndianTest.cpp
	Utils/PortArrayTest.cpp)

target_include_directories(${This} PRIVATE .)

target_compile_features(${This} PRIVATE cxx_std_20)

set_target_properties(${This} PROPERTIES CXX_EXTENSIONS OFF)

if(MSVC)
	target_compile_options(${This} PRIVATE /utf-8 /bigobj)
endif()

target_link_libraries(${This} PRIVATE
	CppSlippi
	nlohmann_json::nlohmann_json
	GTest::gmock_main)

# Due to the wonky behavior of gtest_discover_tests, we need to use an
# absolute path in order to both build and run with ctest correctly.
gtest_discover_tests(${This}
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Test/replays"
	DISCOVERY_MODE POST_BUILD)
