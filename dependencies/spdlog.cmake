# FindSpdlog.cmake - CMake module for spdlog using ExternalProject_Add
# Usage: include(FindSpdlog) in your CMakeLists.txt

include(ExternalProject)

# Set default installation directory
if(NOT SPDLOG_INSTALL_DIR)
    set(SPDLOG_INSTALL_DIR ${CMAKE_BINARY_DIR}/libs)
endif()

# Configure spdlog external project
ExternalProject_Add(spdloglib
    GIT_REPOSITORY   https://github.com/gabime/spdlog.git
    GIT_TAG          v1.15.3
    PREFIX           ${CMAKE_BINARY_DIR}/spdlog
    BINARY_DIR       ${CMAKE_BINARY_DIR}/spdlog/build
    INSTALL_DIR      ${SPDLOG_INSTALL_DIR}
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
        -DSPDLOG_BUILD_SHARED=OFF          # Static library by default
        -DSPDLOG_BUILD_EXAMPLE=OFF         # Don't build examples
        -DSPDLOG_BUILD_TESTS=OFF           # Don't build tests
        -DSPDLOG_BUILD_BENCH=OFF           # Don't build benchmarks
        -DSPDLOG_FMT_EXTERNAL=OFF          # Use bundled fmt library
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --parallel
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR>
)

# Create imported target after external project is built
ExternalProject_Get_property(spdloglib INSTALL_DIR)

# Set up variables for the imported target
set(SPDLOG_INCLUDE_DIR ${INSTALL_DIR}/include)
set(SPDLOG_LIBRARY_DIR ${INSTALL_DIR}/lib)
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(SPDLOG_LIBRARY ${SPDLOG_LIBRARY_DIR}/libspdlogd.a)
elseif(CMAKE_BUILD_TYPE MATCHES RELEASE)
    set(SPDLOG_LIBRARY ${SPDLOG_LIBRARY_DIR}/libspdlog.a)
endif()
