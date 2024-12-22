add_library(layouts OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Maximized.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Grid.cpp
)
target_include_directories(layouts PRIVATE
	${TARGET_INCLUDE_DIRECTORY}
)
