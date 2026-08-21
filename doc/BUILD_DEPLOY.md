# Joshua-HAL Build and Deployment

## Purpose and scope

Joshua-HAL is the C++17 hardware abstraction layer for the Joshua Sitara platform. Its CMake project builds static/shared libraries, tests, generated protocol support, headers, and CPack packages. The root Makefile supports native builds, ARM64 Docker builds, package generation, SSH deployment, analysis, and Doxygen.

## 1. Prerequisites

- GNU Make, CMake 3.10 or newer, Ninja, a C++17 compiler, and `nproc`.
- `Threads`; the CMake project also expects the compiler, standard C++ runtime, and the checked-in protobuf/gRPC layer under `protos/`.
- Docker with ARM64/QEMU support for cross targets. `make docker-install` can install Docker with `sudo apt` if absent.
- `clang-format`, `cppcheck`, and `doxygen` plus Graphviz for the corresponding quality/documentation targets. The Makefile fails early with install hints when these are missing.
- For remote targets: `sshpass`, SSH/SCP access, compatible ARM64 hardware, and credentials. Defaults are `nvidia`/`nvidia` and `192.168.1.3`/`192.168.1.13`; use `HAL_SSH_USER` and `HAL_SSH_PASS` or Make overrides.

## 2. Setup

Run commands from the repository root. `make configure` creates `build/native` and configures Ninja. `make docker-image` builds the Docker image named `joshua` from `docker/` when it is absent. The Docker workflow mounts this repository at `/workspace`.

## 3. Build and compile

- `make configure` - configure the native tree.
- `make build` - compile native targets with Ninja.
- `make docker-configure` - configure the ARM64 cross tree.
- `make docker-build` - compile the ARM64 tree; this is the default `all` workflow.
- `make docker-shell` - open an ARM64 development shell.
- `make list` - list native/cross binaries when present.

Native artifacts are under `build/native/bin` and `build/native/lib`; cross artifacts are under `build/cross/bin` and `build/cross/lib`. The CMake install rules also install libraries and headers.

## 4. Test and lint

- `make test` - native CTest suite.
- `make docker-test` - CTest under ARM64 Docker/QEMU.
- `make format` - format C++ with clang-format and CMake files with cmake-format when available.
- `make cppcheck` - run the generated cppcheck target.
- `make doc` - generate Doxygen through `doc_doxygen`.

## 5. Packaging and artifacts

- `make package-deb` - native DEB through CPack.
- `make package-rpm` - native RPM; requires `rpmbuild`.
- `make package-ipk` - native IPK via the CMake `package_ipk` target.
- `make package` - DEB plus IPK.
- `make docker-package-deb`, `make docker-package-rpm`, `make docker-package-ipk`, `make docker-package` - corresponding ARM64 package workflows.

Artifacts are reported from `build/native` or generated in `build/cross`; package names are based on `joshua-hal` and the configured architecture. The checked-in `acadia-hal_1.0_amd64.deb` is not a current build output to rely on.

## 6. Deployment, install, and run

- `make deploy-a` / `make deploy-b` - cross-build and copy `build/cross/lib` and `build/cross/bin` to `/tmp/acu-test/` on the selected target.
- `make deploy` - deploy to both configured devices.
- `make run-a [app] [args...]` / `make run-b [app] [args...]` - deploy and run selected remote test applications; no app runs all applications.
- `make run [app] [args...]` - run native applications with `LD_LIBRARY_PATH=build/native/lib`.

The Makefile does not invoke `make install` or a package manager on the target; remote deployment is a temporary `/tmp/acu-test` copy. It clears that directory first and disables SSH host-key checking, so use only with intentional lab devices. This workflow requires network access and credentials, not cloud services.

## 7. Cleanup

- `make clean` removes `build/`.
- `make docker-clean` removes the Docker cross-build directory.
- `make setup-completion` appends the repository completion script to `~/.bashrc`.

## 8. Hardware, cloud, and manual verification caveats

Native compilation and CTest do not prove hardware behavior. ARM64 Docker tests use emulation; verify GPIO, PRU, device paths, timing, and gRPC behavior on the actual Sitara target after deployment. No cloud deployment is defined. Before deployment, override `IP_A`/`IP_B` and credentials explicitly and confirm the target architecture.
