cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(docs)


#  Common header
# ---------------

cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
		"Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
	FORCE)
	set(CMAKE_BUILD_TYPE_Strings "Debug;Release;RelWithDebInfo;MinSizeRel")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_BUILD_TYPE_Strings})
endif(NOT CMAKE_BUILD_TYPE)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

if ("${SUPERBUILD_ROOT}" STREQUAL "")
	get_filename_component(SUPERBUILD_ROOT ${CMAKE_SOURCE_DIR} DIRECTORY)
	message(STATUS "Configuring project ${CMAKE_PROJECT_NAME}. Setting superbuild root as ${SUPERBUILD_ROOT}")
else()
	message(STATUS "Configuring project ${CMAKE_PROJECT_NAME}. Got superbuild root as ${SUPERBUILD_ROOT}")
endif()

set (CMAKE_MODULE_PATH "${SUPERBUILD_ROOT}/cmake")
set (CMAKE_PREFIX_PATH "${SUPERBUILD_ROOT}/propoze-docs/cmake")

set_directory_properties(${SUPERBUILD_ROOT}/build CLEAN_NO_CUSTOM TRUE)
set_directory_properties(${SUPERBUILD_ROOT}/install CLEAN_NO_CUSTOM TRUE)

include(common)


#  Configuration options
# -----------------------



#set(DO_PDF 0 CACHE BOOL "Whether generate PDF documentation (requires mkdocs2pandoc + LaTeX)")
#set(DO_HTML 1 CACHE BOOL "Whether generate documentation in static HTML")
#set(DO_EBOOK 0 CACHE BOOL "Whether generate documentation in epub format (requires mkdocs2pandoc)")
#set(DO_LIVE 1 CACHE BOOL "Whether to start server that serves the documentation in http://localhost:8000")

#set(DOS "")
#if(DO_PDF)
#	list(APPEND DOS "DO_PDF")
#endif()
#if(DO_HTML)
#	list(APPEND DOS "DO_HTML")
#endif()
#if(DO_EBOOK)
#	list(APPEND DOS "DO_EBOOK")
#endif()
#if(DO_LIVE)
#	list(APPEND DOS "DO_LIVE")
#endif()


#  Target definitions and library references
# -------------------------------------------


include(mkdocs)
mkdocs_documentation(
	SOURCE mkdocs/mkdocs.yml.in
	TARGET_NAME propoze_docs
	DOCS_SUBDIR docs
	FORMAT pdf
)




#  Printing error report in case there are missing components
# -----------------------------------------------------------
missing_dependency(FINALIZE)
