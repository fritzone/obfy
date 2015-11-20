cmake_minimum_required(VERSION 2.8)
project(obfy)

if(WIN32)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wextra")
endif()

LINK_DIRECTORIES(${CMAKE_LIBRARY_PATH})

set(SOURCE_FILES instr.cpp)
add_library(obfy ${SOURCE_FILES})

if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)	 	
endif()

add_executable(obfy_test main.cpp)

enable_testing()

if(WIN32)
  target_link_libraries(obfy_test
                        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                        obfy
  )
else()
  target_link_libraries(obfy_test
  		    ${Boost_FILESYSTEM_LIBRARY}
  		    ${Boost_SYSTEM_LIBRARY}
        	${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
          obfy
  )
endif()

add_test(NAME obfy_test COMMAND obfy_test)
