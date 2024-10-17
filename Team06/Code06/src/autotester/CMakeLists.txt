file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(autotester ${srcs})

target_link_libraries(autotester spa ${AUTOTESTER_LIBRARIES})


add_custom_target(copy_test_folders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../../../Tests06 ${CMAKE_CURRENT_BINARY_DIR}/Tests06
        )
add_dependencies(autotester copy_test_folders)

if (NOT WIN32)
    target_link_libraries(autotester pthread)
endif ()
