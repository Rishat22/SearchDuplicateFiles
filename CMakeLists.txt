cmake_minimum_required(VERSION 3.5)

if(DEFINED ENV{GITHUB_RUN_NUMBER})
    project(SearchDuplicateFiles VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(SearchDuplicateFiles)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(project_sources
    main.cpp
    )

add_executable(SearchDuplicateFiles ${project_sources} )

target_include_directories(SearchDuplicateFiles
    PUBLIC "${CMAKE_BINARY_DIR}"
)

if (MSVC)
    target_compile_options(SearchDuplicateFiles PRIVATE
        /W4
    )
else ()
    target_compile_options(SearchDuplicateFiles PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS SearchDuplicateFiles RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
