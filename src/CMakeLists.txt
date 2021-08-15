set(DISASSEMBLE_SOURCES
  disassemble.c
)

set(DISASSEMBLE_HEADERS
  disassemble.h
)

add_executable(disassemble ${DISASSEMBLE_SOURCES} ${DISASSEMBLE_HEADERS})

set(CPU_SOURCES
  cpu.c
)

set(CPU_HEADERS
  cpu.h
)

add_library(cpu STATIC ${CPU_SOURCES} ${CPU_HEADERS})

set(EMU8080_SOURCES
  emu8080.c
  machine.c
  cpu.c
)

set(EMU8080_HEADERS
  machine.h
  cpu.h
)

if(SDL2_FOUND)
  add_executable(emu8080 ${EMU8080_SOURCES} ${EMU8080_HEADERS})
  target_link_libraries(emu8080 SDL2)
  install(TARGETS emu8080
    RUNTIME DESTINATION bin
  )
endif(SDL2_FOUND)

install(TARGETS disassemble
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/disassemble.h ${CMAKE_CURRENT_SOURCE_DIR}/cpu.h
        DESTINATION include)