cmake_minimum_required(VERSION 3.13)

add_custom_command(
	OUTPUT specialescapes.c
	COMMAND ./make_specialescapes > ${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c.temp
	COMMAND mv ${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c.temp ${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c
	DEPENDS make_specialescapes)

add_executable(pack
	main.c
	d2h_convert.c)

set(D2HS_LOCATION "${CMAKE_CURRENT_BINARY_DIR}" PARENT_SCOPE)

function(pack name filename)
	set(PACK "${D2HS_LOCATION}/pack")
	set(output "${CMAKE_CURRENT_BINARY_DIR}/${filename}.h")
	
	add_custom_command(
		OUTPUT "${output}"
		COMMAND mkdir -p `dirname "${output}" `
		COMMAND "name=${name}" "${PACK}" < "${filename}" > "${output}.temp"
		COMMAND mv "${output}.temp" "${output}"
		DEPENDS "${filename}" data_to_header_string/pack)
	add_custom_target(
		"PACK_${name}"
		DEPENDS "${output}")
	add_dependencies(modifications "PACK_${name}")
endfunction(pack)

add_executable(make_specialescapes
	make_specialescapes.c)

set_source_files_properties(d2h_convert.c
	PROPERTIES
	OBJECT_DEPENDS	${CMAKE_CURRENT_BINARY_DIR}/specialescapes.c)
