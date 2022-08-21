cmake_minimum_required(VERSION 3.14)

project(CppWinUITests VERSION 1.0)

include(cmake/helper_functions.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
	gsl
	GIT_REPOSITORY https://github.com/microsoft/GSL.git
	GIT_TAG        a3534567187d2edc428efd3f13466ff75fe5805c # release-v4.0.0
)
FetchContent_MakeAvailable(gsl)

add_subdirectory(cSharpConnector)
add_subdirectory(fileExporter)
add_subdirectory(renderer)
add_subdirectory(renderer_tests)
add_subdirectory(simulation)
add_subdirectory(simulation_tests)
add_subdirectory(winapi)
