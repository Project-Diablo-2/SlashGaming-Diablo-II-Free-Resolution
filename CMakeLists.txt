# SlashGaming Diablo II Free Resolution
# Copyright (C) 2019-2022  Mir Drualga
#
# This file is part of SlashGaming Diablo II Free Resolution.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Additional permissions under GNU Affero General Public License version 3
# section 7
#
# If you modify this Program, or any covered work, by linking or combining
# it with Diablo II (or a modified version of that game and its
# libraries), containing parts covered by the terms of Blizzard End User
# License Agreement, the licensors of this Program grant you additional
# permission to convey the resulting work. This additional permission is
# also extended to any combination of expansions, mods, and remasters of
# the game.
#
# If you modify this Program, or any covered work, by linking or combining
# it with any Graphics Device Interface (GDI), DirectDraw, Direct3D,
# Glide, OpenGL, or Rave wrapper (or modified versions of those
# libraries), containing parts not covered by a compatible license, the
# licensors of this Program grant you additional permission to convey the
# resulting work.
#
# If you modify this Program, or any covered work, by linking or combining
# it with any library (or a modified version of that library) that links
# to Diablo II (or a modified version of that game and its libraries),
# containing parts not covered by a compatible license, the licensors of
# this Program grant you additional permission to convey the resulting
# work.

cmake_minimum_required(VERSION 3.24)

# Name of the project, also is the name of the file
project(SGD2FreeRes)

# Define requirements for C
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Define requirements for C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# External dependencies
add_subdirectory(external)

# Enable NASM
set(CMAKE_ASM_NASM_OBJECT_FORMAT win32)
enable_language(ASM_NASM)

# Remove MinGW compiled binary "lib" prefix
if (MINGW)
    set(CMAKE_IMPORT_LIBRARY_PREFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif (MINGW)

# List all of the source files here
set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SlashGaming-Diablo-II-Free-Resolution")

set(RESOURCE_FILES
    "${PROJECT_DIR}/resource/resource.rc"
)

set(CPP_FILES
    "${PROJECT_DIR}/src/helper/800_interface_bar.cc"
    "${PROJECT_DIR}/src/helper/abstract_multiversion_patch.cpp"
    "${PROJECT_DIR}/src/helper/abstract_version_patch.cpp"
    "${PROJECT_DIR}/src/helper/cel_file_collection.cc"
    "${PROJECT_DIR}/src/helper/custom_mpq.cc"
    "${PROJECT_DIR}/src/helper/ddraw_version.cc"
    "${PROJECT_DIR}/src/helper/evaluation.c"
    "${PROJECT_DIR}/src/helper/file_string.cc"
    "${PROJECT_DIR}/src/helper/file_version.cc"
    "${PROJECT_DIR}/src/helper/game_resolution.cc"
    "${PROJECT_DIR}/src/helper/glide3x_d2dx.cpp"
    "${PROJECT_DIR}/src/helper/glide3x_d2gl.cpp"
    "${PROJECT_DIR}/src/helper/glide3x_version.cc"
    "${PROJECT_DIR}/src/helper/patch_address_and_size.cpp"
    "${PROJECT_DIR}/src/helper/position_realignment.cc"
    "${PROJECT_DIR}/src/patches/patches.cc"
    "${PROJECT_DIR}/src/sgd2mapi_extension/game_function/d2client_function/d2client_draw_screens.cpp"
    "${PROJECT_DIR}/src/sgd2mapi_extension/game_variable/d2gfx/is_perspective_mode_enabled.cpp"
    "${PROJECT_DIR}/src/config.cc"
    "${PROJECT_DIR}/src/dll_main.cc"
    "${PROJECT_DIR}/src/license.c"
    "${PROJECT_DIR}/src/sgd2fml_mod_exports.cc"
    "${PROJECT_DIR}/src/helper/checksum_hash.c"
)

set(NASM_FILES
    "${PROJECT_DIR}/src/helper/cel_file_collection_asm.asm"
)

set(PCH_C_FILES
    "${PROJECT_DIR}/src/pch.h"
)

set(PCH_CXX_FILES
    "${PROJECT_DIR}/src/pch.hpp"
)

set(SOURCE_FILES
    ${RESOURCE_FILES}

    ${CPP_FILES}
    ${NASM_FILES}
)

# Output DLL
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:C>:${PCH_C_FILES}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${PCH_CXX_FILES}>"
)

add_subdirectory(${PROJECT_DIR})

add_dependencies(${PROJECT_NAME}
    libMDCc
    libMDCcpp98
    libSGD2MAPI
    rapidjson
    mjsoni)
target_link_libraries(${PROJECT_NAME} PUBLIC
    libMDCc
    libMDCcpp98
    libSGD2MAPI
    rapidjson
    mjsoni)
get_target_property(SOURCE_FILES ${PROJECT_NAME} SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

# MSVC options
if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NOMINMAX _CRT_SECURE_NO_WARNINGS)
endif (MSVC)

# MinGW options
if (MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE "-std=c++20")
endif (MINGW)

install(TARGETS ${PROJECT_NAME})
