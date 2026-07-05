# Zmieniono z 4.2 na 3.20 (wersja 4.2 CMake nie jest standardem)
cmake_minimum_required(VERSION 4.2)
set(CMAKE_CXX_STANDARD 20)

project(Linear)

find_program(GLSL_VALIDATOR glslangValidator REQUIRED)

# 2. Definiujemy LISTĘ wszystkich naszych shaderów
set(COMPUTE_SHADERS
        "src/kernel/mat_mul.comp"
        "src/kernel/mat_det.comp"
)

# Pusta lista, do której będziemy dorzucać wygenerowane nagłówki
set(GENERATED_HEADERS "")

# 3. Pętla kompilująca każdy shader z listy
foreach(SHADER_PATH IN LISTS COMPUTE_SHADERS)

    # Wyciągamy samą nazwę pliku bez rozszerzenia (np. "matMul")
    get_filename_component(SHADER_NAME ${SHADER_PATH} NAME_WE)

    # Generujemy ścieżkę do pliku wyjściowego (np. "build/matMul.hpp")
    set(SHADER_HDR "${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/${SHADER_NAME}.hpp")

    # Tworzymy unikalną nazwę zmiennej C++ (zmieniamy na wielkie litery)
    string(TOUPPER ${SHADER_NAME} SHADER_VAR_NAME)
    set(CPP_ARRAY_NAME "VULKAN_KERNEL_${SHADER_VAR_NAME}")

    # Dodajemy polecenie kompilacji dla TEGO KONKRETNEGO shadera
    add_custom_command(
            OUTPUT ${SHADER_HDR}
            COMMAND ${GLSL_VALIDATOR} -V ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_PATH} --vn ${CPP_ARRAY_NAME} -o ${SHADER_HDR}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_PATH}
            COMMENT "Kompilowanie ${SHADER_NAME}.comp do ${SHADER_NAME}.hpp (Tablica: ${CPP_ARRAY_NAME})..."
    )

    # Dorzucamy wygenerowany plik do naszej listy nagłówków
    list(APPEND GENERATED_HEADERS ${SHADER_HDR})

endforeach()

# 4. Tworzymy bibliotekę, dodając jako zależność CAŁĄ LISTĘ wygenerowanych nagłówków
add_library(VulkanKernel SHARED
        src/vulkanKernel.cpp # <--- POPRAWKA: Dodano folder src/ (upewnij się, że plik tam leży!)
        ${GENERATED_HEADERS}
)

find_package(Vulkan REQUIRED)
target_link_libraries(VulkanKernel PRIVATE Vulkan::Vulkan)

target_include_directories(VulkanKernel PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

add_library(Linear SHARED
        src/library.cpp
        src/vec.h
        src/mat.h
        src/rational.h
)

target_include_directories(Linear PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(VulkanKernel PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Linear PRIVATE VulkanKernel)

add_subdirectory(tests)

set_target_properties(Linear PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)