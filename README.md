Caps Hotkeys V2
===============

A program that remaps the `Caps Lock` key to useful key bindings

Compilation
-----------

To compile the program, simply use any C++ compiler. Unfortunately, for the time being, it doesn't work on Linux since it uses Windows-specific API functions. Here's an example using `G++`:

```shell
g++ '.\Caps Hotkeys V2.1.cpp' -o '.\Caps Hotkeys V2.1.exe'
```

Functionality
-------------

Below is a list of all key bindings the program implements. Note that if a binding is not defined for a certain key, the program will press `Ctrl + key` instead. For example, typing in `CAPS + S` will save the current document, just as if `CTRL + S` was pressed.

* `CAPS + u`:  CTRL + left arrow
* `CAPS + o`:  CTRL + right arrow
* `CAPS + i`:  up arrow
* `CAPS + j`:  left arrow
* `CAPS + k`:  down arrow
* `CAPS + l`:  right arrow
* `CAPS + m`:  home
* `CAPS + .`:  end
* `CAPS + [`:  scroll up
* `CAPS + '`:  scroll down
* `CAPS + q`:  escape
* `CAPS + ,`:  switch apps
* `CAPS + BS`: del
* `CAPS + d`:  SHIFT
* `CAPS + b`:  Win
* `CAPS + ?`:  CTRL + ?
