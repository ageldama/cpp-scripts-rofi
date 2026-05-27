cmake_minimum_required(VERSION 3.15)

project(c-scripts-rofi
  VERSION 2.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_compile_options(-Wall -Wextra -Wpedantic)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb3 -fstack-protector -fsanitize=address,undefined")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# --- fmt: clang-format

find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)
  message(STATUS "Found clang-format: ${CLANG_FORMAT_EXE}")

  file(GLOB_RECURSE ALL_FORMAT_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
  )

  add_custom_target(fmt
    COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_FORMAT_FILES}
    COMMENT "Formatting source files with clang-format..."
  )
else()
  message(WARNING "clang-format not found. 'format' target will not be available.")
endif()


# --- tidy: clang-tidy
find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
  message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}")

  file(GLOB_RECURSE ALL_TIDY_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

  add_custom_target(tidy
    COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy --use-color ${ALL_TIDY_FILES}
    COMMENT "Running Clang-Tidy static analysis..."
  )
else()
  message(WARNING "clang-tidy not found. 'tidy' target will not be available.")
endif()


# --- doxygen

find_package(Doxygen)

if (DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")

  doxygen_add_docs(docs
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    COMMENT "Generating API documentation"
  )
else()
  message(STATUS "Doxygen not found, documentation target will not be created.")
endif()


# --- executable: scripts-rofi

add_executable(scripts-rofi ${SOURCES})

target_include_directories(scripts-rofi PUBLIC include)
