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
	set(oneValueArgs WIDTH BASE SOURCE HEADER TARGET)
	cmake_parse_arguments(A "${options}" "${oneValueArgs}" "")

	if(defined A_BASE)
		set(base "${A_BASE}")
	else()
		set(base "${name}")
	endif()
	if(defined A_SOURCE)
		set(source "${A_SOURCE}")
	else()
		set(source "${base}")
	endif()
	if(defined A_HEADER)
		set(header "${A_HEADER}")
	else()
		# in CMAKE_CURRENT_BINARY_DIR though
		set(header "${source}.h")
	endif()

	get_filename_component(directory "${header}" DIRECTORY)
	file(MAKE_DIRECTORY "${directory}")
	
	add_custom_command(
		OUTPUT "${header}"
		COMMAND "name=${name}"
		$<IF:WIDTH,width="${WIDTH}",>
		$<IF:NOMACRO,nomacro=1,>
		$<IF:STATIC,static=1,>
		$<TARGET_FILE_NAME:data_to_header>
		< "${source}" > "$<TARGET_PROPERTY:LOCATION>"
		MAIN_DEPENDENCY "${source}"
		DEPENDS data_to_header)

	if(A_TARGET) 
		add_custom_target(
			"${A_TARGET}"
			DEPENDS "${output}")
	endif()
endfunction(data_to_header)

add_executable(make_specialescapes
	make_specialescapes.c)

set_source_files_properties(d2h_convert.c
	PROPERTIES
	OBJECT_DEPENDS	${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c)

