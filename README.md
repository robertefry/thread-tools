
# Initial CPP

Setting up a new C++ project usually requires more time than the endurance of ideas. Even more so for modern C++ projects which require static analysis checks, test suites, documentation, and a slew of other requirements. This template is here to streamline the process of beginning a new C++ project.

Heavily inspired by [ModernCppStarter](https://github.com/TheLartians/ModernCppStarter) and [cpp-best-practices](https://github.com/cpp-best-practices).

## Features

* Conformant to [Modern CMake practices](https://github.com/TheLartians/ModernCppStarter).
* Total separation of project units (libraries/executables/etc...).
* Reproducible dependency management via [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake).
* Integrated [GoogleTest](https://github.com/google/googletest) testing suite.
* Integrated LLVM linting via [clang-tidy](https://clang.llvm.org/extra/clang-tidy/).
* Integrated LLVM sanitizer checking.
* Installable targets via [PackageProject.cmake](https://github.com/TheLartians/PackageProject.cmake).

## Features In-Progress

* Continuous integration via [GitHub Actions](https://help.github.com/en/actions/).
* Code coverage via [codecov](https://codecov.io/).
* Code formatting enforced by [clang-format](https://clang.llvm.org/docs/ClangFormat.html).
* Automatic documentation via [Doxygen](https://www.doxygen.nl/).
