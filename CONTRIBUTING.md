# Contributing to Whoa

Thanks for making a contribution to Whoa!

## Whoa is 3.3.5a (build 12340)

Since Whoa is a faithful implementation of the original 3.3.5a (build 12340) client, there are some important considerations to keep in mind regarding accuracy:

* Do not assume behavior present in another version of the client is present in 3.3.5a (build 12340). Always verify 3.3.5a (build 12340) contains the behavior before trying to contribute it.

* Please use disassemblers and debuggers with 3.3.5a (build 12340) to determine original layouts and behaviors.

* Make every effort to maintain equivalent function signatures, function behaviors, class names, class layouts, structs, and enums as the original client. When an original name is not known, attempt to use names based on behavior. If behavior is not known, attempt to use names based on offsets.

* Occasionally, original layouts or behaviors must be changed to permit compilation on modern 64-bit systems. When behaviors differ for compatibility, the original behavior should be documented, and the variance explained.

* Even if known, original names and paths of source code files are not part of Whoa's faithful implementation.

## Commits and Pull Requests

* Keep commits formatted according to [the Conventional Commits standard](https://www.conventionalcommits.org/en/v1.0.0/):
  - Example of a well-formed commit headline: `feat(net): implement SRP6 client proof calculation`
  - Every commit headline (the first line of the message) should begin with a type of `feat`, `fix`, or `chore`.
  - Every type should be followed by the impacted subsystem of Whoa: `app`, `client`, `event`, `glue`, `gx`, `net`, `ui`, etc.
  - Commit headlines should be no longer than 72 characters.
  - If more descriptive text is required, please confine it to the commit description after the headline.
  - Separate the headline and description by one blank line.

* Keep pull requests tightly scoped to make reviews easier. Think: smaller pull requests, more of them. If a PR touches more than a small handful of files or exceeds a few hundred lines in length, please break the PR into multiple distinct PRs.

* Title pull requests like commit messages.

## Code Style

* Follow formatting and style guidelines per the rules defined in `.clang-format` and `.editorconfig`. Ideally, configure your IDE to use both files.

* Use fixed-width integer types like `int8_t`, `uint8_t`, `int32_t`, `uint32_t`, etc. instead of ambiguous-width types like `int`.

* When performing pointer arithmetic, use pointer-appropriate numeric types like `intptr_t` and `uintptr_t`.

* Do not use `NULL`.
  - For pointers, use `nullptr`.
  - For string termination, use `\0`.
  - For zero, use `0`.

* Use complete include paths from root of `src` directory when including local headers:
  - Examples:
    ```cpp
    #include "event/Event.hpp"
    #include "util/foo/Bar.hpp"
    ```

* Use include paths in `<brackets>` when including headers from system libraries, submodules, or vendor directories:
  - Examples:
    ```cpp
    #include <cstdint>
    #include <storm/String.hpp>
    ```

* Guard against multiple includes of headers using path-appropriate `#define`s:
  - Example in a file like `util/foo/Bar.hpp`
    ```cpp
    #ifndef UTIL_FOO_BAR_HPP
    #define UTIL_FOO_BAR_HPP
    
    // Header here
    
    #endif
    ```

## Submodules

* Implementations of classes, templates, and functions that may have been present in Storm [should be added to squall](https://github.com/whoahq/squall), the Storm implementation for Whoa.

* Implementations of classes, templates, and functions that may have been present in Tempest [should be added to typhoon](https://github.com/whoahq/typhoon), the Tempest implementation for Whoa.
