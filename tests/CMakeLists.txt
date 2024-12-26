if(NOT ${BUILD_TESTS} OR NOT DEFINED BUILD_TESTS)
	message(WARNING Set -DBUILD_TESTS=TRUE to build tests)
	return()
endif()

set(SRC_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)

include(${DEPENDENCIES}/gtest.cmake)
enable_testing()

add_subdirectory(events)
add_subdirectory(layouts)
add_subdirectory(window)
