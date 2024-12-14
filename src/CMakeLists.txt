set(TARGET_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(${NATIVE_ENVIRONMENT} STREQUAL "X11")
	include(${DEPENDENCIES}/x11.cmake)
endif()

add_subdirectory(events)
add_subdirectory(window)
add_subdirectory(environment)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME}
	PRIVATE
	${TARGET_INCLUDE_DIRECTORY}
)
add_dependencies(${PROJECT_NAME}
	events
	environment
	window
)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
	events
	environment
	window
)
target_compile_options(${PROJECT_NAME}
	PRIVATE
	$<$<CONFIG:DEBUG>:-g>
	$<$<CONFIG:RELEASE>:-O3>
	-fPIC
	-Wall
	-Werror
	-Wextra
	-Wpedantic
)
