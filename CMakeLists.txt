###############################################################################
# Main CMake file for the CHASE core library
###############################################################################
# @author Michele Lora
#

cmake_minimum_required (VERSION 3.10)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS_DEBUG "-O0 -Wall -Wextra -pedantic -ggdb -fPIC")
set(CMAKE_C_FLAGS_RELEASE "-O3 -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -Wextra -pedantic -ggdb -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3  -fPIC")

if(DEFINED ENV{CHASE_INSTALL_DIR})
    set(CMAKE_INSTALL_PREFIX $ENV{CHASE_INSTALL_DIR} CACHE PATH "Install top-level directory" FORCE)
else()
    set(CMAKE_INSTALL_PREFIX ../../install_dir CACHE PATH "Install top-level directory" FORCE)
endif()

project (CHASE VERSION 1.0.0)

find_package(PythonLibs REQUIRED)
find_package(pybind11 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Enable building tests" ON)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Set a default build type if none was specified
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(default_build_type "Debug")
endif()


if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
# Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set( DEFAULT_INSTALL_LOCATION chase-${CMAKE_BUILD_TYPE} )

option(ENABLE_INSTALL_PREFIX "Install build targets to system (path given by '-DCMAKE_INSTALL_PREFIX' or '${CMAKE_INSTALL_PREFIX}' if not specified)." ON)
if ( ENABLE_INSTALL_PREFIX )
        set (CMAKE_INSTALL_PREFIX ${DEFAULT_INSTALL_LOCATION} CACHE PATH "Installation root")
else()
        set (CMAKE_INSTALL_PREFIX ${DEFAULT_INSTALL_LOCATION} CACHE PATH "Installation root" FORCE)
endif()

set( SRC_CHASELIB_PATH ${PROJECT_SOURCE_DIR}/src)

set(chase_library_representation
    ${SRC_CHASELIB_PATH}/representation/ChaseObject.cc
    ${SRC_CHASELIB_PATH}/representation/DesignProblem.cc

    ${SRC_CHASELIB_PATH}/representation/Scope.cc
    ${SRC_CHASELIB_PATH}/representation/System.cc
    ${SRC_CHASELIB_PATH}/representation/Specification.cc
    ${SRC_CHASELIB_PATH}/representation/Contract.cc

    ${SRC_CHASELIB_PATH}/representation/Contract_algebra.cc
    ${SRC_CHASELIB_PATH}/representation/Library.cc

    ${SRC_CHASELIB_PATH}/representation/Type.cc
    ${SRC_CHASELIB_PATH}/representation/SimpleType.cc
    ${SRC_CHASELIB_PATH}/representation/Integer.cc
    ${SRC_CHASELIB_PATH}/representation/Real.cc
    ${SRC_CHASELIB_PATH}/representation/Boolean.cc
    ${SRC_CHASELIB_PATH}/representation/String.cc
    ${SRC_CHASELIB_PATH}/representation/CustomType.cc
    ${SRC_CHASELIB_PATH}/representation/Enumeration.cc

    ${SRC_CHASELIB_PATH}/representation/Value.cc
    ${SRC_CHASELIB_PATH}/representation/Range.cc
    ${SRC_CHASELIB_PATH}/representation/NumericValue.cc
    ${SRC_CHASELIB_PATH}/representation/IntegerValue.cc
    ${SRC_CHASELIB_PATH}/representation/RealValue.cc
    ${SRC_CHASELIB_PATH}/representation/BooleanValue.cc
    ${SRC_CHASELIB_PATH}/representation/StringValue.cc
    ${SRC_CHASELIB_PATH}/representation/Identifier.cc
    ${SRC_CHASELIB_PATH}/representation/Expression.cc

    ${SRC_CHASELIB_PATH}/representation/Interval.cc
    ${SRC_CHASELIB_PATH}/representation/FunctionCall.cc
    ${SRC_CHASELIB_PATH}/representation/ProbabilityFunction.cc

    ${SRC_CHASELIB_PATH}/representation/Name.cc
    ${SRC_CHASELIB_PATH}/representation/Declaration.cc
    ${SRC_CHASELIB_PATH}/representation/DataDeclaration.cc
    ${SRC_CHASELIB_PATH}/representation/Variable.cc
    ${SRC_CHASELIB_PATH}/representation/Constant.cc
    ${SRC_CHASELIB_PATH}/representation/Parameter.cc
    ${SRC_CHASELIB_PATH}/representation/Function.cc
    ${SRC_CHASELIB_PATH}/representation/ComponentDefinition.cc

    ${SRC_CHASELIB_PATH}/representation/Operators.cc

    ${SRC_CHASELIB_PATH}/representation/Constraint.cc
    ${SRC_CHASELIB_PATH}/representation/Proposition.cc
    ${SRC_CHASELIB_PATH}/representation/UnaryBooleanFormula.cc
    ${SRC_CHASELIB_PATH}/representation/BinaryBooleanFormula.cc
    ${SRC_CHASELIB_PATH}/representation/BooleanConstant.cc
    ${SRC_CHASELIB_PATH}/representation/ModalFormula.cc
    ${SRC_CHASELIB_PATH}/representation/UnaryTemporalFormula.cc
    ${SRC_CHASELIB_PATH}/representation/BinaryTemporalFormula.cc
    ${SRC_CHASELIB_PATH}/representation/LargeBooleanFormula.cc
    ${SRC_CHASELIB_PATH}/representation/QuantifiedFormula.cc
    ${SRC_CHASELIB_PATH}/representation/Graph.cc
    ${SRC_CHASELIB_PATH}/representation/GraphVertex.cc
    ${SRC_CHASELIB_PATH}/representation/GraphEdge.cc

    ${SRC_CHASELIB_PATH}/representation/Matrix.cc
    ${SRC_CHASELIB_PATH}/representation/Distribution.cc

    ${SRC_CHASELIB_PATH}/representation/Contract_algebra.cc
    ${SRC_CHASELIB_PATH}/representation/Component.cc
    ${SRC_CHASELIB_PATH}/representation/Distribution.cc
    )

set(chase_library_utilities
    ${SRC_CHASELIB_PATH}/utilities/IOUtils.cc
    ${SRC_CHASELIB_PATH}/utilities/UtilityFunctions.cc
    ${SRC_CHASELIB_PATH}/utilities/GraphUtilities.cc
    ${SRC_CHASELIB_PATH}/utilities/ClonedDeclarationVisitor.cc
    ${SRC_CHASELIB_PATH}/utilities/LogicSimplificationVisitor.cc
    ${SRC_CHASELIB_PATH}/utilities/LogicNotNormalizationVisitor.cc
    ${SRC_CHASELIB_PATH}/utilities/GroupTemporalOperatorsVisitor.cc
    ${SRC_CHASELIB_PATH}/utilities/VarsCausalityVisitor.cc
    ${SRC_CHASELIB_PATH}/utilities/LogicIdentificationVisitor.cc
    ${SRC_CHASELIB_PATH}/utilities/simplify.cc
    ${SRC_CHASELIB_PATH}/utilities/BaseVisitor.cc
    ${SRC_CHASELIB_PATH}/utilities/GuideVisitor.cc
    ${SRC_CHASELIB_PATH}/utilities/Factory_baseFunctions.cc

    )



set(chase_library
    ${chase_library_representation}
    ${chase_library_utilities}) 

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/representation
    ${PROJECT_SOURCE_DIR}/include/utilities
    )

add_library(chase ${chase_library})
set_target_properties(chase PROPERTIES POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)
set(LIB_INSTALL_DIR chase/lib  CACHE STRING ¨¨)
set(INCLUDE_INSTALL_DIR chase  CACHE STRING ¨¨)

add_custom_target(install_chase
        "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target install
        DEPENDS ${chase}
        DEPENDS ${pychase}
        COMMENT "Installing CHASE core library")

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${INCLUDE_INSTALL_DIR}/include
)

install(
    TARGETS chase
    EXPORT chaseTargets
    DESTINATION ${LIB_INSTALL_DIR}
    )

include(CMakePackageConfigHelpers)
configure_package_config_file( 
    ${PROJECT_SOURCE_DIR}/chaseConfig.cmake.in
    "chaseConfig.cmake"
    INSTALL_DESTINATION ${LIB_INSTALL_DIR}/cmake/chase
    PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR )

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/chaseConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
    )

### Install Config and ConfigVersion files
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/chaseConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/chaseConfigVersion.cmake"
    DESTINATION "${LIB_INSTALL_DIR}/cmake/chase"
    )


add_subdirectory(pychase)
add_subdirectory(doc)


