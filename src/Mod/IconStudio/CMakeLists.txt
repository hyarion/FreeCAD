# SPDX-License-Identifier: LGPL-2.1-or-later

SET(IconStudioMain_SRCS
    Init.py
    InitGui.py
    IconStudio.py
)
SOURCE_GROUP("" FILES ${IconStudioMain_SRCS})

SET(IconStudioResources
    Resources/guides.svg
)
SOURCE_GROUP("Resources" FILES ${IconStudioResources})

SET(all_files ${IconStudioMain_SRCS} ${IconStudioResources})

ADD_CUSTOM_TARGET(IconStudio ALL
    SOURCES ${all_files}
)

fc_copy_sources(IconStudio "${CMAKE_BINARY_DIR}/Mod/IconStudio" ${all_files})

INSTALL(
    FILES ${IconStudioMain_SRCS}
    DESTINATION Mod/IconStudio
)
INSTALL(
    FILES ${IconStudioResources}
    DESTINATION Mod/IconStudio/Resources
)
