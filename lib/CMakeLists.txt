file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(Tetris_cpp STATIC ${SRC_FILES})

target_include_directories(Tetris_cpp PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_options(Tetris_cpp PRIVATE -Wall -Werror)
target_compile_features(Tetris_cpp PRIVATE cxx_std_17)

target_link_libraries(Tetris_cpp PRIVATE ${CURSES_LIBRARIES})