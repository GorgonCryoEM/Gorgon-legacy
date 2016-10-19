init()

include(cmake/Windows/init.cmake)
# --------------------------------------------------------------------
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Gorgon/src_py/gorgon.pyw.in
        ${CMAKE_BINARY_DIR}/bin/legacy/main.py
        )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
        ${CMAKE_BINARY_DIR}/setup_legacy.py
        )

file(WRITE ${CMAKE_BINARY_DIR}/bin/__init__.py)
# --------------------------------------------------------------------
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Gorgon/src_py/
        DESTINATION ${CMAKE_BINARY_DIR}/bin/legacy
        COMPONENT Legacy
        FILES_MATCHING
        PATTERN "*.py"
        )
# --------------------------------------------------------------------
