# Compiler specific settings

if(compiler_included)
  return()
endif()
set(compiler_included true)

# Link this 'library' to use the standard warnings
add_library(compiler_options INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(compiler_options INTERFACE "/permissive-")
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

  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      target_compile_options(compiler_options INTERFACE -Wno-c++98-compat)
      target_compile_options(compiler_options INTERFACE -Wno-c++98-compat-pedantic)
      target_compile_options(compiler_options INTERFACE -Wno-c++98-c++11-compat)
      target_compile_options(compiler_options INTERFACE -Wno-documentation-unknown-command)
      target_compile_options(compiler_options INTERFACE -Wno-missing-prototypes)
      target_compile_options(compiler_options INTERFACE -Wno-switch-enum)
      target_compile_options(compiler_options INTERFACE -Wno-float-equal)
  endif()

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(compiler_options
                           INTERFACE -Wmisleading-indentation
                                     -Wduplicated-cond
                                     -Wduplicated-branches
                                     -Wlogical-op
                                     -Wuseless-cast)
  endif()

endif()
