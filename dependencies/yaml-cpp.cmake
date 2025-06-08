include(ExternalProject)

set(YAML_DIR ${CMAKE_BINARY_DIR}/yaml-cpp)
set(YAML_INSTALL_DIR ${CMAKE_BINARY_DIR}/libs/yaml-cpp)
set(YAML_INCLUDE_DIR ${YAML_INSTALL_DIR}/include)
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(YAML_LIB ${YAML_INSTALL_DIR}/lib/libyaml-cppd.a)
elseif(CMAKE_BUILD_TYPE MATCHES RELEASE)
    set(YAML_LIB ${YAML_INSTALL_DIR}/lib/libyaml-cpp.a)
endif()
set(YAML_REPO https://github.com/jbeder/yaml-cpp.git)

ExternalProject_Add(yamlcpplib
    GIT_REPOSITORY ${YAML_REPO}
    GIT_TAG master
    PREFIX ${YAML_DIR}
    BINARY_DIR ${YAML_DIR}/build
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${YAML_INSTALL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
				-DYAML_BUILD_SHARED_LIBS=OFF 
				-DYAML_CPP_BUILD_TESTS=OFF
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --parallel
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR>
)

add_library(yaml_cpp INTERFACE IMPORTED)
add_dependencies(yaml_cpp yamlcpplib)
target_include_directories(yaml_cpp INTERFACE ${YAML_INCLUDE_DIR})
