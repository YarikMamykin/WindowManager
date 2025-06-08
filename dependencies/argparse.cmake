include(ExternalProject)

set(ARGPARSE_DIR ${CMAKE_BINARY_DIR}/argparse)
set(ARGPARSE_INSTALL_DIR ${CMAKE_BINARY_DIR}/libs/argparse)
set(ARGPARSE_REPO https://github.com/p-ranav/argparse.git)

ExternalProject_Add(argparse_lib
    GIT_REPOSITORY ${ARGPARSE_REPO}
    GIT_TAG master
    PREFIX ${ARGPARSE_DIR}
    BINARY_DIR ${ARGPARSE_DIR}/build
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${ARGPARSE_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DARGPARSE_BUILD_SAMPLES=OFF
        -DARGPARSE_BUILD_TESTS=OFF
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --parallel
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR>
)

# Get the install directory for headers
set(ARGPARSE_INCLUDE_DIR ${ARGPARSE_INSTALL_DIR}/include)

# Create an imported target for easier linking
add_library(argparse INTERFACE IMPORTED)
add_dependencies(argparse argparse_lib)
target_include_directories(argparse INTERFACE ${ARGPARSE_INCLUDE_DIR})
