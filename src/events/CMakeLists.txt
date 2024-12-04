include(${DEPENDENCIES}/yaml-cpp.cmake)

set(NATIVE_ENVIRONMENT_SRC)
set(INCLUDE_DIRS ${TARGET_INCLUDE_DIRECTORY} ${YAML_INCLUDE_DIR})
set(SHARED_LIBS ${YAML_SHARED_LIB})

if(${NATIVE_ENVIRONMENT} STREQUAL "X11")
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/KeyCodes.cpp)
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/KeyMasks.cpp)
	list(APPEND INCUDE_DIRS ${X11_INCLUDE_DIR})
	list(APPEND SHARED_LIBS ${X11_LIBRARIES})
endif()

add_library(events OBJECT 
	Map.cpp 
	${NATIVE_ENVIRONMENT_SRC}
)
target_include_directories(events PRIVATE 
	${INCLUDE_DIRS}
)
target_link_libraries(events PRIVATE
	${SHARED_LIBS}
)
add_dependencies(events
	yaml_cpp
)
