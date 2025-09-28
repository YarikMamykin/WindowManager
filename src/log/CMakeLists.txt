include(${DEPENDENCIES}/spdlog.cmake)

add_library(log OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
)
target_include_directories(log PRIVATE 
	${SPDLOG_INCLUDE_DIR}
)
target_link_libraries(log PRIVATE
	${SPDLOG_LIBRARY}
)
add_dependencies(log
	spdloglib
)

