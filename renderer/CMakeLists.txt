set(SOURCES
	renderer.cpp
	renderer.h
)

setup_library("${SOURCES}")

get_project_name()
target_link_libraries(${PROJECTNAME} PUBLIC
	"GSL"
	"simulation"
)
