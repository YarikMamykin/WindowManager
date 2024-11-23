include(${DEPENDENCIES}/yaml-cpp.cmake)

set(NATIVE_ENVIRONMENT_SRC)
set(NATIVE_ENVIRONMENT X11)

if(${NATIVE_ENVIRONMENT} STREQUAL "X11")
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/KeyCodes.cpp)
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/KeyMasks.cpp)
endif()

add_library(events OBJECT 
	Map.cpp 
	${NATIVE_ENVIRONMENT_SRC}
)
target_include_directories(events PRIVATE 
	${TARGET_INCLUDE_DIRECTORY}
	${YAML_INCLUDE_DIR}
)
target_link_libraries(events PRIVATE
	${YAML_SHARED_LIB}
)
add_dependencies(events
	yaml_cpp
)
