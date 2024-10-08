set(TEST_SOURCE_FILES
        #common
        card_test.cpp
        hand_test.cpp
        player_test.cpp
        draw_pile_test.cpp
        discard_pile_test.cpp
        game_state_test.cpp
        #server
        request_handler_test.cpp)

add_executable(Uno-tests ${TEST_SOURCE_FILES})

target_compile_definitions(Uno-tests PRIVATE UNO_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)
target_link_libraries(Uno-tests PRIVATE gtest gtest_main Uno-lib)

# linking to sockpp
target_link_libraries(Uno-tests PRIVATE sockpp)
if(WIN32)
    message("Detected Win32")

    target_link_libraries(Uno-tests PRIVATE wsock32 ws2_32)
else()
    message("Not Win32, so probably a Linux") # We assume it's Linux in this case

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(Uno-tests PRIVATE Threads::Threads)
endif()

