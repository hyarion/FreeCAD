include_directories(${CMAKE_SOURCE_DIR}/src/3rdParty/json/single_include/nlohmann)


target_sources(PartDesign_tests_run PRIVATE
        BackwardCompatibility.cpp
        DatumPlane.cpp
        FeatureHole.cpp
        ShapeBinder.cpp
        Pad.cpp
)

set(PartDesignTestData_Files
        TestModels/ModelFromV021.FCStd
)

ADD_CUSTOM_TARGET(PartDesignTestData ALL
        SOURCES ${PartDesignTestData_Files}
)

fc_target_copy_resource(PartDesignTestData
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}/tests
        ${PartDesignTestData_Files}
)
