# Roofus.c

Clear the table

# How to compile

Current version requires ncures (checkout the no-ncurses branch with
`git checkout no-ncurses`
if you don't want it).

Add the path to your LD_LIBRARY_PATH and then compile with the flag -lncurses

ubuntu example:

(as root)
apt-get install ncurses-dev

(compile)
`gcc ./Roofus.c -o roofus -lncurses`
