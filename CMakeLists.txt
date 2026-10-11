cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(pattern-structure-matching VERSION 1.0.0 LANGUAGES CXX)

if(NOT TARGET coretoolkit::coretoolkit)
    find_package(coretoolkit REQUIRED)
endif()
if(NOT TARGET common-neighbor-analysis::common-neighbor-analysis)
    find_package(common-neighbor-analysis REQUIRED)
endif()
if(NOT TARGET structure-identification::structure-identification)
    find_package(structure-identification REQUIRED)
endif()
if(NOT TARGET yaml-cpp::yaml-cpp)
    find_package(yaml-cpp REQUIRED)
endif()

volt_add_plugin(pattern-structure-matching
    SOURCES
        src/analysis/pattern_service.cpp
        src/analysis/pattern_structure_analysis.cpp
        src/analysis/pattern_cluster_input_adapter.cpp
        src/analysis/pattern_classifier.cpp
        src/analysis/pattern_matching_shared.cpp
        src/analysis/pattern_dxa_topology_provider.cpp
        src/patterns/pattern_catalog.cpp
        src/patterns/pattern_source_loader.cpp
        src/patterns/template_structure_loader.cpp
        src/patterns/pattern_compiler.cpp
    MAIN_SOURCE src/plugin.cpp
    DEPENDENCIES
        common-neighbor-analysis::common-neighbor-analysis
        structure-identification::structure-identification
        yaml-cpp::yaml-cpp
    INSTALL_DATA lattices
)
