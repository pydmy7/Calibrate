function(addExecutable target)
    aux_source_directory(./src src)

    add_executable(
        ${target}
        ${src}
    )

    target_include_directories(${target} PRIVATE include)
    target_compile_features(${target} PRIVATE cxx_std_20)

    useSanitize(${target})
    # useClangTidy(${target})
    # useClangFormat(${target} .)
endfunction(addExecutable)


function(addLibrary target)
    aux_source_directory(./src src)

    add_library(
        ${target}
        STATIC
        ${src}
    )

    target_include_directories(${target} PUBLIC include)
    target_compile_features(${target} PRIVATE cxx_std_20)

    useSanitize(${target})
    # useClangTidy(${target})
    # useClangFormat(${target} .)
endfunction(addLibrary)
