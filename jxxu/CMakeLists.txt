
file(GLOB_RECURSE JXXU_SOURCE_FILES "src/*.cpp")

add_library(jxxu STATIC ${JXXU_SOURCE_FILES})

target_include_directories(jxxu PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
