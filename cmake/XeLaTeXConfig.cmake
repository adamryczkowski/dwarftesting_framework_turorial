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
#    XELATEX_EXECUTABLE     = The path to the xelatex command.
#    XELATEX_FOUND          = Was XeLaTeX found or not?


find_program(XELATEX_EXECUTABLE
  NAMES xelatex
  PATHS
    /usr/bin
  DOC "XeLaTeX"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XeLaTeX DEFAULT_MSG XELATEX_EXECUTABLE)

mark_as_advanced(XELATEX_EXECUTABLE)

