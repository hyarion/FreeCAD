set(ENV{PATH} "${FREECAD_LIBPACK_DIR};$ENV{PATH}")
set(ENV{CMAKE_PREFIX_PATH} ${FREECAD_LIBPACK_DIR})

set(Boost_INCLUDE_DIR ${FREECAD_LIBPACK_DIR}/include CACHE PATH "" FORCE)

set(OCE_DIR ${FREECAD_LIBPACK_DIR}/lib/cmake CACHE PATH "" FORCE)

set(SWIG_EXECUTABLE ${FREECAD_LIBPACK_DIR}/bin/swig/swig.exe CACHE FILEPATH "Swig" FORCE)

#set(PYTHON_EXECUTABLE ${FREECAD_LIBPACK_DIR}/bin/python.exe CACHE FILEPATH "" FORCE)
#set(PYTHON_LIBRARY ${FREECAD_LIBPACK_DIR}/lib/python27.lib CACHE FILEPATH "" FORCE)
#set(PYTHON_DEBUG_LIBRARY ${FREECAD_LIBPACK_DIR}/lib/python27_d.lib CACHE FILEPATH "" FORCE)
#set(PYTHON_INCLUDE_DIR ${FREECAD_LIBPACK_DIR}/include/python2.7 CACHE PATH "" FORCE)

# default Qt stuff
# Set paths to cmake config files for each Qt module
set(Qt_ROOT_DIR ${FREECAD_LIBPACK_DIR} CACHE PATH "")

set (Qt_DIR ${Qt_ROOT_DIR}/lib/cmake/Qt${QT_VERSION_MAJOR} CACHE PATH "")
set (QtAxBase_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}AxBase CACHE PATH "")
set (QtAxContainer_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}AxContainer CACHE PATH "")
set (QtAxServer_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}AxServer CACHE PATH "")
set (QtConcurrent_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Concurrent CACHE PATH "")
set (QtCore_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Core CACHE PATH "")
set (QtDBus_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}DBus CACHE PATH "")
set (QtDesigner_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Designer CACHE PATH "")
set (QtGui_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Gui CACHE PATH "")
set (QtHelp_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Help CACHE PATH "")
set (QtLinguistTools_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}LinguistTools CACHE PATH "")
set (QtMultimedia_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Multimedia CACHE PATH "")
set (QtMultimediaWidgets_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}MultimediaWidgets CACHE PATH "")
set (QtNetwork_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Network CACHE PATH "")
set (QtOpenGL_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}OpenGL CACHE PATH "")
set (QtOpenGLExtensions_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}OpenGLExtensions CACHE PATH "")
set (QtPrintSupport_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}PrintSupport CACHE PATH "")
set (QtQml_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Qml CACHE PATH "")
set (QtQuick_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Quick CACHE PATH "")
set (QtQuickTest_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}QuickTest CACHE PATH "")
set (QtQuickWidgets_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}QuickWidgets CACHE PATH "")
set (QtSql_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Sql CACHE PATH "")
set (QtSvg_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Svg CACHE PATH "")
set (QtTest_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Test CACHE PATH "")
set (QtUiPlugin_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}UiPlugin CACHE PATH "")
set (QtUiTools_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}UiTools CACHE PATH "")
set (QtWidgets_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Widgets CACHE PATH "")
set (QtXml_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}Xml CACHE PATH "")
set (QtXmlPatterns_DIR ${Qt_ROOT_DIR}/lib/cmake/${QT_VERSION_MAJOR}XmlPatterns CACHE PATH "")


find_library(XercesC_LIBRARY_RELEASE xerces-c_3 "${FREECAD_LIBPACK_DIR}/lib")
find_library(XercesC_LIBRARY_DEBUG xerces-c_3D "${FREECAD_LIBPACK_DIR}/lib")
set (XercesC_LIBRARIES debug ${XercesC_LIBRARY_DEBUG} optimized ${XercesC_LIBRARY_RELEASE})
set(XercesC_FOUND TRUE) 

find_library(COIN3D_LIBRARY_RELEASE coin4 "${FREECAD_LIBPACK_DIR}/lib")
find_library(COIN3D_LIBRARY_DEBUG coin4d "${FREECAD_LIBPACK_DIR}/lib")
set(COIN3D_LIBRARIES optimized ${COIN3D_LIBRARY_RELEASE}
                     debug ${COIN3D_LIBRARY_DEBUG})
set(COIN3D_FOUND TRUE)

set(NETGENDATA ${FREECAD_LIBPACK_DIR}/include/netgen)

if(FREECAD_USE_FREETYPE)
    set(FREETYPE_INCLUDE_DIR_freetype2 ${FREECAD_LIBPACK_DIR}/include/freetype2)
endif(FREECAD_USE_FREETYPE)

link_directories(${FREECAD_LIBPACK_DIR}/lib)
