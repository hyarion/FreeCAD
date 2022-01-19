# Try to find PySide2 utilities, PYSIDE6UIC and PYSIDE6RCC:
# PYSIDE6UICBINARY - Location of PYSIDE6UIC executable
# PYSIDE6RCCBINARY - Location of PYSIDE6RCC executable
# PYSIDE6_TOOLS_FOUND - PySide2 utilities found.

# Also provides macro similar to FindQt4.cmake's WRAP_UI and WRAP_RC,
# for the automatic generation of Python code from Qt4's user interface
# ('.ui') and resource ('.qrc') files. These macros are called:
# - PYSIDE_WRAP_UI
# - PYSIDE_WRAP_RC

IF(PYSIDE6UICBINARY AND PYSIDE6RCCBINARY)
  # Already in cache, be silent
  set(PYSIDE6_TOOLS_FOUND_QUIETLY TRUE)
ENDIF(PYSIDE6UICBINARY AND PYSIDE6RCCBINARY)

if(WIN32 OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #pyside6 tools are often in same location as python interpreter
    get_filename_component(PYTHON_BIN_DIR ${PYTHON_EXECUTABLE} PATH)
    set(PYSIDE_BIN_DIR ${PYTHON_BIN_DIR})
endif(WIN32 OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Since Qt v5.14, pyside6-uic and pyside6-rcc are directly provided by Qt6Core uic and rcc, with '-g python' option
# We test Qt6Core version to act accordingly

FIND_PACKAGE(Qt6Core)

IF(Qt6Core_VERSION VERSION_LESS 5.14)
  # Legacy (< 5.14)
  FIND_PROGRAM(PYSIDE6UICBINARY NAMES python2-pyside6-uic pyside6-uic pyside6-uic-${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} pyuic5 HINTS ${PYSIDE_BIN_DIR})
  FIND_PROGRAM(PYSIDE6RCCBINARY NAMES pyside6-rcc pyside6-rcc-${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} pyrcc5 HINTS ${PYSIDE_BIN_DIR})
  set(UICOPTIONS "")
  set(RCCOPTIONS "")
ELSE(Qt6Core_VERSION VERSION_LESS 5.14)
  # New (>= 5.14)
  FIND_PROGRAM(PYSIDE6UICBINARY NAMES uic-qt5 uic pyside6-uic)
  set(UICOPTIONS "--generator=python")
  FIND_PROGRAM(PYSIDE6RCCBINARY NAMES rcc-qt5 rcc pyside6-rcc)
  set(RCCOPTIONS "--generator=python" "--compress-algo=zlib" "--compress=1")
ENDIF(Qt6Core_VERSION VERSION_LESS 5.14)

MACRO(PYSIDE_WRAP_UI outfiles)
  FOREACH(it ${ARGN})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.py)
    #ADD_CUSTOM_TARGET(${it} ALL
    #  DEPENDS ${outfile}
    #)
    if(WIN32 OR APPLE)
        ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
          COMMAND ${PYSIDE6UICBINARY} ${UICOPTIONS} ${infile} -o ${outfile}
          MAIN_DEPENDENCY ${infile}
        )
    else()
        # Especially on Open Build Service we don't want changing date like
        # pyside6-uic generates in comments at beginning., which is why
        # we follow the tool command with a POSIX-friendly sed.
        ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
          COMMAND "${PYSIDE6UICBINARY}" ${UICOPTIONS} "${infile}" -o "${outfile}"
          COMMAND sed "/^# /d" "${outfile}" >"${outfile}.tmp" && mv "${outfile}.tmp" "${outfile}"
          MAIN_DEPENDENCY "${infile}"
        )
    endif()
    list(APPEND ${outfiles} ${outfile})
  ENDFOREACH(it)
ENDMACRO (PYSIDE_WRAP_UI)

MACRO(PYSIDE_WRAP_RC outfiles)
  FOREACH(it ${ARGN})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile "${CMAKE_CURRENT_BINARY_DIR}/${outfile}_rc.py")
    #ADD_CUSTOM_TARGET(${it} ALL
    #  DEPENDS ${outfile}
    #)
    if(WIN32 OR APPLE)
        ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
          COMMAND ${PYSIDE6RCCBINARY} ${RCCOPTIONS} ${infile} -o ${outfile}
          MAIN_DEPENDENCY ${infile}
        )
    else()
        # Especially on Open Build Service we don't want changing date like
        # pyside-rcc generates in comments at beginning, which is why
        # we follow the tool command with in-place sed.
        ADD_CUSTOM_COMMAND(OUTPUT "${outfile}"
          COMMAND "${PYSIDE6RCCBINARY}" ${RCCOPTIONS} "${infile}" ${PY_ATTRIBUTE} -o "${outfile}"
          COMMAND sed "/^# /d" "${outfile}" >"${outfile}.tmp" && mv "${outfile}.tmp" "${outfile}"
          MAIN_DEPENDENCY "${infile}"
        )
    endif()
    list(APPEND ${outfiles} ${outfile})
  ENDFOREACH(it)
ENDMACRO (PYSIDE_WRAP_RC)

IF(EXISTS ${PYSIDE6UICBINARY} AND EXISTS ${PYSIDE6RCCBINARY})
   set(PYSIDE6_TOOLS_FOUND TRUE)
ENDIF(EXISTS ${PYSIDE6UICBINARY} AND EXISTS ${PYSIDE6RCCBINARY})

if(PYSIDE6RCCBINARY AND PYSIDE6UICBINARY)
    if (NOT PySide2Tools_FIND_QUIETLY)
        message(STATUS "Found PySide2 tools: ${PYSIDE6UICBINARY}, ${PYSIDE6RCCBINARY}")
    endif (NOT PySide2Tools_FIND_QUIETLY)
else(PYSIDE6RCCBINARY AND PYSIDE6UICBINARY)
    if(PySide2Tools_FIND_REQUIRED)
        message(FATAL_ERROR "PySide2 tools could not be found, but are required.")
    else(PySide2Tools_FIND_REQUIRED)
        if (NOT PySide2Tools_FIND_QUIETLY)
                message(STATUS "PySide2 tools: not found.")
        endif (NOT PySide2Tools_FIND_QUIETLY)
    endif(PySide2Tools_FIND_REQUIRED)
endif(PYSIDE6RCCBINARY AND PYSIDE6UICBINARY)
