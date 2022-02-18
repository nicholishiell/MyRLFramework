set title "Optimal Policy"

set cbrange [-1:8]
set palette defined (-1 "black", 0 "dark-red", 1 "red", 2 "orange", 3 "yellow", 4 "magenta", 5 "dark-blue" , 6 "cyan" , 7"blue" ,8"dark-green" ,9"green")

set xlabel "# of Critical"
set ylabel "# of Non-Critical"

set xrange [-5:100]
set yrange [-5:100]

set term x11 1

plot "learnedPolicy.dat" u 2:1:3 w image
replot

set term x11 2
plot "learnedPolicy_100m_e_5.dat" u 2:1:3 w image
replot


