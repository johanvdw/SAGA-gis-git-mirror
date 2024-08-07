option(WITH_TOOLS_RIEGL "Check to create build targets for Riegl DB tools" OFF)

if(WITH_TOOLS_RIEGL)
	if(MSVC AND DEPS_FROM_ENVARS)
		if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "Win32")
			set(RIEGL_RDB $ENV{RIEGL_RDB_32})
		else()
			set(RIEGL_RDB $ENV{RIEGL_RDB})
		endif()
	endif()

	find_path   (RIEGL_RDB_INCLUDE NAMES c/riegl/rdb.h PATHS "${RIEGL_RDB}/interface")
	find_library(RIEGL_RDB_LIBRARY NAMES rdb rdblib    PATHS "${RIEGL_RDB}/library")
	mark_as_advanced(RIEGL_RDB_INCLUDE RIEGL_RDB_LIBRARY)
	include(FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(RDB REQUIRED_VARS RIEGL_RDB_LIBRARY RIEGL_RDB_INCLUDE)

	if(RDB_FOUND)
		project(io_riegl_rdb)
		include(../../CMakePluginTemplate.cmake)	# load common tool template
		include_directories(rapidjson/include)
		target_include_directories(${PROJECT_NAME} PRIVATE "${RIEGL_RDB_INCLUDE}/c")
		target_include_directories(${PROJECT_NAME} PRIVATE "${RIEGL_RDB_INCLUDE}/cpp")
		target_link_libraries(${PROJECT_NAME} ${RIEGL_RDB_LIBRARY})
	endif()
endif()
