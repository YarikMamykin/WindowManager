if(NOT ${BUILD_TESTS} OR NOT DEFINED BUILD_TESTS)
	message(WARNING Set -DBUILD_TESTS=TRUE to build tests)
	return()
endif()

include(${DEPENDENCIES}/gtest.cmake)
enable_testing()
