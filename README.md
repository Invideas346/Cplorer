# Cplorer
Cplorer is a console based file explorer for Linux and Windows.
This file explorer is developed in C++ natively for Windows and Linux.

# Building
For unix like systems the building process is rather straight forward. The buildsystems is based on cmake and necesarry for Windows and Linux.

## Dependencies
##### Ubuntu
```
sudo apt-get install libncurses5-dev libncursesw5-dev libboost-all-dev
```
##### Arch
```
sudo pacman -S libncurses5-dev libncursesw5-dev boost
```
##### Windows
For Windows the ncurses library has to be compiled and added e.g. 
to the lib-folder of the mingw compiler.

Link to [PDCurses](https://github.com/wmcbrine/PDCurses)
