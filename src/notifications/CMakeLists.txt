add_library(notifications OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Notification.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/System.cpp
)
add_dependencies(notifications
	socket
)
target_link_libraries(notifications PUBLIC socket)
