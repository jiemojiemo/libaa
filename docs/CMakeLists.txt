find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_QUIET YES)
    set(DOXYGEN_WARNINGS NO)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
    set(DOXYGEN_EXCLUDE_PATTERNS
            tests/*
            lib/
            build/
            cmake/
            app/
            res/
            )
    set(DOXYGEN_HTML_DYNAMIC_MENUS NO)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ./README.md)

    doxygen_add_docs(
            libaa_docs_doxygen
            README.md
            src/
            include/
            docs/
            WORKING_DIRECTORY ${LIBAA_DIR}
    )

    message(STATUS "DOXYGEN_FOUND")
else ()
    message(STATUS "Doxygen not found!")
endif ()