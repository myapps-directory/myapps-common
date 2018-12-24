set( OlaUtilityTestSuite
    test_crypto_basic.cpp
)

create_test_sourcelist( OlaUtilityTests test_utility.cpp ${OlaUtilityTestSuite})

add_executable(test_ola_utility ${OlaUtilityTests})

target_link_libraries(test_ola_utility
    ola_utility
    ${OPENSSL_LIBRARIES}
    ${SYSTEM_BASIC_LIBRARIES}
    ${SYSTEM_DYNAMIC_LOAD_LIBRARY}
)
