find_package(yaml-cpp)
if(DEFINED YAML_CPP_INCLUDE_DIR AND DEFINED YAML_CPP_LIBRARIES)
	set(YAML_INCLUDE_DIR ${YAML_CPP_INCLUDE_DIR})
	set(YAML_SHARED_LIB ${YAML_CPP_LIBRARIES})
	add_library(yaml_cpp ALIAS yaml-cpp)
	return()
endif()


set(YAML_DIR ${CMAKE_BINARY_DIR}/yaml-cpp)
set(YAML_INCLUDE_DIR ${YAML_DIR}/include)
set(YAML_SHARED_LIB ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cpp.so)
set(YAML_REPO https://github.com/jbeder/yaml-cpp.git)

get_build_type(build_type)

add_custom_command(OUTPUT ${YAML_DIR}
	COMMAND git clone ${YAML_REPO}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_command(OUTPUT ${YAML_SHARED_LIB}
	COMMAND cmake -S . -B ${build_type} -DYAML_BUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
	COMMAND cmake --build ${build_type} --parallel
	COMMAND cmake --install ${build_type}
	DEPENDS ${YAML_DIR}
	WORKING_DIRECTORY ${YAML_DIR}
)

add_custom_target(yaml_cpp 
	DEPENDS ${YAML_DIR}
	DEPENDS ${YAML_SHARED_LIB}
)
