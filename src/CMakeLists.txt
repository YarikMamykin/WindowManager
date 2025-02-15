set(TARGET_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include(${DEPENDENCIES}/argparse.cmake)

if(${NATIVE_ENVIRONMENT} STREQUAL "X11")
	include(${DEPENDENCIES}/x11.cmake)
endif()

add_subdirectory(common)
add_subdirectory(config)
add_subdirectory(events)
add_subdirectory(layouts)
add_subdirectory(environment)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME}
	PRIVATE
	${TARGET_INCLUDE_DIRECTORY}
	${ARGPARSE_INCLUDE_DIR}
)
add_dependencies(${PROJECT_NAME}
	events
	environment
	layouts
	common
	config
	argparse_lib 
)
target_link_libraries(${PROJECT_NAME}
	PRIVATE
	events
	environment
	layouts
	common
	config
)
target_compile_options(${PROJECT_NAME}
	PRIVATE
	$<$<CONFIG:DEBUG>:-g>
	$<$<CONFIG:RELEASE>:-O3>
	-fPIC
	-fno-rtti
	-Wall
	-Werror
	-Wextra
	-Wpedantic
)
target_compile_definitions(${PROJECT_NAME}
 PRIVATE
 VERSION="${PROJECT_VERSION}"
)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
