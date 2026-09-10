add_executable(CrashReporter WIN32
	main.cpp
	Envelope.cpp
	Envelope.hpp
	Report.cpp
	Report.hpp
	Reporter.cpp
	Reporter.hpp
	CrashReporter.manifest)

# Build
target_link_libraries(CrashReporter PRIVATE IzEngine)
target_link_options(CrashReporter PRIVATE /ENTRY:wmainCRTStartup /MANIFEST:EMBED "/MANIFESTINPUT:${CMAKE_CURRENT_SOURCE_DIR}/CrashReporter.manifest")

# Tree
set_target_properties(CrashReporter PROPERTIES FOLDER Tools)
get_target_property(SOURCES CrashReporter SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
