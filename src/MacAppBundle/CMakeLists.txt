set(PYTHON_DIR_BASENAME python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR})

if(PYTHON_LIBRARY MATCHES "(.*Python\\.framework).*")
    #framework
    set(PYTHON_DIR ${CMAKE_MATCH_1}/Versions/Current/lib/${PYTHON_DIR_BASENAME})
else()
    #unix
    get_filename_component(PYTHON_DIR ${PYTHON_LIBRARY} PATH)
    set(PYTHON_DIR ${PYTHON_DIR}/${PYTHON_DIR_BASENAME})
endif()

set(WEBKIT_FRAMEWORK_DIR "")

install(CODE "execute_process(COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${PYTHON_DIR} ${CMAKE_INSTALL_LIBDIR}/${PYTHON_DIR_BASENAME}
)")

if(HOMEBREW_PREFIX)
    #Homebrew installs python dependencies to a site dir in prefix/libexec
    #and installs a .pth file containing its path to the HOMEBREW_PREFIX site dir.

    file(GLOB HOMEBREW_PTH_FILES "${PYTHON_DIR}/site-packages/homebrew*.pth")

    foreach(PTH_FILE ${HOMEBREW_PTH_FILES})
        file(READ ${PTH_FILE} ADDITIONAL_DIR)

	string(STRIP "${ADDITIONAL_DIR}" ADDITIONAL_DIR)
        string(FIND "${ADDITIONAL_DIR}" "${HOMEBREW_PREFIX}/Cellar" POSITION)
        string(LENGTH "${ADDITIONAL_DIR}" DIR_LENGTH)
        string(SUBSTRING "${ADDITIONAL_DIR}" ${POSITION} ${DIR_LENGTH}-${POSITION} DIR_TAIL)
        string(REGEX MATCHALL "^([/A-Za-z0-9_.@-]+)" CLEAR_TAIL ${DIR_TAIL})
        string(REGEX REPLACE "^${HOMEBREW_PREFIX}/Cellar/([A-Za-z0-9_]+).*$" "\\1" LIB_NAME ${CLEAR_TAIL})
        string(REGEX REPLACE ".*libexec(.*)/site-packages" "libexec/${LIB_NAME}\\1" NEW_SITE_DIR ${CLEAR_TAIL})

        install(DIRECTORY ${CLEAR_TAIL} DESTINATION ${CMAKE_INSTALL_PREFIX}/${NEW_SITE_DIR})

        #update the paths of the .pth files copied into the bundle
        get_filename_component(PTH_FILENAME ${PTH_FILE} NAME)
        install(CODE
            "file(WRITE
                 ${CMAKE_INSTALL_LIBDIR}/${PYTHON_DIR_BASENAME}/site-packages/${PTH_FILENAME}
		 \"../../../${NEW_SITE_DIR}/site-packages\"
            )"
        )
    endforeach(PTH_FILE)
endif()

if(Qt6_FOUND)
  set(QtCore_DIR ${Qt6Core_DIR})
else()
  set(QtCore_DIR ${Qt5Core_DIR})
end()
set(QT_PLUGINS_DIR "${QtCore_DIR}/../../../plugins")
execute_process(COMMAND "xcode-select" "--print-path"
                OUTPUT_VARIABLE XCODE_PATH
                ERROR_QUIET
      	 )
string(STRIP ${XCODE_PATH} XCODE_PATH)

set(XCTEST_PATH "${XCODE_PATH}/Platforms/MacOSX.platform/Developer/Library/Frameworks/XCTest.framework/Versions/Current")

