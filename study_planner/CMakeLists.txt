set(CMAKE_PREFIX_PATH "C:\\Qt\\6.9.2\\msvc2022_64\\lib\\cmake")
set(nlohmann_json_DIR "C:\\cpp_json\\out\\build\\x64-Debug")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(nlohmann_json 3.12.0 REQUIRED)

qt_standard_project_setup()

qt_add_executable(study_planner study_planner.cpp Course.cpp ParticipationPlan.cpp ExamPlan.cpp Period.cpp)

target_link_libraries(study_planner PRIVATE Qt6::Widgets PRIVATE nlohmann_json::nlohmann_json)

configure_file(
    ${CMAKE_SOURCE_DIR}/data/courses.json
    ${CMAKE_BINARY_DIR}/data/courses.json
    COPYONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/data/delete_button_icon.png
    ${CMAKE_BINARY_DIR}/data/delete_button_icon.png
    COPYONLY
)

set_target_properties(study_planner PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET study_planner PROPERTY CXX_STANDARD 20)
endif()

INSTALL(
    FILES ${CMAKE_SOURCE_DIR}/data/courses.json ${CMAKE_SOURCE_DIR}/data/delete_button_icon.png
    DESTINATION data
)

INSTALL(
    FILES 
    DESTINATION data
)

install(TARGETS study_planner
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET study_planner
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})