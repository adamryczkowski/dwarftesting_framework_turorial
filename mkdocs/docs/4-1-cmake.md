The cmake language offers great flexibility in a way of expressing the building process. 

Here is the place to write about the best practice, tricks and tips and our conventions.

For instance, here will be a list of all non-obvious cmake-modules that our build system uses. 

(https://github.com/leapmotion/cmake-modules)


# Why CMake?

### Pros:

1. CMake is an industry standard. It is used by our Swiss partners in the repositories they have kindly shared with us.
2. CMake is mature. There is a lot of third-party addons for virtually all situations. Specifically:
	1. CMake supports NVidia's `nvcc`
	2. CMake supports generated source files for Fortran (which is a little tricky; requires Fortran parser built into CMake itself)
	3. CMake supports generating not only Makefiles, but also Ninja files and Eclipse projects (and many more)
	4. CMake supports precompiled headers

### Cons:

1. CMake language is Turing complete, i.e. in principle you can program whole OS on top of it (it wouldn't be efficient though). It is an anti-feature, because 
   it allows you to write unstructured and highly complex code. `CMakeLists.txt` tend to grow to unmanagable beasts.
2. CMake is very complex. It follows from last point and from the fact that it is 17 years old. 
3. There is usually more than one way to program every task. While this statement may look good on the surface, it makes reading the CMake code even harder to read. 

Because the "Cons" which are design flaws and unlikely to be fixed, we (i.e. me, Adam) investigated alternatives. The most tempting alternative is [Meson](http://mesonbuild.com/). Unfortunately, as of writing this text (march 2017) meson falls short of this project's requirements. Nevertheless it will stay on our (or at least my) radar, and from time to time I will return an re-evaluate it. 

Meson has none of the CMake's Cons, but unfortunately it 

1. [Doesn't support CUDA](https://github.com/mesonbuild/meson/issues/1003)
2. [Doesn't support generated files with Fortran code](https://github.com/mesonbuild/meson/issues/1348)
3. [Doesn't support precompiled headers of non-standard libraries (like gridtools)](https://github.com/mesonbuild/meson/issues/1415)



# CMake bast practices

CMake language is very (too?) powerfull and usually there are many ways to accomplish every task. This section is about standardizing the way this language should be used
in order to make code simplier and more readable by humans.

Each `CMakeLists.txt` should contain 6 sections:

1. *Common header* Boilerplate code, that in ideal world should have been included in CMake automatically.
2. *Configuration options* The only place to define all user-visible configuration parameters. Just at the beginning of the CMakeLists, after the mandatory boilerplate gibberish, so it is easily visible for each user.
3. *Code generation* - If we generate code, here is a place to do it, before even mentioning our targets. 
4. *Target definitions* - Ostensibly the most important part. Here we define which sources (or generated sources) are used to build which targets. (In CMake's parlance target is either executable or shared library we want to build).
5. *Library references* - Place to define which 3rd party libraries are needed for each target. This is usually the most difficult and complex step, 
that's why it is abstracted away in a separate file and function, exposing only relevant parameters to the user.
Details can be found in the corresponding cmake file in the CMake folder.
6. Footer: Optimization tricks and testing definitions. Place to define which executables are going to be tested and place to call cotire for targets we wish to optimize with precompiled headers. 

Strcuture of the CMakeLists.txt

```
project(dwarf-gridtools CXX) 

#  Common header - this is a boilerplate code, that in ideal world should have been included in CMake automatically.
# ---------------

cmake_minimum_required(VERSION 3.5) # - We may support earlier versions, but why?

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}") 
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake") # - We are going to expand CMake by custom plugins/code. We will keep this code in the CMake directory.

include(cotire) # - This is a 3rd party module, that easily allows using precompiled headers
set(COTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES 0) # - To maximize number of precompiled headers; this number potentially maybe tweaked later. 



#  Configuration options - This section defines all user-visible configuration parameters.  
# -----------------------  Just at the beginning of the CMakeLists, after the mandatory boilerplate gibberish.


# Set floating point precision
set(GRIDTOOLS_SINGLE_PRECISION OFF CACHE BOOL 
    "Compile for single precision (32-bit) (default: 64-bit)")


#  Code generation - If we generate code, here is a place to do it. 
# -----------------



#  Target definitions - Here we define which sources (or generated sources) are used   
# --------------------  to build which targets. Each dwarf is a separate target.

add_executable(dwarf1 dwarf1.cpp)
add_executable(dwarf2 dwarf2.cpp)
add_executable(dwarf3 dwarf3.cpp)


#  Library references - Here we define which 3rd party libraries are needed for each target. 
# --------------------  This is usually the most difficult and complex step, 
#                       that's why it is abstracted away in a separate file and function,
#                       exposing only relevant parameters to the user.
#                       Details can be found in the corresponding cmake file in the CMake folder.


include(gridtools)
#gridtools also configures boost for us, if we pass CONFIGURE_BOOST flag
if(GRIDTOOLS_SINGLE_PRECISION)
	configure_gridtools(
			TARGETS dwarf1 dwarf2 dwarf3
			CONFIGURE_BOOST
			SINGLE_PRECISION
			)
else()
	configure_gridtools(
			TARGETS dwarf1 dwarf2 dwarf3
			CONFIGURE_BOOST
			)
endif()


set_target_properties(dwarf3 PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "dwarf3.hpp")





#  Footer: Optimization tricks and testing definitions 
# -----------------------------------------------------
# 

cotire(dwarf1 dwarf3) #This command effectively enables precompiled headers for the targets. 
# `dwarf2` is not cotired to measure the relative speed of skipping the precompiled headers


enable_testing()
add_test(do_dwarf1 dwarf1)
add_test(do_dwarf2 dwarf2)
add_test(do_dwarf3 dwarf3)

```

## How to write code that configures library

The most important thing to remember is to write the as a macro (not function), otherwise our calls to `find_package` will be local and wouldn't affect the rest of the project.

Parameters should be read using built in function `cmake_parse_arguments` using the syntax below. 

Provide sensible defaults for each parameter to reduce code bloat in the caller. 

Use mandatory parameter `TARGETS` that accepts list of targets that uses the library and use only `target_include_directories` and `target_link_libraries`. Never use global equvialents, so the compilation process will not try to include/link this library for *all* targets, saving compilation time. 

When you use library that is not going to be installed (like serialbox or gridtools) don't bother writing `Find<library>.cmake` or `Configure<library>.cmake` and subsequently calling `find_package()`. There is no use of it.


```
macro(configure_boost)
	set(options REQUIRED)
	set(oneValueArgs VERSION FUSION_MAX_SIZE)
	set(multiValueArgs COMPONENTS TARGETS)
 
	cmake_parse_arguments(BOOST "${options}" "${oneValueArgs}" "${multiValueArgs}" "${ARGN}")
	message("configure_boost: BOOST_VERSION=${BOOST_VERSION}, BOOST_COMPONENTS=${BOOST_COMPONENTS}")
	message("configure_boost: BOOST_TARGETS=${BOOST_TARGETS}, BOOST_FUSION_MAX_SIZE=${BOOST_FUSION_MAX_SIZE}")

	if(${BOOST_REQUIRED})
		message("find_package(Boost ${BOOST_VERSION} COMPONENTS '${BOOST_COMPONENTS}' REQUIRED)")
		find_package(Boost ${BOOST_VERSION} COMPONENTS system program_options REQUIRED)
		if(NOT(${Boost_FOUND}))
		    message(FATAL_ERROR "Boost not found or insufficient version.")
		endif(NOT(${Boost_FOUND}))
	else()
		find_package(Boost ${BOOST_VERSION} COMPONENTS "${BOOST_COMPONENTS}")
	endif()

	link_directories(${Boost_LIBRARY_DIRS})

	set(Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})

	if (NOT ${BOOST_FUSION_MAX_SIZE} STREQUAL "")
		set(BOOST_FUSION_MAX_SIZE ${BOOST_FUSION_MAX_SIZE} CACHE STRING "Maximal sizes of boost fusion containers (GridTools)" )
		add_definitions(-DFUSION_MAX_VECTOR_SIZE=${BOOST_FUSION_MAX_SIZE})
		add_definitions(-DFUSION_MAX_MAP_SIZE=${BOOST_FUSION_MAX_SIZE})
	endif()

	foreach(TARGET ${BOOST_TARGETS})
		target_include_directories(${TARGET} SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
		target_link_libraries(${TARGET} ${Boost_LIBRARIES})
		if (NOT ${BOOST_FUSION_MAX_SIZE} STREQUAL "")
			target_compile_definitions(${TARGET} PRIVATE "-DFUSION_MAX_VECTOR_SIZE=${BOOST_FUSION_MAX_SIZE}")
			target_compile_definitions(${TARGET} PRIVATE "-DFUSION_MAX_MAP_SIZE=${BOOST_FUSION_MAX_SIZE}")
		endif()
	endforeach()

endmacro()

```

# Other trics and tips



### Embed projects as sub-projects.

`add_subdirectory(<sourcedir> [<binarydir>])`

* CMake creates a Makefile/Solution for each subproject.
* The subproject does not have to reside in a subfolder.

Make sure that all your projects can be built both standalone and as
a subproject of another project:

* Don’t assume that your project root is the build root.
* Don’t modify global compile/link flags.
* Don’t make any global changes!

### Finds preinstalled dependencies

`find_package(Qt5 REQUIRED COMPONENTS Widgets)`

* Can set some variables and define imported targets.
* Supports components.

### Add an executable target.

```
add_executable(tool
      main.cpp
      another_file.cpp
   )
add_executable(my::tool ALIAS tool)
```

### Add a library target.

```
add_library(foo STATIC
      foo1.cpp
      foo2.cpp
   )
add_library(my::foo ALIAS foo)
```

* Libraries can be `STATIC`, `SHARED`, `MODULE`, or `INTERFACE`.
* Default can be controlled with `BUILD_SHARED_LIBS`.
* Always add namespaced aliases for libraries.
* Dont’t make libraries `STATIC`/`SHARED` unless they cannot be built
otherwise.
* Leave the control of `BUILD_SHARED_LIBS` to your clients!

### Usage requirements signature

```
target_<usage requirement>(<target>
   <PRIVATE|PUBLIC|INTERFACE> <lib> ...
   [<PRIVATE|PUBLIC|INTERFACE> <lib> ... ] ...]
   )
```

* `PRIVATE` Only used for this target.
* `PUBLIC` Used for this target and all targets that link against it.
* `INTERFACE` Only used for targets that link against this library.

### Set libraries as dependency.

```
target_link_libraries(foobar
   PUBLIC my::foo
   PRIVATE my::bar
   )
```

* Prefer to link against namespaced targets.
* Specify the dependencies are private or public.
* Avoid the `link_libraries()` command.
* Avoid the `link_directories()` command.
* No need to call `add_dependencies()`.

### Set include directories.

```
target_include_directories(foo
   PUBLIC include
   PRIVATE src
   )
```

* Avoid the include_directories() command.

### Set compile definitions (preprocessor constants).

```
target_compile_definitions(foo
   PRIVATE SRC_DIR=${Foo_SOURCE_DIR}
   )
```

* Avoid the `add_definitions()` command.
* Avoid adding definitions to `CMAKE_<LANG>_FLAGS`.

### Set compile options/flags.

```
if(CMAKE_COMPILER_IS_GNUCXX)
   target_compile_options(foo
      PUBLIC -fno-elide-constructors
      )
endif()
```

* Wrap compiler specific options in an appropriate condition.
* Avoid the `add_compile_options()` command.
* Avoid adding options to `CMAKE_<LANG>_FLAGS`.

### Set required compiler features.

```
target_compile_features(foo
   PUBLIC
      cxx_auto_type
      cxx_range_for
   PRIVATE
      cxx_variadic_templates
   )

```

* CMake will add required compile flags.
* Errors if the compiler is not supported.
* Don’t add `-std=c++11` to `CMAKE_<LANG>_FLAGS`.
* Don’t pass `-std=c++11` to `target_compile_options().`
* Alternatively you can use `set (CMAKE_CXX_STANDARD 11)`


## Best practices

### Goal: No custom variables

Wrong:

```
set(PROJECT foobar)
set(LIBRARIES foo)
if(WIN32)
   list(APPEND LIBRARIES bar)
endif()
target_link_libraries(${Project}
   ${LIBRARIES}
   )
```

Hard to diagnose:
`bar` links against `foo`! But only on Windows!

This is better:

```
target_link_libraries(foobar PRIVATE my::foo)

if(WIN32)
   target_link_libraries(foobar PRIVATE my::bar)
endif()
```

Without variables the code is more robust and easier to read.


### Goal: No custom functions

* Quote: "I wrote many custom functions. Experience tought me: Most of the time it is a bad idea."

Just don't:

```
magic_project(<name> ...)

```


