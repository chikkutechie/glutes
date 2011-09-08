glutes is a trimmed version of glut (GL Utility Toolkit).



How to build

============


Symbian

*******

    1)  Go to build/Symbian
    2)  Use sbs to build for different platforms (winscw_udeb, armv5_urel or armv5_urel_gcce)


Linux

*****

    1)	Go to build/linux

    2)	Rum make



Maemo

*****

    In maemo, on device building is only tried. To do this g++ is required togather with needed X11 include files.

    If libX11.so it not available a soft link might need to be created which points to appropriate library(say libX11.so.6)


    1) go to build/maemo

    2) Run make

