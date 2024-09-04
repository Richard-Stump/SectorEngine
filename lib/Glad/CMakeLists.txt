cmake_minimum_required( VERSION 3.20.0 )

set(GLAD_SOURCE_FILES src/glad.c)
set(GLAD_HEADER_FILES include/glad/glad.h include/khr/khrplatform.h)

add_library(glad
	${GLAD_SOURCE_FILES}
	${GLAD_HEADER_FILES}
)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GLAD_HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GLAD_SOURCE_FILES})

message(WARNING ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_include_directories(glad PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/
)

set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)