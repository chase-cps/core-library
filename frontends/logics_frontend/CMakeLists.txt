################################################################################
# CMake file for the CHASE tool managing Logic-based specifications.
# @author Michele Lora
################################################################################

include_directories(
        ${PROJECT_SOURCE_DIR}/third_party/antlr4/runtime/src
        ${PROJECT_SOURCE_DIR}/core/include
        ${PROJECT_SOURCE_DIR}/core/include/representation
        ${PROJECT_SOURCE_DIR}/core/include/utilities
        ${PROJECT_SOURCE_DIR}/frontends/logics_frontend/include
        ${PROJECT_SOURCE_DIR}/frontends/logics_frontend/include/LogicsParser
)

set(Logics_PARSER_SRC_DIR ${PROJECT_SOURCE_DIR}/frontends/logics_frontend/src/LogicsParser)
set(Logics_SRC_DIR ${PROJECT_SOURCE_DIR}/frontends/logics_frontend/src)

set(Logics_PARSER_SRCS
        ${Logics_PARSER_SRC_DIR}/LogicsContractsBaseVisitor.cpp
        ${Logics_PARSER_SRC_DIR}/LogicsContractsLexer.cpp
        ${Logics_PARSER_SRC_DIR}/LogicsContractsVisitor.cpp
        ${Logics_PARSER_SRC_DIR}/LogicsContractsParser.cpp)


if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(chase_ll
       ${Logics_PARSER_SRCS}
       ${Logics_SRC_DIR}/LogicsSpecsBuilder.cc
        )

include(GNUInstallDirs)
set(LLLIB_INSTALL_DIR chase/lib  CACHE STRING ¨¨)
set(LLINCLUDE_INSTALL_DIR chase  CACHE STRING ¨¨)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/frontends/logics_frontend/include/
    DESTINATION ${INCLUDE_INSTALL_DIR}/include/frontends
    )

install(
    TARGETS chase_ll
    EXPORT chaseTargets
    DESTINATION ${LIB_INSTALL_DIR}
    )



