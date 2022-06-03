# @author Michele Lora

###############################################################################
# CMake file for the CHASE tool managing patterns over networks.
# @author Michele Lora
###############################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/core/include
        ${PROJECT_SOURCE_DIR}/core/include/representation
        ${PROJECT_SOURCE_DIR}/core/include/utilities
        ${PROJECT_SOURCE_DIR}/third_party/antlr4/runtime/src
        ${PROJECT_SOURCE_DIR}/frontends/dsl_frontend/include
        ${PROJECT_SOURCE_DIR}/frontends/dsl_frontend/include/DSLParser
)

set(DSL_FRONTEND_SRC ${PROJECT_SOURCE_DIR}/frontends/dsl_frontend/src)

set(dsl_parser_lib_srcs
        ${DSL_FRONTEND_SRC}/DSLParser/ChaseBaseListener.cpp
        ${DSL_FRONTEND_SRC}/DSLParser/ChaseLexer.cpp
        ${DSL_FRONTEND_SRC}/DSLParser/ChaseListener.cpp
        ${DSL_FRONTEND_SRC}/DSLParser/ChaseParser.cpp

        ${DSL_FRONTEND_SRC}/DSLProblem.cc

        ${DSL_FRONTEND_SRC}/DSLSpecsBuilder.cc
        ${DSL_FRONTEND_SRC}/DSLSpecBuilder_support.cc
        ${DSL_FRONTEND_SRC}/DSLProblemDefinitions.cc
        ${DSL_FRONTEND_SRC}/DSLDesignProblem_patterns.cc
        ${DSL_FRONTEND_SRC}/DSLDesignProblem_support.cc
        )

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(chase_dsl
        ${dsl_parser_lib_srcs}
        )

include(GNUInstallDirs)
set(DSLLIB_INSTALL_DIR chase/lib  CACHE STRING ¨¨)
set(DSLINCLUDE_INSTALL_DIR chase  CACHE STRING ¨¨)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/frontends/dsl_frontend/include/
    DESTINATION ${INCLUDE_INSTALL_DIR}/include/frontends
    )

install(
    TARGETS chase_dsl
    EXPORT chaseTargets
    DESTINATION ${LIB_INSTALL_DIR}
    )



