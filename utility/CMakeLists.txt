add_custom_target(versioning
    COMMAND ${CMAKE_COMMAND} -DINFILE="${CMAKE_CURRENT_SOURCE_DIR}/src/version.cpp.in"  -DOUTFILE="${CMAKE_CURRENT_BINARY_DIR}/version.cpp.tmp" -DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}" -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/version_configure_file.cmake"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/version.cpp.tmp" "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/version.cpp PROPERTIES GENERATED TRUE)


add_library(myapps_utility encode.hpp error.hpp protocol.hpp archive.hpp src/encode.cpp src/error.cpp src/archive.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/version.cpp"
)

target_include_directories(myapps_utility PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
)

add_dependencies(myapps_utility build-libzip build-cereal versioning)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version_impl.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/version_impl.hpp)

if(WIN32)
    
else()
    set(LIBLZMA_LIBRARIES lzma)
endif()

target_link_libraries(myapps_utility OpenSSL::Crypto SolidFrame::solid_system Boost::filesystem ${LIBZIP_LIBRARIES} ${LIBLZMA_LIBRARIES})

add_subdirectory(test)
