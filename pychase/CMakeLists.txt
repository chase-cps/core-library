find_package(PythonLibs REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/core/include
        ${PROJECT_SOURCE_DIR}/core/include/representation
        ${PROJECT_SOURCE_DIR}/core/include/utilities
)

pybind11_add_module(
        pychase MODULE
        bindings.cc
)

target_link_libraries(pychase PRIVATE
        chase
        )

install(
        TARGETS pychase
        EXPORT pychaseTargets
        DESTINATION ${LIB_INSTALL_DIR}
)
