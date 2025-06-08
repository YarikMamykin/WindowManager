include(ExternalProject)

set(GTEST_DIR ${CMAKE_BINARY_DIR}/googletest)
set(GTEST_INSTALL_DIR ${CMAKE_BINARY_DIR}/libs/gtest)
set(GTEST_REPO https://github.com/google/googletest.git)
set(GTEST_INCLUDE_DIRS ${GTEST_INSTALL_DIR}/include)

list(APPEND GTEST_LIBRARIES
	${GTEST_INSTALL_DIR}/lib/libgtest.a
	${GTEST_INSTALL_DIR}/lib/libgtest_main.a
	${GTEST_INSTALL_DIR}/lib/libgmock.a
	${GTEST_INSTALL_DIR}/lib/libgmock_main.a
)

ExternalProject_Add(googletest_lib
    GIT_REPOSITORY ${GTEST_REPO}
    GIT_TAG v1.17.0
    PREFIX ${GTEST_DIR}
    BINARY_DIR ${GTEST_DIR}/build
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
				-DBUILD_GMOCK=ON 
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --parallel
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR>
)

add_library(googletestlib INTERFACE IMPORTED)
add_dependencies(googletestlib googletest_lib)
target_include_directories(googletestlib INTERFACE ${GTEST_INCLUDE_DIRS})
