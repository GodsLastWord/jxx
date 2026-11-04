file(GLOB_RECURSE JXXC_SOURCE_FILES "src/*.cpp")

add_executable(jxxc ${JXXC_SOURCE_FILES})

target_include_directories(jxxc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(jxxc PRIVATE jxxu)
