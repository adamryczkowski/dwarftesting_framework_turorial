The cmake language offers great flexibility in a way of expressing the building process. 

Here is the place to write about the best practice, tricks and tips and our conventions.

For instance, here will be a list of all non-obvious cmake-modules that our build system uses. 

[https://github.com/leapmotion/cmake-modules]


# CMake bast practices

Source: Presentation by 
Daniel Pfeifer,
May 21, 2015. 

### Sets the mimimum required version of CMake.


`cmake_minimum_required(VERSION 3.2 FATAL_ERROR)`

* Prefer the latest version of CMake.
* Please don’t set 2.8 as the minimum.

### Set the name and version; enable languages.

`project(<name> VERSION <version> LANGUAGES CXX)`

* CMake sets several variables based on project().
* Call to project() must be direct, not through a
function/macro/include.
* CMake will add a call to project() if not found on the top level.

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


