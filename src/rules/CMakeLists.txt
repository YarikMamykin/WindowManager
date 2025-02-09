add_library(rules OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Rule.cpp
)
target_include_directories(rules PRIVATE
	${TARGET_INCLUDE_DIRECTORY}
)
target_link_libraries(rules PRIVATE
	environment
)
add_dependencies(rules
	environment
)

