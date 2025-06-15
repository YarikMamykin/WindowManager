include(${DEPENDENCIES}/libsocket.cmake)

add_library(socket OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Socket.cpp
)
add_dependencies(socket libsocket)
target_link_libraries(socket PRIVATE ${LIBSOCKET_LIB})
target_include_directories(socket PRIVATE ${LIBSOCKET_INCLUDE_DIR})
