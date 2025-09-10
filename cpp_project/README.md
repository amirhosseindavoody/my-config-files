# C++ project template

Development environment overview:

- Use `pixi` to install required tools and libraries
  - python
  - cmake
  - clang
- Use CMake's FetchContent to download dependencies during build.
- Use `clangd` for autocompletion and code navigation.
- We can use `zig cc` and `zig c++` as a droping replacement for clang or gcc.
  - See [zig-cross](https://github.com/amirhosseindavoody/zig-cross) repo for example of how to enable `zig cc` compiler for cmake.