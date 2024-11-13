set(GTEST_DIR ${CMAKE_BINARY_DIR}/googletest)
set(GTEST_INSTALL_DIR ${CMAKE_BINARY_DIR}/gtest_lib)
set(GTEST_REPO https://github.com/google/googletest.git)

list(APPEND GTEST_INCLUDE_DIRS 
	${GTEST_DIR}/googlemock/include 
	${GTEST_DIR}/googletest/include
)

list(APPEND GTEST_LIBRARIES 
	${GTEST_INSTALL_DIR}/lib/libgtest.a
	${GTEST_INSTALL_DIR}/lib/libgtest_main.a
	${GTEST_INSTALL_DIR}/lib/libgmock.a
	${GTEST_INSTALL_DIR}/lib/libgmock_main.a
)

if(NOT EXISTS ${GTEST_DIR})
	add_custom_command(OUTPUT ${GTEST_DIR} ${GTEST_INCLUDE_DIRS}
		COMMAND git clone ${GTEST_REPO}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
endif()

add_custom_command(OUTPUT ${GTEST_LIBRARIES} 
	COMMAND cmake -S . -B build -DBUILD_GMOCK=ON -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_DIR}
	COMMAND cmake --build build -- -j `nproc`
	COMMAND cmake --install build
	DEPENDS ${GTEST_DIR}
	WORKING_DIRECTORY ${GTEST_DIR}
)

add_custom_target(googletestlib DEPENDS
	${GTEST_DIR}
	${GTEST_LIBRARIES}
)
