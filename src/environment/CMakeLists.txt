include(${DEPENDENCIES}/imlib2.cmake)

set(NATIVE_ENVIRONMENT_SRC)
set(INCLUDE_DIRS ${TARGET_INCLUDE_DIRECTORY} ${IMLIB2_INCLUDE_DIR})
set(SHARED_LIBS events ${IMLIB2_LIBRARIES})

if(${NATIVE_ENVIRONMENT} STREQUAL "X11")
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/Environment.cpp)
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/Commands.cpp)
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/ErrorHandling.cpp)
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/XHelpers.cpp)
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/Handlers.cpp)
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/BackgroundImageHandler.cpp)
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/ScreenshotHandler.cpp)
	list(APPEND NATIVE_ENVIRONMENT_SRC x11/X11ToAbstractEvent.cpp)
	list(APPEND INCUDE_DIRS ${X11_INCLUDE_DIR} ${X11_xkbfile_INCLUDE_PATH})
	list(APPEND SHARED_LIBS ${X11_LIBRARIES} ${X11_xkbfile_LIB})
endif()

add_library(environment OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Environment.cpp
	${NATIVE_ENVIRONMENT_SRC}
)
target_include_directories(environment PRIVATE
	${INCLUDE_DIRS}
)
target_link_libraries(environment PRIVATE
	${SHARED_LIBS}
	common
)
add_dependencies(environment
	events
	common
)