# add qt assistant to bundle
install(PROGRAMS "${QtCore_DIR}/../../../libexec/Assistant.app/Contents/MacOS/Assistant" DESTINATION ${CMAKE_INSTALL_PREFIX}/MacOS)

  # add QtWebEngineProcess to bundle
  if(BUILD_WEB)
  install(PROGRAMS "${QtCore_DIR}/../../../lib/QtWebEngineCore.framework/Versions/Current/Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess" DESTINATION ${CMAKE_INSTALL_PREFIX}/MacOS)

  # add locales to bundle
  	file(GLOB _locales_files RELATIVE "${QtCore_DIR}/../../../lib/QtWebEngineCore.framework/Versions/Current/Resources/qtwebengine_locales" "${QtCore_DIR}/../../../lib/QtWebEngineCore.framework/Versions/Current/Resources/qtwebengine_locales/*")
  	foreach(_locales_file ${_locales_files})
        get_filename_component(_resolved_file "${QtCore_DIR}/../../../lib/QtWebEngineCore.framework/Versions/Current/Resources/qtwebengine_locales/${_locales_file}" REALPATH)
        list(APPEND _locales_resolved_files ${_resolved_file})
  	endforeach()
  	install(FILES ${_locales_resolved_files} DESTINATION "${CMAKE_INSTALL_PREFIX}/MacOS/qtwebengine_locales")
  
  # add pak
  file(GLOB _pak_files RELATIVE "${QtCore_DIR}/../../../lib/QtWebEngineCore.framework/Versions/Current/Resources/" "${QtCore_DIR}/../../../lib/QtWebEngineCore.framework/Versions/Current/Resources/*.pak")
  foreach(_pak_file ${_pak_files})
        get_filename_component(_pak_resolved_file "${QtCore_DIR}/../../../lib/QtWebEngineCore.framework/Versions/Current/Resources/${_pak_file}" REALPATH)
        list(APPEND _pak_resolved_files ${_pak_resolved_file})
  endforeach()
  install(FILES ${_pak_resolved_files} DESTINATION "${CMAKE_INSTALL_PREFIX}/")

  # add icudtl.dat
 
  install(PROGRAMS "${QtCore_DIR}/../../../lib/QtWebEngineCore.framework/Versions/Current/Resources/icudtl.dat" DESTINATION ${CMAKE_INSTALL_PREFIX}/)
  	install(PROGRAMS "${QtCore_DIR}/../../../lib/QtWebEngineCore.framework/Versions/Current/Resources/QtWebEngineCore.prl" DESTINATION ${CMAKE_INSTALL_PREFIX}/)
  endif(BUILD_WEB)


# Ensure the actual plugin files are installed instead of symlinks.
file(GLOB _subdirs RELATIVE "${QT_PLUGINS_DIR}" "${QT_PLUGINS_DIR}/*")

foreach(_subdir ${_subdirs})
    if(IS_DIRECTORY "${QT_PLUGINS_DIR}/${_subdir}")
        set(_resolved_files "")
        file(GLOB _plugin_files RELATIVE "${QT_PLUGINS_DIR}/${_subdir}" "${QT_PLUGINS_DIR}/${_subdir}/*")

        foreach(_plugin_file ${_plugin_files})
            get_filename_component(_resolved_file "${QT_PLUGINS_DIR}/${_subdir}/${_plugin_file}" REALPATH)
            list(APPEND _resolved_files ${_resolved_file})
        endforeach()

        install(FILES ${_resolved_files} DESTINATION "${CMAKE_INSTALL_LIBDIR}/qtplugins/${_subdir}")
    endif()
endforeach()

#files installed by homebrew do not have write permission for regular user
install(CODE "execute_process(COMMAND chmod -R a+w ${CMAKE_INSTALL_LIBDIR})")

get_filename_component(APP_PATH ${CMAKE_INSTALL_PREFIX} PATH)

execute_process(
       COMMAND find /usr/local/Cellar/icu4c -name pkgconfig
       RESULT_VARIABLE CMD_ERROR
       OUTPUT_VARIABLE CONFIG_ICU)
set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CONFIG_ICU}")
find_package(PkgConfig)
pkg_check_modules(ICU icu-uc)

execute_process(
       COMMAND find -L /usr/local/Cellar/nglib -name MacOS
       OUTPUT_VARIABLE CONFIG_NGLIB)

install(CODE 
    "message(STATUS \"Making bundle relocatable...\")
    # The top-level CMakeLists.txt should prevent multiple package manager
    # prefixes from being set, so the lib path will resolve correctly...
    execute_process(
        COMMAND python2.7
        ${CMAKE_SOURCE_DIR}/src/Tools/MakeMacBundleRelocatable.py
        ${APP_PATH} ${HOMEBREW_PREFIX}${MACPORTS_PREFIX}/lib ${ICU_PREFIX}/lib/ /usr/local/opt ${CONFIG_NGLIB} ${QtCore_DIR}/../../.. ${XCTEST_PATH} ${WEBKIT_FRAMEWORK_DIR}
    )"
)
