# CMakeList.txt : projet CMake pour Arkanoid, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required(VERSION 3.21)  # or higher, depending on your CMake version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Arkanoid LANGUAGES CXX)

# Let CMake know about vcpkg integration (automatic if `vcpkg integrate install` is done)
find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)

file(GLOB ASSETS
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/*.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/*.jpg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/verdana.ttf"
)
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "*.cpp"
    "*.cc"
)

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Arkanoid ${SRC_FILES})

target_compile_features(Arkanoid PRIVATE cxx_std_17)

target_link_libraries(Arkanoid PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)

# Automatically copy required SFML DLLs to the output folder after build
add_custom_command(TARGET Arkanoid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ASSETS}
        $<TARGET_FILE_DIR:Arkanoid>
)

#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#    file(GLOB SFML_DLLS
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/sfml-*-d-3.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/zlibd1.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/bz2d.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/freetyped.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/libpng16d.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/brotlidec.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/brotlicommon.dll"
#    )
#else()
#    file(GLOB SFML_DLLS
#    "${VCPKG_ROOT}/installed/x64-windows/bin/sfml-*-3.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/zlib1.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/bz2.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/freetype.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/libpng16.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/brotlidec.dll"
#    "${VCPKG_ROOT}/installed/x64-windows/debug/bin/brotlicommon.dll"
#    )
#endif()
#add_custom_command(TARGET Arkanoid POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${SFML_DLLS}
#        $<TARGET_FILE_DIR:Arkanoid>
#)

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
