# Resource archives (fonts.zip, shaders.zip, textures.zip, ...) built from loose folders instead of committed
# zips: resources/<name>/ of the engine, then of the app, packed under a capitalised top folder (fonts/ goes
# to Fonts/). An app file replaces the engine file at the same path.
set(IZ_ENGINE_RESOURCES ${CMAKE_CURRENT_LIST_DIR}/../resources CACHE INTERNAL "")
set(IZ_PACK_RESOURCES ${CMAKE_CURRENT_LIST_DIR}/PackResources.cmake CACHE INTERNAL "")

# iz_resource_archives(TARGET <target> SOURCE <app resources dir> DESTINATION <install dir>
#	NAMES <name>... [EXCLUDE <regex>...])
function(iz_resource_archives)
	cmake_parse_arguments(ARG "" "TARGET;SOURCE;DESTINATION" "NAMES;EXCLUDE" ${ARGN})

	foreach(NAME ${ARG_NAMES})
		set(DIRS ${IZ_ENGINE_RESOURCES}/${NAME} ${ARG_SOURCE}/${NAME})
		set(FILES)
		foreach(DIR ${DIRS})
			file(GLOB_RECURSE FOUND CONFIGURE_DEPENDS ${DIR}/*)
			list(APPEND FILES ${FOUND})
		endforeach()
		foreach(PATTERN ${ARG_EXCLUDE})
			list(FILTER FILES EXCLUDE REGEX ${PATTERN})
		endforeach()

		string(SUBSTRING ${NAME} 0 1 FIRST)
		string(SUBSTRING ${NAME} 1 -1 REST)
		string(TOUPPER ${FIRST} FIRST)
		set(STAGE ${CMAKE_BINARY_DIR}/resources/${NAME})
		set(OUTPUT ${CMAKE_BINARY_DIR}/resources/${NAME}.zip)
		file(MAKE_DIRECTORY ${STAGE})

		# Lists go to the script joined with | since a ; would split the argument.
		string(JOIN "|" DIR_ARG ${DIRS})
		string(JOIN "|" EXCLUDE_ARG ${ARG_EXCLUDE})
		add_custom_command(
			OUTPUT ${OUTPUT}
			COMMAND ${CMAKE_COMMAND} -DDIRS=${DIR_ARG} -DEXCLUDE=${EXCLUDE_ARG} -DTOP=${FIRST}${REST}
				-DOUTPUT=${OUTPUT} -P ${IZ_PACK_RESOURCES}
			WORKING_DIRECTORY ${STAGE}
			DEPENDS ${FILES} ${IZ_PACK_RESOURCES}
			COMMENT "Packing ${NAME}.zip"
			VERBATIM)

		# Shown in the IDE, never compiled by it (Visual Studio would run fxc on the .hlsl).
		set_source_files_properties(${FILES} PROPERTIES VS_TOOL_OVERRIDE None)
		add_custom_target(${ARG_TARGET}.${NAME} DEPENDS ${OUTPUT} SOURCES ${FILES})
		set_target_properties(${ARG_TARGET}.${NAME} PROPERTIES FOLDER Resources)
		add_dependencies(${ARG_TARGET} ${ARG_TARGET}.${NAME})
		install(FILES ${OUTPUT} DESTINATION ${ARG_DESTINATION})
	endforeach()
endfunction()
