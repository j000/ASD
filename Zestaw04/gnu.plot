set term pngcairo background "#171111" size 1200,900 font 'Input Mono'

########################################
# line styles for ColorBrewer Set3
# author: Anna Schneider
set linetype 1 lc rgb '#8DD3C7' pt 0 # teal
set linetype 2 lc rgb '#FFFFB3' pt 5 # banana
set linetype 3 lc rgb '#BEBADA' pt 7 # lilac
set linetype 4 lc rgb '#FB8072' pt 9 # red
set linetype 5 lc rgb '#80B1D3' pt 11 # steel blue
set linetype 6 lc rgb '#FDB462' pt 13 # adobe orange
set linetype 7 lc rgb '#B3DE69' pt 15 # lime green
set linetype 8 lc rgb '#FCCDE5' pt 3 # mauve
########################################

set style line 11 lt 1 lw 1 lc rgbcolor "#AA9999"
set style line 12 lt 1 lw 1 lc rgbcolor "#443333"

set key top left textcolor linestyle 11
set grid ls 12

set border linewidth 0 ls 12
set ytics textcolor linestyle 11
set xtics textcolor linestyle 11 rotate
set xlabel tc ls 11 offset 0,-3
unset xzeroaxis

set style data linespoints
set datafile separator ","
set key autotitle columnhead

# second plot max y
max = 0.02
# second plot size
split = 0.4

# stats
stats filename name 'ALL' nooutput
array maxs[ALL_columns + 1]

do for [i=2:ALL_columns] {
	stats filename using i nooutput
	maxs[i] = STATS_max
}

set multiplot
set origin 0,split
set size 1,(1-split)

plot for [i=2:ALL_columns] filename using 1:i:xtic(1) title sprintf("%s (max: %7.4fs)",columnhead(i),maxs[i])

set origin 0,0
set size 1,split
set yrange [0:max]
unset xtics
set key off

plot for [i=2:ALL_columns] filename using 1:i:xtic(1)

unset multiplot
