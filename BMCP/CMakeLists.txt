add_subdirectory(DataStructure)

add_library(BMCP BMCP.cpp Input.cpp Output.cpp)

target_link_libraries(BMCP DataStructure)

target_include_directories(BMCP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

