add_library(layouts OBJECT
	${CMAKE_CURRENT_SOURCE_DIR}/Maximized.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Grid.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StackVerticalRight.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StackVerticalLeft.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StackVerticalDouble.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StackHorizontalTop.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StackHorizontalBottom.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParallelVertical.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParallelHorizontal.cpp
)
target_include_directories(layouts PRIVATE
	${TARGET_INCLUDE_DIRECTORY}
)
