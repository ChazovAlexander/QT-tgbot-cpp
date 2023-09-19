# QT-tgbot-cpp
C++ QT GUI and Windows library for Telegram bot API
[Author of tgbot-cpp library](https://github.com/reo7sp/tgbot-cpp)
## Library installation on Windows

### Download vcpkg and tgbot-cpp

Taken from [Vcpkg - Quick Start: Windows](https://github.com/Microsoft/vcpkg/#quick-start-windows).

Prerequisites:
- Windows 7 or newer
- Git
- QT MSVC2019 64bit

First, download and bootstrap vcpkg itself; it can be installed anywhere, but generally we recommend using vcpkg as a submodule for CMake projects, and installing it globally for Visual Studio projects. We recommend somewhere like `C:\src\vcpkg` or `C:\dev\vcpkg`, since otherwise you may run into path issues for some port build systems.

```cmd
> git clone https://github.com/microsoft/vcpkg
> .\vcpkg\bootstrap-vcpkg.bat
```

To install the libraries for Windows x64, run:

```cmd
> .\vcpkg\vcpkg install tgbot-cpp:x64-windows
```
To install for Windows x86, run:

```cmd
> .\vcpkg\vcpkg install tgbot-cpp
```


### Setup project with CMakeLists

Use the [example CMakeLists.txt](/CMakeLists.txt) with changes:

1. Change `target_link_libraries(../vcpkg/installed/x64-windows/lib/TgBot)` to your vcpkg path.

## Bot compilation

### With QT and CMake

1. In project settings set QT MSVC2019 64bit kit. 
2. In CMake configuration select "Initial Configuration".
3. Add new field with key `CMAKE_TOOLCHAIN_FILE` and value `C:/Path/to/vcpkg/../vcpkg/scripts/buildsystems/vcpkg.cmake` 
4. Re-configure with Initial Parameters.
![image_2023-09-19_222005102-fotor-20230919222351](https://github.com/ChazovAlexander/QT-tgbot-cpp/assets/106874694/4d754fb6-6440-476b-9fd8-b05ed173e048)




