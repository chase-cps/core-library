find_package(PythonLibs REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/core/include
        ${PROJECT_SOURCE_DIR}/core/include/representation
        ${PROJECT_SOURCE_DIR}/core/include/utilities
        ${PROJECT_SOURCE_DIR}/frontends/dsl_frontend/include
        ${PROJECT_SOURCE_DIR}/frontends/dsl_frontend/include/DSLParser
        ${PROJECT_SOURCE_DIR}/frontends/logics_frontend/include
        ${PROJECT_SOURCE_DIR}/frontends/logics_frontend/include/LogicsParser
        ${PROJECT_SOURCE_DIR}/backends/include
        ${PROJECT_SOURCE_DIR}/third_party/antlr4/runtime/src
)



pybind11_add_module(
        pychase MODULE
        bindings.cc
)

target_link_libraries(pychase PRIVATE
        chase_backends
        chase_dsl
        chase_ll
        antlr4_static
        chase
        )

install(
        TARGETS pychase
        EXPORT pychaseTargets
        DESTINATION ${LIB_INSTALL_DIR}
)
