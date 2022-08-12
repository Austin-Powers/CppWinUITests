include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCES
	particle.cpp
	simulation.cpp
	vector.cpp
)

setup_executable("${SOURCES}")

get_project_name()
target_link_libraries(${PROJECTNAME} PUBLIC
	"simulation"
	"gtest_main"
)
target_include_directories(${PROJECTNAME} PUBLIC
	${PROJECT_SOURCE_DIR}/Craft/deps/noise
)

include(GoogleTest)
gtest_discover_tests(${PROJECTNAME})
