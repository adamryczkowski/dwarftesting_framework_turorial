1. Create CMakeFile that creates pdf:
 `mkdocs2pandoc > docs.pd`
 `pandoc --toc -f markdown+grid_tables+table_captions -o docs.pdf docs.pd `
or ebook:
 `pandoc --toc -f markdown+grid_tables -t epub -o docs.epub docs.pd`    

2. Create CMake that launches html version of the site
 `mkdocs serve`

3. Create CMake that launches html version ofline
 `mkdocs serve`
