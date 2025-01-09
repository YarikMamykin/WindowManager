add_executable(RatioTest
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RatioTest.cpp
)
target_include_directories(RatioTest
	PUBLIC
	${SRC_INCLUDE_DIR}
	${GTEST_INCLUDE_DIRS}
)
target_link_libraries(RatioTest
	${GTEST_LIBRARIES}
	common
)
add_dependencies(RatioTest
	googletestlib
	common
)
add_test(NAME RatioTest COMMAND RatioTest WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
