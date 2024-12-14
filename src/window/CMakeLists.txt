add_library(window OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Manager.cpp
)
target_include_directories(window PRIVATE
	${TARGET_INCLUDE_DIRECTORY}
)
