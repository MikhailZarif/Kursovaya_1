cmake_minimum_required(VERSION 3.18)

set (CMAKE_CXX_STANDART 17)
set (CMAKE_CXX_STANDART_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_COVERAGE "Build code coverage" OFF)

include("cmake/HunterGate.cmake")

HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.314.tar.gz"
        SHA1 "95c47c92f68edb091b5d6d18924baabe02a6962a"
)

#project(template VERSION 0.1.0)
project(Kursovaya)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

hunter_add_package(nlohmann_json)
find_package(nlohmann_json 3.8.0 REQUIRED)

find_package(Boost 1.71.0 COMPONENTS system filesystem REQUIRED)

add_library(${PROJECT_NAME} STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/source/source.cpp)

add_executable(demo
        ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:include>"
        )

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

if(Boost_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(demo PUBLIC nlohmann_json::nlohmann_json ${PROJECT_NAME})

if(BUILD_TESTS)
    add_executable(tests
            # TODO: enum your files and delete this comment
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp)
    target_link_libraries(tests ${PROJECT_NAME} GTest::gtest_main)
    enable_testing()
    add_test(NAME unit_tests COMMAND tests)
endif()

if(BUILD_COVERAGE)
    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/coverage/cmake")
    find_package(codecov)
    string(APPEND CMAKE_CXX_FLAGS " --coverage")
    # add_coverage(${PROJECT_NAME})
    add_coverage(tests)
    coverage_evaluate()
endif()

include(CPackConfig.cmake)

install(TARGETS ${PROJECT_NAME} demo
        EXPORT "${PROJECT_NAME}-targets"
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        INCLUDES DESTINATION include
        )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

install(EXPORT "${PROJECT_NAME}-targets"
        NAMESPACE "${PROJECT_NAME}::"
        DESTINATION "lib/cmake"
        )