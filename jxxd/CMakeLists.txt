
file(GLOB_RECURSE JXXD_SOURCE_FILES "src/*.cpp")

add_executable(jxxd ${JXXD_SOURCE_FILES})

target_include_directories(jxxd PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(jxxd PRIVATE jxxu)
