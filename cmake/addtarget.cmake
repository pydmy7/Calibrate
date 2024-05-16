macro(addExecutable target)
    aux_source_directory(./src src)
    message("${target}_src: ${src}")

    file(GLOB_RECURSE include ./include/${target}/*.hpp)
    message("${target}_include: ${include}")

    add_executable(
        ${target}
        ${src}
    )

    target_include_directories(${target} PRIVATE include)
    target_compile_features(${target} PRIVATE cxx_std_17)
endmacro(addExecutable)


macro(addLibrary target)
    aux_source_directory(./src src)
    message("${target}_src: ${src}")

    file(GLOB_RECURSE include ./include/${target}/*.hpp)
    message("${target}_include: ${include}")

    add_library(
        ${target}
        STATIC
        ${src}
    )

    target_include_directories(${target} PUBLIC include)
    target_compile_features(${target} PRIVATE cxx_std_17)
endmacro(addLibrary)
