add_library(checks OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Check.cpp
)
target_include_directories(checks PRIVATE
	${TARGET_INCLUDE_DIRECTORY}
)
target_link_libraries(checks PRIVATE
	environment
)
add_dependencies(checks
	environment
)

