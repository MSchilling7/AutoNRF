cmake_minimum_required(VERSION 3.3)
project(AutoNRF)
set(CMAKE_BUILD_TYPE DEBUG)
set(PROJECT_VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Using C++${CMAKE_CXX_STANDARD}")

#if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  #set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
#endif()
#set(CMAKE_ALLOWED_BUILD_TYPES DEBUG RELEASE RELWITHDEBINFO)
#if(NOT ";${CMAKE_BUILD_TYPE};" MATCHES ";${CMAKE_ALLOWED_BUILD_TYPES};")
  #message(FATAL_ERROR "Invalid build type ${CMAKE_BUILD_TYPE}. Use one of: ${CMAKE_ALLOWED_BUILD_TYPES}.")
#endif()
#set(CMAKE_VERBOSE_MAKEFILE OFF)
#set(CMAKE_RULE_MESSAGES OFF)
#set(CMAKE_CXX_FLAGS_DEBUG "-Wall -O0 -ggdb -DDEBUG -D__DEBUG")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -ggdb")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -ftree-vectorize -ffast-math -DNODEBUG")
#message(STATUS "Using CXX flags for ${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")


include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${N})
  set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
  set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j ${N}")      
endif()

set(SOURCES
      src/DataReader.cpp
      src/Efficiency.cpp
      src/Flux.cpp
      src/Functions.cpp
      src/Gamma.cpp  
      src/Output.cpp
        )


set(PROJECT_BINARY_DIR ./install)

string(REPLACE ".cpp" ".h" HEADERS_ "${SOURCES}")
string(REPLACE "src/" "" HEADERS "${HEADERS_}")

# Telling CMake where to find the ROOT installation.
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/lib")

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS Core MathCore RIO Hist Tree)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
add_definitions(${ROOT_CXX_FLAGS})

include_directories(${CMAKE_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

set(DICT_FILE_NAME G__${PROJECT_NAME})
set(PCM_FILE_NAME lib${PROJECT_NAME})

ROOT_GENERATE_DICTIONARY(${DICT_FILE_NAME} ${HEADERS} LINKDEF include/AutoNRFLinkDef.h MODULE ${PROJECT_NAME})
add_library(AutoNRF SHARED ${SOURCES} ${DICT_FILE_NAME} )
target_link_libraries(AutoNRF ${ROOT_LIBRARIES})





add_executable(autoNRF src/main.cpp)
target_include_directories(autoNRF PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(autoNRF ${ROOT_LIBRARIES} AutoNRF )

find_package(OpenMP) 
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_custom_target(new COMMAND @rm -rf ${CMAKE_CURRENT_BINARY_DIR}/* && cmake ${CMAKE_SOURCE_DIR} && make -j)
add_custom_target(purge COMMAND @rm -rf ${CMAKE_CURRENT_BINARY_DIR}/*)



set(INSTALL ${CMAKE_CURRENT_BINARY_DIR}/install)

if(NOT DEFINED ${CMAKE_INSTALL_PREFIX})
  if(NOT EXISTS ${INSTALL})
    file(MAKE_DIRECTORY ${INSTALL})
  endif()
  set(CMAKE_INSTALL_PREFIX ${INSTALL})
endif()

add_executable(makeInputFile src/makeInputFile.cpp)


install(TARGETS AutoNRF autoNRF makeInputFile
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        )


INSTALL( FILES ${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}_rdict.pcm 
             DESTINATION ${CMAKE_INSTALL_PREFIX}
             COMPONENT dict)


install(DIRECTORY ${CMAKE_SOURCE_DIR}/
       DESTINATION ${CMAKE_INSTALL_PREFIX}
       PATTERN "build" EXCLUDE
       PATTERN "Output" EXCLUDE
       PATTERN ".git" EXCLUDE
       PATTERN ".gitignore" EXCLUDE
       PATTERN "CMakeLists.txt" EXCLUDE 
  )
