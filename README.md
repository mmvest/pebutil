## About The Project

pebutil is a small utility header which gives your code access to functions for interacting with not only the calling process's Process Environment Block (PEB), but also the PEB of other processes. As of right now, the utility functions can:
* Get the address of the PEB for the calling process or a target process
* Copy the PEB of the calling process or a target process.
<br>
<br>

## Getting Started

To use the utility functions in your project, first download or clone the git repo:
```sh
git clone https://github.com/mmvest/pebutil.git
```
Now copy the pebutil.h/.c files from the repo into whatever project you need it. You can then include it using:
```C
#include "pebutil.h"
```
After including the header in your project, you will need to make sure you link these files when you compile.
<br>
<br>
See [Usage](#usage) for examples of how to use the utility functions.
<br>
<br>

### Prerequisites

* Windows 10, 64-bit
* C Compiler of Choice

Note that currently this only supports 64-bit applications. 
<br>
<br>

## Usage

Using the utility functions is as simple as including the header and then calling the functions. For example:
```c
#include <stdio.h>
#include "pebutil.h"
...
PPEB curProcPEB = GetCurrentPebAddress();
printf("Address of PEB: %p\n", curProcPEB);
...
```
_View [main.c](main.c) for an example usage of each utility function._
<br>
<br>

### Demo ([main.c](main.c))
To use the demo, first compile `main.c` using your compiler of choice. For example, if you are using `gcc`, from the repo root directory you can run the following command:
```sh
gcc main.c pebutil.* -o main.exe
```
This should produce an executable named `main.exe` which you can then run using:
```sh
main.exe <pid of target process>
```
The demo executable will simply print some information about the PEBs of the target process and the calling process.
<br>
<br>


## Future Development
As of right now, copying the PEB from a target process is a bit useless as the PEB contains a lot of pointers. With the pointers, you can do things like use ReadProcessMemory() to gain access to their contents which is still somewhat useful, but perhaps in the future I may come up with a way to do a complete copy and include the structs that those addresses point to. I would also like to add 32-bit support which shouldn't be too hard (I believe there is only one function that would need some additional code).
<br>
<br>

## License and Disclaimer

Use this in whatever project however you want. Note that this module uses techniques that may throw red flags for software such as anti-cheats and anti-virus programs. The code provided is for educational and ethical purposes and is not intended to be malicious in any way. With that said, I am not responsible and assume no liability for any misuse or damage caused by the concepts found in or the use of this code. 
<br>
<br>

## References
1) https://www.geoffchappell.com/studies/windows/km/ntoskrnl/inc/api/pebteb/peb/index.htm
2) https://blog.benoitblanchon.fr/getprocaddress-like-a-boss/
3) https://learn.microsoft.com/en-us/windows/win32/api/