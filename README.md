<h1>Cplorer</h1>
<p>
    Cplorer is a console based file explorer for Linux and Windows.
    This file explorer is developed in C++ natively for Windows and Linux.
</p>

<h1>Building</h1>
<p>For UNIX like systems the building process is rather straight forward. The buildsystems is based on cmake and necesarry for Windows and Linux.</p>

<h2>Dependencies</h2>
<h3>Ubuntu</h3>

```
sudo apt-get install libncurses5-dev libncursesw5-dev libboost-all-dev
```
<h3>Arch</h3>

```
sudo pacman -S libncurses5-dev libncursesw5-dev boost
```
<h3>Windows</h3>
For Windows the ncurses library has to be compiled and added e.g. 
to the lib-folder of the mingw compiler.

Link to [PDCurses](https://github.com/wmcbrine/PDCurses)
