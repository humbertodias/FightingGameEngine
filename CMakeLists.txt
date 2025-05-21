project(kusoge)

cmake_minimum_required(VERSION 3.9)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_EXE_LINKER_FLAGS "-Wl,-no_pie,-export_dynamic")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(MY_CXX_COMPILE_FEATURES cxx_generic_lambdas cxx_range_for cxx_strong_enums)
set(JSON_BuildTests OFF CACHE INTERNAL "")

file(GLOB_RECURSE SOURCE src/*.cpp)
file(GLOB_RECURSE HEADER include/*.h)

find_package(PkgConfig REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_Mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Boost 1.55 REQUIRED system serialization)
# find_package(Gperftools REQUIRED COMPONENTS TCMALLOC_AND_PROFILER)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party/ggpo/src/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2MIXER_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    # ${Gperftools_INCLUDE_DIRS}
)

add_executable(kusoge ${SOURCE} ${HEADER})

# add_custom_command(TARGET kusoge
#     POST_BUILD COMMAND
#     ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@executable_path/../third_party/"
#     $<TARGET_FILE:kusoge>
# )

target_link_libraries(
    kusoge
    PRIVATE
    nlohmann_json::nlohmann_json
    # ${PROJECT_SOURCE_DIR}/third_party/ggpo/build/src/libGGPO.a
    
    /usr/local/lib/libGGPO.so
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${Boost_LIBRARIES}
    SDL2_mixer::SDL2_mixer
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    
    # ${Gperftools_LIBRARIES}
)
