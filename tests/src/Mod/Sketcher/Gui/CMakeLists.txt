target_include_directories(Sketcher_tests_run PRIVATE
    ${Qt${FREECAD_QT_MAJOR_VERSION}Widgets_INCLUDE_DIRS}
    ${Qt${FREECAD_QT_MAJOR_VERSION}Gui_INCLUDE_DIRS}
)

target_link_libraries(Sketcher_tests_run
    Qt${FREECAD_QT_MAJOR_VERSION}::Widgets
    Qt${FREECAD_QT_MAJOR_VERSION}::Gui
    SketcherGui
)

target_sources(Sketcher_tests_run PRIVATE
    DrawSketchHandler.cpp
)
