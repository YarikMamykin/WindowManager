include(${DEPENDENCIES}/yaml-cpp.cmake)

add_library(config OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
)
target_include_directories(config PRIVATE 
	${TARGET_INCLUDE_DIRECTORY} 
	${YAML_INCLUDE_DIR}
)
target_link_libraries(config PRIVATE
	${YAML_LIB}
)
add_dependencies(config
	yaml_cpp
)
