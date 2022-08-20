set(SOURCES
	main.cpp
	helpers.h
	window.hpp
)

setup_executable("${SOURCES}")

get_project_name()
target_link_libraries(${PROJECTNAME} PUBLIC
	"renderer"
	"simulation"
)

# this line is needed for a windowed application
target_link_options(${PROJECTNAME} PRIVATE "/SUBSYSTEM:WINDOWS")
