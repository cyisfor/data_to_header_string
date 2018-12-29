cmake_minimum_required(VERSION 3.13)

add_custom_command(
	OUTPUT specialescapes.c
	COMMAND ./make_specialescapes > ${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c.temp
	COMMAND mv ${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c.temp ${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c
	DEPENDS make_specialesapes)

add_executable(pack
	main.c
	d2hconvert.c)
