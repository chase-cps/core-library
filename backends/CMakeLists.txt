###############################################################################
# CMake file for the CHASE Library standard backends.
# @author Michele Lora
###############################################################################

set( BACKENDS_SRC ${PROJECT_SOURCE_DIR}/backends/src)


set(chase_library_backends
        ${BACKENDS_SRC}/GR1CPrinter.cc
        ${BACKENDS_SRC}/GR1CPrinter_visit.cc
        ${BACKENDS_SRC}/SlugsPrinter.cc
        ${BACKENDS_SRC}/SlugsPrinter_visit.cc
        ${BACKENDS_SRC}/NuSMVPrinter.cc
        ${BACKENDS_SRC}/NuSMVPrinter_visit.cc
        ${BACKENDS_SRC}/PySTLPrinter.cc
)

include_directories(
    ${PROJECT_SOURCE_DIR}/core/include
    ${PROJECT_SOURCE_DIR}/core/include/representation
    ${PROJECT_SOURCE_DIR}/core/include/utilities
    ${PROJECT_SOURCE_DIR}/backends/include
    )

add_library(chase_backends ${chase_library_backends})

include(GNUInstallDirs)
set(BACKENDSLIB_INSTALL_DIR chase/lib  CACHE STRING ¨¨)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/backends/include/
    DESTINATION ${INCLUDE_INSTALL_DIR}/include/backends
    )

install(
    TARGETS chase_backends
    EXPORT chaseTargets
    DESTINATION ${LIB_INSTALL_DIR}
    )




