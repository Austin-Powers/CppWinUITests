set(SOURCES
	noise.cpp
	particle.cpp
	particle.h
	simulation.cpp
	simulation.h
	vector.cpp
	vector.h
)

setup_library("${SOURCES}")

get_project_name()
target_link_libraries(${PROJECTNAME} PUBLIC
	"GSL"
)
target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}/Craft/deps/noise
)
