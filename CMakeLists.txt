cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

string(REGEX REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
string(REGEX REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG
                     "${CMAKE_CXX_FLAGS_DEBUG}")

project(terra VERSION 0.1)
set(CMAKE_DEPENDENCIES_DIR ${CMAKE_SOURCE_DIR}/dependencies)

# Necessary libraries
find_package(Vulkan REQUIRED)
find_package(Vulkan COMPONENTS glslc)
find_program(
  glslc_executable
  NAMES glslc
  HINTS Vulkan::glslc)

# Compiles GLSL shaders into SPIR-V
function(compile_shader target)
  cmake_parse_arguments(PARSE_ARGV 1 arg "" "ENV;FORMAT" "SOURCES")
  if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    get_target_property(OUTDIR ${target} RUNTIME_OUTPUT_DIRECTORY_RELEASE)
  else()
    get_target_property(OUTDIR ${target} RUNTIME_OUTPUT_DIRECTORY_DEBUG)
  endif()
  foreach(source ${arg_SOURCES})
    get_filename_component(filename ${source} NAME)
    add_custom_command(
      OUTPUT ${OUTDIR}/${filename}.spv
      DEPENDS ${source}
      DEPFILE ${filename}.d
      COMMAND
        ${glslc_executable} $<$<BOOL:${arg_ENV}>:--target-env=${arg_ENV}>
        $<$<BOOL:${arg_FORMAT}>:-mfmt=${arg_FORMAT}> -MD -MF ${filename}.d -o
        ${OUTDIR}/${filename}.spv ${source})
    target_sources(${target} PRIVATE ${OUTDIR}/${filename}.spv)
  endforeach()
endfunction()

# Output for files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/pdb)
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/pdb)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib/debug)
set(CMAKE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/pdb/debug)
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/pdb/debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/pdb)
set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/pdb)

# Directory of the code
set(PROJECT_SOURCE_DIR_LOCATION ${CMAKE_SOURCE_DIR}/src)

# Constants for the project
if(MSVC)
  set(PROJECT_COMPILE_OPTIONS /WX /W4)
else()
  set(PROJECT_COMPILE_OPTIONS
      -Wall
      -Wextra
      -Werror
      -Wno-unused-parameter
      -Wno-unused-function
      -Wconversion
      -Wsign-conversion
      -pedantic
      -pedantic-errors)
endif()

# Names for the different libraries in the project
set(PROJECT_UTILS_NAME ${PROJECT_NAME}_utils)
set(PROJECT_CORE_NAME ${PROJECT_NAME}_core)
set(PROJECT_VMA_NAME ${PROJECT_NAME}_vma)
set(PROJECT_HEADERS ${CMAKE_SOURCE_DIR}/include)

option(TERRA_BUILD_EXAMPLES "Build the examples that come with the package."
       OFF)
option(TERRA_BUILD_TESTS "Build the tests that come with the package." OFF)
option(TERRA_INSTALL "Install target." OFF)

add_compile_definitions(LOG_USE_COLOR)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  add_compile_definitions(TERRA_BUILD_TYPE_RELEASE)
  add_compile_definitions(TERRA_DISABLE_LOGGING)
else()
  add_compile_definitions(TERRA_BUILD_TYPE_DEBUG)
endif()

string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

# Subdirectories

add_subdirectory(dependencies)
add_subdirectory(src)

if(${TERRA_BUILD_EXAMPLES})
  add_subdirectory(examples)
endif()

if(${TERRA_BUILD_TESTS})
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()
