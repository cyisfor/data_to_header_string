cmake_minimum_required(VERSION 3.13)

add_custom_command(
	OUTPUT specialescapes.c
	COMMAND ./make_specialescapes > ${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c.temp
	COMMAND mv ${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c.temp ${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c
	DEPENDS make_specialescapes)

add_executable(data_to_header
	main.c
	d2h_convert.c)

function(data_to_header name)
	# TODO: example, docs
	set(options NOMACRO STATIC)
	set(oneValueArgs WIDTH BASE SOURCE TARGET HEADER)
	cmake_parse_arguments(PARSE_ARGV 1 A "${options}" "${oneValueArgs}" "")

	if(DEFINED A_BASE)
		set(base "${A_BASE}")
	else()
		set(base "${name}")
	endif()
	
	if(DEFINED A_SOURCE)
		set(source "${A_SOURCE}")
	else()
		set(source "${base}")
	endif()
	if(DEFINED A_HEADER)
		set(header "${A_HEADER}")
	else()
		# in CMAKE_CURRENT_BINARY_DIR though
		set(header "${source}.h")
	endif()
	get_filename_component(source "${source}" ABSOLUTE)

	get_filename_component(directory "${header}" DIRECTORY)
	file(MAKE_DIRECTORY "${directory}")

	if(DEFINED A_WIDTH)
		list(APPEND env "width=${A_WIDTH}")
	endif()
	if(A_NOMACRO)
		list(APPEND env "nomacro=1")
	endif()
	if(A_STATIC)
		list(APPEND env "static=1")
	endif()
	
	add_custom_command(
		OUTPUT "${header}"
		COMMAND
		"name=${name}"
		${env}
		$<TARGET_FILE:data_to_header>
		< "${source}" > "${header}"
		MAIN_DEPENDENCY "${source}"
		DEPENDS data_to_header)

	if(DEFINED A_TARGET)
		add_custom_target(
			"${A_TARGET}"
			DEPENDS "${header}")
	endif()
endfunction(data_to_header)

add_executable(make_specialescapes
	make_specialescapes.c)

set_source_files_properties(d2h_convert.c
	PROPERTIES
	OBJECT_DEPENDS	${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c)

add_executable(example example.c)

data_to_header(EXAMPLE_ONE_YAY
	HEADER first_example.h
	SOURCE example1.txt
	TARGET EX1)
data_to_header(second_example
	BASE example2.txt
	WIDTH 5
	STATIC
	NOMACRO
	TARGET D2HEx2)

data_to_header(example_tres
	BASE example2.txt
	HEADER example_tres.h
	STATIC
	NOMACRO
	TARGET DERP3)

add_dependencies(example EX1 D2HEx2 DERP3)
target_include_directories(example PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
