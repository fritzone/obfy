cmake_minimum_required(VERSION 2.8)
project(obfy)

if(NOT WIN32)
    option(unit_tests "Build unit tests. Requries Boost Unit Tests" OFF)
    option(coverage-build "gcov/lcov test coverage analysis (make coverage_test)" OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wextra -Werror")
else()
    add_definitions("/EHsc")
endif()

add_executable(obfy_test main.cpp ${SOURCE_FILES})

if(unit_tests)

    find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
    if(NOT Boost_FOUND)
        set(unit_tests OFF)
    else()
        add_definitions("-DUNIT_TESTS")
        include_directories(${Boost_INCLUDE_DIRS})
    endif() 

    enable_testing()

    target_link_libraries(obfy_test
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

    add_test(NAME obfy_test COMMAND obfy_test)
endif()

if(coverage-build)
   set(CMAKE_BUILD_TYPE Debug)
   message("building for coverage ...")
   SET(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -rdynamic")

   include(cmake/CodeCoverage.cmake)
   SETUP_TARGET_FOR_COVERAGE(coverage_test ctest coverage)
endif()
