set(SOURCES
	main.cpp
)

setup_executable("${SOURCES}")

get_project_name()
target_link_libraries(${PROJECTNAME} PUBLIC
	"renderer"
	"simulation"
)