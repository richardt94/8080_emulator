set(DISASSEMBLE_SOURCES
  disassemble.c
)

set(DISASSEMBLE_HEADERS
  disassemble.h
)

add_executable(disassemble ${DISASSEMBLE_SOURCES} ${DISASSEMBLE_HEADERS})

install(TARGETS disassemble
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/disassemble.h ${CMAKE_CURRENT_SOURCE_DIR}/cpu.h
        DESTINATION include)