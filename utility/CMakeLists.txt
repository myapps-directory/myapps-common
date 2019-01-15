add_library(ola_utility crypto.hpp src/crypto.cpp src/ola_error.cpp)

target_link_libraries(ola_utility ${OPENSSL_CRYPTO_LIBRARY} solid_system)

add_subdirectory(test)
