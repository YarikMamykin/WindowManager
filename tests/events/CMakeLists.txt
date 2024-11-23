add_executable(EventsTest
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EventsTest.cpp
)
target_include_directories(EventsTest
	PUBLIC
	${SRC_INCLUDE_DIR}
	${GTEST_INCLUDE_DIRS}
)
target_link_libraries(EventsTest
	${GTEST_LIBRARIES}
	events
)
add_dependencies(EventsTest
	googletestlib
	events
)
add_test(NAME EventsTest COMMAND EventsTest WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/key-bindings.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/key-bindings-without-masks.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/key-bindings-without-cmd-specified.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
