
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${OCC_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIR}
    ${XercesC_INCLUDE_DIRS}
)

if(Qt6_FOUND)
    set(QtNetwork_INCLUDE_DIRS ${Qt6Network_INCLUDE_DIRS})
    set(QtNetwork_LIBRARIES ${Qt6Network_LIBRARIES})
else()
    set(QtNetwork_INCLUDE_DIRS ${Qt5Network_INCLUDE_DIRS})
    set(QtNetwork_LIBRARIES ${Qt5Network_LIBRARIES})
endif()
include_directories(
    ${QtNetwork_INCLUDE_DIRS}
)
set(Web_LIBS
     FreeCADApp
     ${QtNetwork_LIBRARIES}
)

SET(Web_SRCS
    AppWeb.cpp
    PreCompiled.cpp
    PreCompiled.h
    Server.cpp
    Server.h
)

add_library(Web SHARED ${Web_SRCS})
target_link_libraries(Web ${Web_LIBS})


SET_BIN_DIR(Web Web /Mod/Web)
SET_PYTHON_PREFIX_SUFFIX(Web)

install(TARGETS Web DESTINATION ${CMAKE_INSTALL_LIBDIR})
