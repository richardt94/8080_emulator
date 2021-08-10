include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# set CHECK_INSTALL_DIR to override search for Check

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

set(TEST_SOURCES
  test_cpu_instructions.c
)

add_executable(test_cpu ${TEST_SOURCES})
target_link_libraries(test_cpu ${CHECK_LIBRARIES})

#the "integration test" (needs cpudiag binary)
set(CPUDIAG_SOURCES
    cpudiag_shell.c
)

add_executable(cpudiag_shell ${CPUDIAG_SOURCES})