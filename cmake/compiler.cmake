# Compiler specific settings

if(compiler_included)
  return()
endif()
set(compiler_included true)

# Link this 'library' to use the standard warnings
add_library(compiler_options INTERFACE)

if(MSVC)
  target_compile_options(compiler_options INTERFACE /W4 "/permissive-")
  if(EML_WARNING_AS_ERROR)
    target_compile_options(compiler_options INTERFACE /WX)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(compiler_options
                         INTERFACE -Wall
                                   -Wextra
                                   -Wshadow
                                   -Wnon-virtual-dtor
                                   -Wold-style-cast
                                   -Wcast-align
                                   -Wunused
                                   -Woverloaded-virtual
                                   -Wpedantic
                                   -Wconversion
                                   -Wsign-conversion
                                   -Wnull-dereference
                                   -Wdouble-promotion
                                   -Wformat=2)
  if(EML_WARNING_AS_ERROR)
    target_compile_options(compiler_options INTERFACE -Werror)
  endif()

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(compiler_options
                           INTERFACE -Wmisleading-indentation
                                     -Wduplicated-cond
                                     -Wduplicated-branches
                                     -Wlogical-op
                                     -Wuseless-cast
                           )
  endif()
endif()
