reset
set title "Sort Performance"
set xlabel "Number of Elements"
set ylabel "Time (nanoseconds)"
set xtics 5000
set xtics font "Times_New Roman,10"
set terminal png font "Times_New_Roman,12"
set output "sort_compare.png"

plot "out_ksort.csv" using 1:2 with linespoints title "ksort", \
     "out_l_sort.csv" using 1:2 with linespoints title "l_sort"
