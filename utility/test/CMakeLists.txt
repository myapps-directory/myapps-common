set( MyAppsUtilityTestSuite
    test_archive.cpp
)

create_test_sourcelist( MyAppsUtilityTests test_utility.cpp ${MyAppsUtilityTestSuite})

add_executable(test_myapps_utility ${MyAppsUtilityTests})

target_link_libraries(test_myapps_utility
    myapps_utility
    Threads::Threads
    ${SYSTEM_BASIC_LIBRARIES}
    ${SYSTEM_DYNAMIC_LOAD_LIBRARY}
)

target_include_directories(test_myapps_utility PRIVATE
    ${Boost_INCLUDE_DIRS}
)
