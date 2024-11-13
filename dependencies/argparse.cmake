set(ARGPARSE_DIR ${CMAKE_BINARY_DIR}/argparse)
set(ARGPARSE_INCLUDE_DIR ${ARGPARSE_DIR}/include)
set(ARGPARSE_REPO https://github.com/p-ranav/argparse.git)

add_custom_command(OUTPUT ${ARGPARSE_DIR}
	COMMAND git clone ${ARGPARSE_REPO}
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

get_build_type(build_type)

add_custom_command(OUTPUT ${ARGPARSE_INCLUDE_DIR}
	COMMAND cmake -S . -B ${build_type} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
	COMMAND cmake --build ${build_type} --parallel
	COMMAND cmake --install ${build_type}
	DEPENDS ${ARGPARSE_DIR}
	WORKING_DIRECTORY ${ARGPARSE_DIR}
)

add_custom_target(argparse_lib 
	DEPENDS ${ARGPARSE_DIR}
	DEPENDS ${ARGPARSE_INCLUDE_DIR}
)
