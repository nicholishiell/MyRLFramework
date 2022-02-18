set term x11 1

set yrange [0:12]

plot    'history.dat' u 1:2 w l, \
        'history.dat' u 1:3 w l, \
        'history.dat' u 1:4 w l, \
        'history.dat' u 1:5 w l

set term x11 2

set yrange [0:2000]

plot    'history.dat' u 1:6 w l, \
        'history.dat' u 1:7 w l, \
        'history.dat' u 1:8 w l, \
        'history.dat' u 1:9 w l


set term x11 3

set yrange [0:1100]

plot 'valueHistory.dat' u 1:2 w l

pause 0.1
reread
