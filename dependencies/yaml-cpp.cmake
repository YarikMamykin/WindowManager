set(YAML_DIR ${CMAKE_BINARY_DIR}/yaml-cpp)
set(YAML_INCLUDE_DIR ${YAML_DIR}/include)
set(YAML_LIB ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cpp.a)
set(YAML_REPO https://github.com/jbeder/yaml-cpp.git)

add_custom_command(OUTPUT ${YAML_DIR}
	COMMAND git clone ${YAML_REPO}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_command(OUTPUT ${YAML_LIB}
	COMMAND cmake -S . -B ${CMAKE_BUILD_TYPE} -DYAML_BUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
	COMMAND cmake --build ${CMAKE_BUILD_TYPE} --parallel
	COMMAND cmake --install ${CMAKE_BUILD_TYPE}
	DEPENDS ${YAML_DIR}
	WORKING_DIRECTORY ${YAML_DIR}
)

add_custom_target(yaml_cpp 
	DEPENDS ${YAML_DIR}
	DEPENDS ${YAML_LIB}
)
