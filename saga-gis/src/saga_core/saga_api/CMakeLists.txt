project(saga_api)

message(STATUS "project: ${PROJECT_NAME}")

# define headers
set(SAGA_API_PUBLIC_HEADERS
	api_core.h
	data_manager.h
	dataobject.h
	datetime.h
	geo_tools.h
	grid.h
	grids.h
	grid_pyramid.h
	mat_tools.h
	metadata.h
	parameters.h
	pointcloud.h
	saga_api.h
	shapes.h
	table_dbase.h
	table.h
	table_value.h
	tin.h
	tool.h
	tool_library.h
)

# define sources
set(SAGA_API_SOURCES
	api_callback.cpp
	api_colors.cpp
	api_core.cpp
	api_file.cpp
	api_memory.cpp
	api_string.cpp
	api_translator.cpp
	data_manager.cpp
	dataobject.cpp
	datetime.cpp
	geo_classes.cpp
	geo_functions.cpp
	grid.cpp
	grid_io.cpp
	grid_memory.cpp
	grid_operation.cpp
	grid_pyramid.cpp
	grid_system.cpp
	grids.cpp
	kdtree.cpp
	mat_formula.cpp
	mat_grid_radius.cpp
	mat_indexing.cpp
	mat_matrix.cpp
	mat_regression.cpp
	mat_regression_multiple.cpp
	mat_regression_weighted.cpp
	mat_spline.cpp
	mat_tools.cpp
	mat_trend.cpp
	metadata.cpp
	parameter.cpp
	parameter_data.cpp
	parameters.cpp
	pointcloud.cpp
	projections.cpp
	quadtree.cpp
	saga_api.cpp
	shape.cpp
	shape_line.cpp
	shape_part.cpp
	shape_point.cpp
	shape_points.cpp
	shape_polygon.cpp
	shapes.cpp
	shapes_io.cpp
	shapes_ogis.cpp
	shapes_selection.cpp
	table.cpp
	table_dbase.cpp
	table_io.cpp
	table_record.cpp
	table_selection.cpp
	tin.cpp
	tin_elements.cpp
	tin_triangulation.cpp
	tool.cpp
	tool_chain.cpp
	tool_grid.cpp
	tool_grid_interactive.cpp
	tool_interactive.cpp
	tool_interactive_base.cpp
	tool_library.cpp
	tool_library_interface.cpp
	tool_summary.cpp
	api_core.h
	data_manager.h
	dataobject.h
	datetime.h
	geo_tools.h
	grid.h
	grid_pyramid.h
	mat_tools.h
	metadata.h
	parameters.h
	pointcloud.h
	saga_api.h
	shapes.h
	table.h
	table_dbase.h
	table_value.h
	tin.h
	tool.h
	tool_chain.h
	tool_library.h
	nanoflann/nanoflann.hpp
)

#________________________________________
option(WITH_CLIPPER_ONE "Check to make SAGA API use older Clipper1 instead of newer Clipper2 (which expects at least C++ 17)" OFF)

if(WITH_CLIPPER_ONE)
	set(SAGA_API_SOURCES
		${SAGA_API_SOURCES}
		clipper1/clipper.cpp
		clipper1/clipper.hpp
		shapes_polygons.cpp
	)
else()
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

	set(SAGA_API_SOURCES
		${SAGA_API_SOURCES}
		clipper2/clipper.engine.cpp
		clipper2/clipper.offset.cpp
		clipper2/clipper.rectclip.cpp
		clipper2/clipper.h
		clipper2/clipper.core.h
		clipper2/clipper.engine.h
		clipper2/clipper.minkowski.h
		clipper2/clipper.offset.h
		clipper2/clipper.rectclip.h
		clipper2/clipper.version.h
		shapes_clipper.cpp
	)
endif()

#________________________________________
option(WITH_MRMR "Check to include mRMR code (Minimum Redundancy Feature Selection, incompatible with Debian licensing)" OFF)

if(WITH_MRMR)
	set(SAGA_API_SOURCES
		${SAGA_API_SOURCES}
		mat_mRMR.cpp
	)
endif()

#________________________________________
add_library(saga_api SHARED
	${SAGA_API_SOURCES}
	${SAGA_API_PUBLIC_HEADERS} # just cosmetic, show headers in IDE
)

target_include_directories(saga_api INTERFACE "${saga_core_include_dir}")

if(NOT WITH_CLIPPER_ONE)
	target_include_directories(saga_api PRIVATE "${saga_core_include_dir}/saga_api")
else()
	target_compile_definitions(saga_api PUBLIC -DCLIPPER_ONE)
endif()

find_package(wxWidgets COMPONENTS base net xml html REQUIRED)
target_link_libraries(saga_api PRIVATE ${wxWidgets_LIBRARIES})

if(MSVC) # windows msvc
	target_compile_definitions(saga_api PUBLIC -D_SAGA_MSW -DUNICODE)
else() # unix like systems
	target_compile_definitions(saga_api PUBLIC -D_SAGA_LINUX)
	target_compile_definitions(saga_api PRIVATE -D${MTOOLSPATH} -D${MSHAREPATH})
	set_target_properties(saga_api PROPERTIES COMPILE_FLAGS -fPIC)
endif()

target_compile_definitions(saga_api PUBLIC -D_TYPEDEF_BYTE -D_TYPEDEF_WORD)
target_compile_definitions(saga_api PRIVATE -D_SAGA_API_EXPORTS)

if(WITH_MRMR)
	target_compile_definitions(saga_api PUBLIC -DWITH_MRMR)
endif()

option(WITH_LIFETIME_TRACKER "Check to build with CSG_Data_Object::Track() functionality (data object lifetime tracker)" ON)
if(WITH_LIFETIME_TRACKER)
	target_compile_definitions(saga_api PUBLIC -DWITH_LIFETIME_TRACKER)
endif()

# set version number for library
set_target_properties(saga_api PROPERTIES
	VERSION ${SAGA_VERSION_STRING}
	SOVERSION ${SAGA_VERSION_MAJOR}
)

# where to install the library
install(TARGETS saga_api DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${SAGA_API_PUBLIC_HEADERS} DESTINATION include/saga/saga_api)

if(MSVC)
	string(REPLACE "/" "\\" SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")
	add_custom_command(TARGET saga_api POST_BUILD
		COMMAND IF NOT EXIST "$(OutDir)include" MKDIR "$(OutDir)include"
		COMMAND IF EXIST "$(OutDir)include\\saga_api" RMDIR /S/Q "$(OutDir)include\\saga_api"
		COMMAND MKDIR    "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}api_core.h"     "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}data_manager.h" "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}dataobject.h"   "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}datetime.h"     "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}geo_tools.h"    "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}grid.h"         "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}grid_pyramid.h" "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}grids.h"        "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}mat_tools.h"    "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}metadata.h"     "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}parameters.h"   "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}pointcloud.h"   "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}saga_api.h"     "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}shapes.h"       "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}table.h"        "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}table_value.h"  "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}tin.h"          "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}tool.h"         "$(OutDir)include\\saga_api"
		COMMAND COPY "${SOURCE_DIR}tool_library.h" "$(OutDir)include\\saga_api"
	)
endif()

#-#-#-#-#-# SWIG / Python #-#-#-#-#-#
option(WITH_PYTHON "Uncheck to not try to Build the SAGA API for Python." ON)
if(WITH_PYTHON)
	add_subdirectory(saga_api_python)
endif()
