cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(docs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH cmake)
find_package(MkDocs REQUIRED)


# message(STATUS "${MKDOCS_EXECUTABLE} ${MKDOCS_VERSION}")
message(STATUS "ENGINE NAME ${ENGINE_NAME}")

set(PROJECT_NAME "Propoze documentation")
set(AUTHOR "Adam Ryczkowski")
set(COPYRIGHT ${ENGINE_COPYRIGHT})
set(DOCS_DIR ${PROJECT_SOURCE_DIR}/mkdocs)
set(USE_DIRECTORY_URLS FALSE)
set(PROJECT_VERSION ${ENGINE_VERSION_FULL})
configure_file("${DOCS_DIR}/mkdocs.yml.in" "${PROJECT_BINARY_DIR}/mkdocs.yml")

set(DO_PDF 0 CACHE BOOL "Whether generate PDF documentation (requires mkdocs2pandoc + LaTeX)")
set(DO_HTML 1 CACHE BOOL "Whether generate documentation in static HTML")
set(DO_EBOOK 0 CACHE BOOL "Whether generate documentation in epub format (requires mkdocs2pandoc)")
set(DO_LIVE 0 CACHE BOOL "Whether to start server that serves the documentation in http://localhost:8000")
set(SITE_DIR ${PROJECT_BINARY_DIR}/${DOCUMENTATION_FORMAT})

set(PD_DIR ${PROJECT_BINARY_DIR})

if (${DO_PDF})
	find_package(MkDocs2Pandoc REQUIRED)
	list(APPEND DOCUMENTATION_FORMAT "pdf")
	set(PDF_DIR ${PROJECT_BINARY_DIR}/pdf)
#	file(MAKE_DIRECTORY ${PDF_DIR})
	message("Made ${PDF_DIR}")
#	add_dependencies(ALL docs_pdf)
	add_custom_target(docs_pdf ALL
		DEPENDS ${PDF_DIR}/${PROJECT_NAME}.pdf
	)
	add_custom_command(TARGET docs_pdf POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Documentation can be found in file://${PDF_DIR}/${PROJECT_NAME}.pdf" )

endif()

if (${DO_HTML})
	list(APPEND DOCUMENTATION_FORMAT "html")
	set(HTML_DIR ${PROJECT_BINARY_DIR}/site)
	file(MAKE_DIRECTORY ${HTML_DIR})
	message("Made ${HTML_DIR}")
#	add_dependencies(ALL docs_html)
	add_custom_target(docs_html ALL 
		DEPENDS ${HTML_DIR}/index.html
	)
	add_custom_command(TARGET docs_html POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "You can open the documentation in your browser on file://${HTML_DIR}/index.html")
endif()

if (${DO_EBOOK})
	find_package(MkDocs2Pandoc REQUIRED)
	list(APPEND DOCUMENTATION_FORMAT "ebook")
	set(EBOOK_DIR ${PROJECT_BINARY_DIR}/ebook)
#	file(MAKE_DIRECTORY ${EBOOK_DIR})
	message("Made ${EBOOK_DIR}")
#	add_dependencies(ALL docs_ebook)
	add_custom_target(docs_ebook ALL
		DEPENDS ${EBOOK_DIR}/${PROJECT_NAME}.epub
	)
	add_custom_command(TARGET docs_ebook PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "${EBOOK_DIR}")
	add_custom_command(TARGET docs_ebook POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Documentation can be found in file://${PDF_EBOOK}/${PROJECT_NAME}.epub")
endif()

if (${DO_LIVE})
	list(APPEND DOCUMENTATION_FORMAT "live")
	# Commands that prepare DOCS_SITE target (live web server)

	add_custom_target(docs_site ALL "${MKDOCS_EXECUTABLE} serve &"
			  VERBATIM
			  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
			  COMMENT "Generating starting to serve the documentation." VERBATIM)

	add_custom_command(TARGET docs_site POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "You can open the documentation in your browser on http://localhost:8000")

endif()

# Commands that prepare DOCS_HTML target (static HTML)


add_custom_command(OUTPUT ${HTML_DIR}/index.html 
	COMMAND ${MKDOCS_EXECUTABLE} build
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	COMMENT "Generating HTML documentation" VERBATIM)





# Commands that prepare DOCS_PDF target


add_custom_command(OUTPUT "${PD_DIR}" PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PD_DIR})
add_custom_command(OUTPUT ${PD_DIR}/${PROJECT_NAME}.pd 
	COMMAND ${MKDOCS2PANDOC_EXECUTABLE} > "${PD_DIR}/${PROJECT_NAME}.pd"
	VERBATIM
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	COMMENT "Generating intermediate documentation in Pandoc format" VERBATIM
)

add_custom_command(OUTPUT "${PDF_DIR}/${PROJECT_NAME}.pdf"
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PDF_DIR}
	COMMAND ${PANDOC_EXECUTABLE} --toc -f markdown+grid_tables+table_captions -o ${PDF_DIR}/${PROJECT_NAME}.pdf ${PD_DIR}/${PROJECT_NAME}.pd
	DEPENDS ${PD_DIR}/${PROJECT_NAME}.pd
#	WORKING_DIRECTORY ${PDF_DIR}
	COMMENT "Generating PDF documentation" 
	VERBATIM
)





# Commands that prepare DOCS_EBOOK target (epub ebook)

add_custom_command(OUTPUT "${EBOOK_DIR}/${PROJECT_NAME}.epub"
	COMMAND ${CMAKE_COMMAND} -E make_directory ${EBOOK_DIR}
	COMMAND ${PANDOC_EXECUTABLE} --toc -f markdown+grid_tables -t epub -o ${EBOOK_DIR}/${PROJECT_NAME}.epub ${PD_DIR}/${PROJECT_NAME}.pd
	DEPENDS ${PD_DIR}/${PROJECT_NAME}.pd
#	WORKING_DIRECTORY ${EBOOK_DIR}
	COMMENT "Generating epub documentation" VERBATIM
)






