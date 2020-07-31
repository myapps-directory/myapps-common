add_library(ola_utility encode.hpp ola_error.hpp ola_protocol.hpp src/encode.cpp src/ola_error.cpp)

target_link_libraries(ola_utility ${OPENSSL_CRYPTO_LIBRARY} SolidFrame::solid_system)

add_subdirectory(test)
