#.rst:
# FindMkDocs
# -----------
#
# This module looks for Pandoc
#
#If you need to convert files from one markup format into another, pandoc is your swiss-army knife. 
#http://pandoc.org/
#
# This module defines the following variables:
#
# ::
#
#    PANDOC_EXECUTABLE     = The path to the MkDocs2Pandoc command.
#    PANDOC_FOUND          = Was MkDocs2Pandoc found or not?
#    PANDOC_VERSION	   = Version of the Pandoc

#
# Find Pandoc...
#

find_program(PANDOC_EXECUTABLE
  NAMES pandoc
  PATHS
    /usr/bin
  DOC "Pandoc document convertion tool (http://pandoc.org/)"
)

if(PANDOC_EXECUTABLE)
  execute_process(COMMAND ${PANDOC_EXECUTABLE} "--version"  
	OUTPUT_VARIABLE PANDOC_VERSION_OUTPUT OUTPUT_STRIP_TRAILING_WHITESPACE)
  string(REGEX MATCH "[0-9]+\.[0-9]*" PANDOC_VERSION ${PANDOC_VERSION_OUTPUT})
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pandoc DEFAULT_MSG PANDOC_EXECUTABLE PANDOC_VERSION)

mark_as_advanced(PANDOC_EXECUTABLE)

