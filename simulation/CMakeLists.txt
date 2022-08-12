set(SOURCES
	particle.cpp
	particle.h
	simulation.cpp
	simulation.h
	vector.cpp
	vector.h
	../Craft/deps/noise/noise.c
	../Craft/deps/noise/noise.h
)

setup_library("${SOURCES}")

get_project_name()
target_link_libraries(${PROJECTNAME} PUBLIC
	"GSL"
)
target_include_directories(${PROJECTNAME} PUBLIC
		${PROJECT_SOURCE_DIR}/Craft/deps/noise
)
