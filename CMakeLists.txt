cmake_minimum_required(VERSION 2.8)
project(obfy)

set(SOURCE_FILES instr.cpp)

option(unit_tests "Build unit tests. Requries Boost Unit Tests" ON)

if(NOT WIN32)
    option(coverage-build "gcov/lcov test coverage analysis (make coverage_test)" OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wextra -Werror")
  endif()

if(WIN32)
  add_definitions("/EHsc")
  if(unit_tests)
  
      set(BOOST_ROOT "e:/work/boost_1_60_0/")
      LINK_DIRECTORIES("${BOOST_ROOT}/stage/lib")

      set(SOURCE_FILES ${SOURCE_FILES} 
          # ${BOOST_ROOT}/libs/test/src/framework.cpp 
          ${BOOST_ROOT}/libs/test/src/unit_test_main.cpp 
          ${BOOST_ROOT}/libs/test/src/test_tools.cpp
          )

  endif()

  find_package(Boost)
  if(NOT Boost_FOUND)
        message("WIN32: Cannot find boost, unit testing disabled")
        set(unit_tests OFF)
  else()
        include_directories(${Boost_INCLUDE_DIRS})
  endif() 
else()
  find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
endif()

LINK_DIRECTORIES(${CMAKE_LIBRARY_PATH})

if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif()

if(unit_tests)
    add_executable(obfy_test main.cpp ${SOURCE_FILES})
    enable_testing()

    if(WIN32)
      target_link_libraries(obfy_test
              ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
      )
    else()
      target_link_libraries(obfy_test
      		    ${Boost_FILESYSTEM_LIBRARY}
      		    ${Boost_SYSTEM_LIBRARY}
              ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
      )
    endif()

  add_test(NAME obfy_test COMMAND obfy_test)
  endif()

if(coverage-build)
   set(CMAKE_BUILD_TYPE Debug)
   message("building for coverage ...")
   SET(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -rdynamic")

   include(cmake/CodeCoverage.cmake)
   SETUP_TARGET_FOR_COVERAGE(coverage_test ctest coverage)
endif()
