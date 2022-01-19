include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${COIN3D_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${XercesC_INCLUDE_DIRS}
)

set(WebGui_LIBS
    FreeCADGui
)

if(Qt6_FOUND)
    include_directories(${Qt6WebEngineWidgets_INCLUDE_DIRS})
    list(APPEND WebGui_LIBS ${Qt6WebEngineWidgets_LIBRARIES})
    qt6_add_resources(Web_QRC_SRCS Resources/Web.qrc)
else()
    include_directories(${Qt5WebEngineWidgets_INCLUDE_DIRS})
    list(APPEND WebGui_LIBS ${Qt5WebEngineWidgets_LIBRARIES})
    qt5_add_resources(Web_QRC_SRCS Resources/Web.qrc)
endif()

SET(WebGui_SRCS
    ${Web_QRC_SRCS}
    AppWebGui.cpp
    Command.cpp
    PreCompiled.cpp
    PreCompiled.h
    Workbench.cpp
    Workbench.h
    BrowserView.h
    BrowserView.cpp
    CookieJar.cpp   # QWebkit only
    CookieJar.h     # QWebkit only
)

SET(WebGuiIcon_SVG
    Resources/icons/WebWorkbench.svg
)

add_library(WebGui SHARED ${WebGui_SRCS} ${WebGuiIcon_SVG})
target_link_libraries(WebGui ${WebGui_LIBS})


SET_BIN_DIR(WebGui WebGui /Mod/Web)
SET_PYTHON_PREFIX_SUFFIX(WebGui)

fc_copy_sources(WebGui "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/Mod/Web" ${WebGuiIcon_SVG})

INSTALL(TARGETS WebGui DESTINATION ${CMAKE_INSTALL_LIBDIR})

INSTALL(FILES ${WebGuiIcon_SVG} DESTINATION "${CMAKE_INSTALL_DATADIR}/Mod/Web/Resources/icons")
