cmake_minimum_required(VERSION 3.10.2)

list(APPEND 
     CMAKE_MODULE_PATH 
     ${CMAKE_CURRENT_LIST_DIR}/find_modules
)

function(set_target_cpp_standard target standard)
set_target_properties(
    ${target}
    PROPERTIES
        CXX_STANDARD ${standard}
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
endfunction()

function(enable_target_warnings target)
    if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        target_compile_options(
            ${target}
                PRIVATE
                  /W4
                  /WX
        )
        return()
    endif()
    
    target_compile_options(
        ${target}
            PRIVATE
              -Wall
              -Wextra
              -Werror
              -Wundef
              -Wuninitialized
              -Wshadow
              -Wpointer-arith
              -Wcast-align
              -Wcast-qual
              -Wunused-parameter
              -Wdouble-promotion
              -Wnull-dereference
    )
    
    if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND NOT ${USE_IWYU})
        #supported only in GNU
        #however include-what-you-use is not happy with those options
        target_compile_options(
          ${target}
              PRIVATE
                -Wlogical-op
                -Wduplicated-cond
                -Wduplicated-branches
        )
    endif()
endfunction()
