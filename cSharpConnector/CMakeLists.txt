set(SOURCES
	main.cpp
	simpleSocket.cpp
	simpleSocket.h
)

setup_executable("${SOURCES}")

get_project_name()
target_link_libraries(${PROJECTNAME} PUBLIC
	"renderer"
	"simulation"
	"wsock32"
	"ws2_32"
)