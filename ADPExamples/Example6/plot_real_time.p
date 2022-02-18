set term x11 1

set yrange [0:5]
set xrange [0:50]

plot    'history.dat' u 1:2 w l, \
        'history.dat' u 1:3 w l

set term x11 2

#set yrange [0:100]
#
#plot    'history.dat' u 1:4 w l, \
#        'history.dat' u 1:5 w l

pause 0.1
reread
