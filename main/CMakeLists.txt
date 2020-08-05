cmake_minimum_required(VERSION 3.5)
set (SOURCES ping.c)

idf_component_register(SRCS ${SOURCES}
    INCLUDE_DIRS ".")

target_compile_definitions(${COMPONENT_LIB} PRIVATE LV_CONF_INCLUDE_SIMPLE=1)

