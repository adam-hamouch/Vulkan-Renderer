cmake_minimum_required(VERSION 3.16)
project(VulkanRenderer)

set(VKRENDERER_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")
message("[VulkanRenderer]: Output directory: ${VKRENDERER_OUTPUT_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${VKRENDERER_OUTPUT_DIR}/libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${VKRENDERER_OUTPUT_DIR}/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${VKRENDERER_OUTPUT_DIR}")

find_package(Vulkan REQUIRED)

# ATTENTION : si tu crees une lib statique separee plus tard (RHI, ECS...),
# deplace ses sources hors de Code/Source/ et cree un add_library dedie
# pour ne pas les dupliquer dans l'executable.
# ─────────────────────────────────────────────────────────────────────────────
file(GLOB_RECURSE VK_SOURCES "Code/Source/*.cpp")

file(GLOB_RECURSE MATHS_SOURCES "Libs/maths/src/*.cpp")
add_library(MATHS STATIC ${MATHS_SOURCES}
        Libs/maths/includes/Math.h)
target_compile_features(MATHS PUBLIC cxx_std_20)

# ─────────────────────────────────────────────────────────────────────────────
# EXECUTABLE
# Point d'entree du moteur. Quand ton projet grandira, seul main.cpp
# devrait rester ici — la logique metier sera deplacee dans des libs.
# ─────────────────────────────────────────────────────────────────────────────
add_executable(VulkanRenderer ${VK_SOURCES})

target_compile_features(VulkanRenderer PUBLIC cxx_std_20)

target_compile_definitions(VulkanRenderer PRIVATE
    $<$<CONFIG:Debug>:VULKAN_VALIDATION_LAYERS>
    PLATFORM_WINDOWS
)


target_include_directories(VulkanRenderer PUBLIC Code/Include)

# ─────────────────────────────────────────────────────────────────────────────
# LIBRAIRIES
# Pour ajouter une lib externe :
#   1. Via vcpkg : ajoute le package dans vcpkg.json puis find_package + link
#      Ex : GLM (header-only, pas de link necessaire, juste include)
#      Ex : stb_image (header-only, pas de link)
#
#   2. Lib systeme avec find_package :
#      find_package(NomLib REQUIRED)
#      target_link_libraries(VulkanRenderer PRIVATE NomLib::NomLib)
#
#   3. Lib dans Libs/ via add_subdirectory :
#      target_link_libraries(VulkanRenderer PRIVATE NomDeLaLib)
# ─────────────────────────────────────────────────────────────────────────────
target_link_libraries(VulkanRenderer PRIVATE
    Vulkan::Vulkan
    glfw 
    MATHS
    # Ajouter ici les prochaines libs :
    # glm          (si packagée, sinon header-only dans Code/Include/)
    # ImGui        (debug UI, a ajouter quand tu veux un overlay de debug)
)

# ─────────────────────────────────────────────────────────────────────────────
# SOUS-MODULES
# Libs/ contient les librairies tierces compilees depuis leurs sources
# (GLFW, potentiellement ImGui, VMA...).
# Chaque sous-dossier de Libs/ doit avoir son propre CMakeLists.txt.
#
# Pour ajouter une lib dans Libs/ :
#   1. Copie ou clone la lib dans Libs/NomLib/
#   2. add_subdirectory(Libs/NomLib)
#   3. Ajoute-la dans target_link_libraries ci-dessus
# ─────────────────────────────────────────────────────────────────────────────
add_subdirectory(Libs/glfw)
# ─────────────────────────────────────────────────────────────────────────────
# QUAND TON PROJET GRANDIRA — prochaines etapes CMake :
#
# Separer la RHI en lib statique :
#   file(GLOB_RECURSE RHI_SOURCES "Code/RHI/*.cpp")
#   add_library(RHI STATIC ${RHI_SOURCES})
#   target_link_libraries(RHI PRIVATE Vulkan::Vulkan)
#   target_link_libraries(VulkanRenderer PRIVATE RHI)
#
# Separer l'ECS :
#   file(GLOB_RECURSE ECS_SOURCES "Code/ECS/*.cpp")
#   add_library(ECS STATIC ${ECS_SOURCES})
#   target_link_libraries(VulkanRenderer PRIVATE ECS)
# ─────────────────────────────────────────────────────────────────────────────