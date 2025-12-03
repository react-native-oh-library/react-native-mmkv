cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)
set(rnoh_native_mmkv_generated_dir "${RNOH_GENERATED_DIR}/react_native_harmony_native_camera")
file(GLOB rnoh_native_mmkv_SRC CONFIGURE_DEPENDS *.cpp)
add_library(rnoh_native_mmkv SHARED ${rnoh_native_mmkv_SRC}
                                     HarmonyLogger.cpp
                                     )

target_include_directories(rnoh_native_mmkv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(rnoh_native_mmkv PUBLIC MMKV/Core)

add_subdirectory(MMKV/Core core)

target_link_libraries(rnoh_native_mmkv PUBLIC rnoh core libace_napi.z.so libhilog_ndk.z.so)
