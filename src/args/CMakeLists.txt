include(${DEPENDENCIES}/argparse.cmake)

add_library(args OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
)
target_include_directories(args 
	PRIVATE
	${TARGET_INCLUDE_DIRECTORY}
	${ARGPARSE_INCLUDE_DIR}
)
add_dependencies(args
	argparse_lib 
)
target_compile_definitions(args
 PRIVATE
 VERSION="${PROJECT_VERSION}"
)
