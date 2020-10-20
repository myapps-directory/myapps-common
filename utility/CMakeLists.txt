add_custom_target(versioning
    COMMAND ${CMAKE_COMMAND} -DINFILE="${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in"  -DOUTFILE="${CMAKE_CURRENT_BINARY_DIR}/version.cpp.tmp" -DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}" -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version_configure_file.cmake"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/version.cpp.tmp" "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/version.cpp PROPERTIES GENERATED TRUE)


add_library(ola_utility encode.hpp ola_error.hpp ola_protocol.hpp src/encode.cpp src/ola_error.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
)

target_include_directories(ola_utility PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
)

add_dependencies(ola_utility versioning)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version_impl.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/version_impl.hpp)

target_link_libraries(ola_utility ${OPENSSL_CRYPTO_LIBRARY} SolidFrame::solid_system)

add_subdirectory(test)
