include(ExternalProject)

set(LIBSOCKET_INSTALL_DIR ${CMAKE_BINARY_DIR}/libs/libsocket)
set(LIBSOCKET_DIR ${CMAKE_BINARY_DIR}/libsocket)
set(LIBSOCKET_LIB ${LIBSOCKET_INSTALL_DIR}/lib/libsocket++.so)
set(LIBSOCKET_REPO https://github.com/dermesser/libsocket.git)


ExternalProject_Add(libsocket_lib
    GIT_REPOSITORY ${LIBSOCKET_REPO}
    GIT_TAG master
    PREFIX ${LIBSOCKET_DIR}
    BINARY_DIR ${LIBSOCKET_DIR}/build
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${LIBSOCKET_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --parallel
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR>
)

set(LIBSOCKET_INCLUDE_DIR ${LIBSOCKET_INSTALL_DIR}/include)

add_library(libsocket INTERFACE IMPORTED)
add_dependencies(libsocket libsocket_lib)
target_include_directories(libsocket INTERFACE ${LIBSOCKET_INCLUDE_DIR})
