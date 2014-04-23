cmake_minimum_required(VERSION 2.8)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/src)

ADD_EXECUTABLE(texec texec.c)
TARGET_LINK_LIBRARIES(texec packetstream pthread)

ADD_EXECUTABLE(drain_test drain_test.c)
TARGET_LINK_LIBRARIES(drain_test packetstream pthread)

IF (UNIX)
  INSTALL(TARGETS texec
  	  RUNTIME DESTINATION bin)
ENDIF (UNIX)
