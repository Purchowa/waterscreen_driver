# Water Screen Controller - Test Suite

This directory contains unit tests for the Water Screen Controller project. The tests are implemented using the **cmocka** framework and built using **CMake**.

## Prerequisites

Ensure you have the following dependencies installed before proceeding:

- **CMake** (version 3.28 or later)
- **MinGW-w64** (including GCC)
- **Ninja** as the build system

There is no guarantee that the tests will compile when using different compiler.

## Building the Tests manually

1. **Navigate to the test directory**:
   ```sh
   cd waterscreen_driver/tests
   ```
2. **Create a build directory:**
   ```sh
   mkdir build && cd build
   ```
3. **Run CMake with Ninja:**
   ```sh
   cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
   ```
4. **Compile the tests:**
   ```sh
   ninja
   ```

## Running the Tests

Once the tests are compiled, you can execute them by running:

```sh
ctest --extra-verbose
```

Or manually run specific test executables:

```sh
./build/test_example
```

(On Windows, replace `./build/test_example` with `build\test_example.exe`)

---
