project(obs-rest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(obs-rest_SOURCES
	mongoose.c
	obs-rest.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMG_ENABLE_THREADS")

add_library(obs-rest MODULE
	${obs-rest_SOURCES})
target_link_libraries(obs-rest
	libobs
	${obs-rest_PLATFORM_DEPS})

install_obs_plugin_with_data(obs-rest data)