![Logo](https://media.githubusercontent.com/media/XyrisOS/artwork/stable/images/xyris-banner-full.png)

[![Discord](https://img.shields.io/discord/732032083647660123.svg?logo=discord&logoColor=white&logoWidth=20&labelColor=7289DA&label=Discord&color=17cf48)](https://discord.gg/FNbZMr7p9d)

## Xyris
Xyris aims to become a fully-fledged microkernel with an emphasis on usability and security.

## Project Structure

The `xyris-redux` repository is effectively split into two projects, both with different sets of assumptions and
requirements. They are as follows:

- `Modules`
    - Freestanding kernel code broken up into separate components that all combine to create a single, functional kernel
      and operating system.
        - `Loader`: Bootloader-to-kernel translation code and early bootstrapping
            - Only code required to bootstrap the kernel and abstract bootloader-specific information should go here
        - `Kernel`: The kernel itself
            - Broken up into multiple different components within the `Modules` directory that create a single cohesive kernel
            - Most code should go here. Examples include:
            - Additional / complex language runtimes (e.g., C++ pure virtual handler, static global guards, etc.)
            - Memory management
            - Scheduler
            - etc.
        - `Runtime`: Runtime required by compiled languages
            - This should only contain the minimal set of code required by both `Loader` and `Kernel`
            - This is subject to change and might be rolled into `Loader` and `Kernel` individually
- `Tools`
    - Tools and other components that are **not** freestanding and use the host's compiler
    - These tools and other components are used for validating and testing the components defined in `Modules`

### License
Xyris is available under the MIT license but uses third party source code which may have different licenses.  
See `LICENSE.md` for details.  

## Third Party Projects
* [Limine Bootloader](https://github.com/limine-bootloader/limine)
