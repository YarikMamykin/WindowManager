add_executable(ConfigTest
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParserTest.cpp
)
target_include_directories(ConfigTest
	PUBLIC
	${SRC_INCLUDE_DIR}
	${GTEST_INCLUDE_DIRS}
)
target_link_libraries(ConfigTest
	${GTEST_LIBRARIES}
	config
	events
	common
)
add_dependencies(ConfigTest
	googletestlib
	config
	events
	common
)
add_test(NAME ConfigTest COMMAND ConfigTest WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/key-bindings.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/key-bindings-without-masks.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/key-bindings-without-cmd-specified.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/layouts.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/windows.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/all-config.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/key-bindings-default-overriden.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/misc.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
