file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/QPS)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/SP)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/PKB)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/utils)

target_link_libraries(unit_testing spa)