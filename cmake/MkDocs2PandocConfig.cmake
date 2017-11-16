#.rst:
# FindMkDocs
# -----------
#
# This module looks for MkDocs2Pandoc
#
#This module contains a set of filters for converting mkdocs style markdown documentation into a single pandoc(1) flavoured markdown document. This is useful for
#
#    Generating PDF or EPUB from your mkdocs documentation
#    Generating single-page HTML from your mkdocs documentation
#    Converting your mkdocs documentation to other formats, such as asciidoc.
#
#Aside from the filters the module contains a converter class tying them together into a coherent whole and the command line converter mkdocs2pandoc.
#
# This module defines the following variables:
#
# ::
#
#    MKDOCS2PANDOC_EXECUTABLE     = The path to the MkDocs2Pandoc command.
#    MKDOCS2PANDOC_FOUND          = Was MkDocs2Pandoc found or not?
#

#
# Find MkDocs2PanDoc...
#

find_program(MKDOCS2PANDOC_EXECUTABLE
  NAMES mkdocs2pandoc
  PATHS
    /usr/local/bin
    $ENV{HOME}/.local/bin
  DOC "MkDocs2Pandoc documentation generation tool (https://github.com/jgrassler/mkdocs-pandoc)"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MkDocs2Pandoc DEFAULT_MSG MKDOCS2PANDOC_EXECUTABLE)

#if(NOT MkDocs2Pandoc_FOUND)
#	message(FATAL_ERROR "No mkdocs2pandoc found. Please install it using\n\n       sudo -H pip3 install mkdocs-pandoc\n\n")
#endif()

mark_as_advanced(MKDOCS2PANDOC_EXECUTABLE)
