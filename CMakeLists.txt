cmake_minimum_required(VERSION 3.20)
project(lispkit C)

if(APPLE)
	add_compile_options(-fno-signed-char)
elseif(UNIX)
	add_compile_options(-fno-signed-char)
endif()

add_executable(lispkit
	src/cell.c
	src/main.c
	src/symbol.c
)

if(UNIX)
	set_target_properties(lispkit PROPERTIES COMPILE_DEFINITIONS _UNIX_)
endif()

target_compile_features(lispkit PUBLIC c_std_99)

