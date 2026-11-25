cmake_minimum_required(VERSION 3.14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

project(tests C)

set(EXPORT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXPORT_DIR}")

option (USE_SDL2 "Build with SDL2 instead of SDL3" OFF)

set(CMAKE_C_STANDARD 99)

if (USE_SDL2)
  find_package(SDL2 REQUIRED)
else()
  include(get_SDL3)
  get_SDL3("3.4.10")
endif()

set(PICO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../src")

set(base_sources
  ${PICO_DIR}/api.c
  ${PICO_DIR}/auxiliary.c
  ${PICO_DIR}/memory.c
  ${PICO_DIR}/p8scii.c
  ${PICO_DIR}/z8lua/lapi.c
  ${PICO_DIR}/z8lua/lauxlib.c
  ${PICO_DIR}/z8lua/lbaselib.c
  ${PICO_DIR}/z8lua/lcode.c
  ${PICO_DIR}/z8lua/lcorolib.c
  ${PICO_DIR}/z8lua/lctype.c
  ${PICO_DIR}/z8lua/ldblib.c
  ${PICO_DIR}/z8lua/ldebug.c
  ${PICO_DIR}/z8lua/ldo.c
  ${PICO_DIR}/z8lua/ldump.c
  ${PICO_DIR}/z8lua/lfunc.c
  ${PICO_DIR}/z8lua/lgc.c
  ${PICO_DIR}/z8lua/linit.c
  ${PICO_DIR}/z8lua/llex.c
  ${PICO_DIR}/z8lua/lmem.c
  ${PICO_DIR}/z8lua/lobject.c
  ${PICO_DIR}/z8lua/lopcodes.c
  ${PICO_DIR}/z8lua/lparser.c
  ${PICO_DIR}/z8lua/lpico8lib.c
  ${PICO_DIR}/z8lua/lstate.c
  ${PICO_DIR}/z8lua/lstring.c
  ${PICO_DIR}/z8lua/lstrlib.c
  ${PICO_DIR}/z8lua/ltable.c
  ${PICO_DIR}/z8lua/ltablib.c
  ${PICO_DIR}/z8lua/ltm.c
  ${PICO_DIR}/z8lua/lundump.c
  ${PICO_DIR}/z8lua/lvm.c
  ${PICO_DIR}/z8lua/lzio.c)

set(tests_sources ${CMAKE_CURRENT_SOURCE_DIR}/tests.c)

add_executable(tests ${base_sources} ${tests_sources})
if (USE_SDL2)
  target_compile_definitions(tests PRIVATE USE_SDL2)
  if (TARGET SDL2::SDL2main)
    target_link_libraries(tests PRIVATE SDL2::SDL2main)
  endif()
  target_link_libraries(tests PRIVATE SDL2::SDL2)
  if (UNIX)
    target_link_libraries(tests PRIVATE m)
  endif()
else()
  target_link_libraries(tests PRIVATE ${SDL3_LIBRARIES})
endif()

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src")
if (NOT USE_SDL2)
  include_directories("${sdl3_SOURCE_DIR}/include")
endif()
