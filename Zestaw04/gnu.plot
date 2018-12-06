set term pngcairo background "#171111" size 1200,900

########################################
# line styles for ColorBrewer Pastel2
# author: Anna Schneider

# line styles
set style line 1 lt 1 lc rgb '#B3E2CD' # pale teal
set style line 2 lt 1 lc rgb '#FDCDAC' # pale orange
set style line 3 lt 1 lc rgb '#CDB5E8' # pale lilac
set style line 4 lt 1 lc rgb '#F4CAE4' # pale magenta
set style line 5 lt 1 lc rgb '#D6F5C9' # pale lime green
set style line 6 lt 1 lc rgb '#FFF2AE' # pale banana
set style line 7 lt 1 lc rgb '#F1E2CC' # pale tan
set style line 8 lt 1 lc rgb '#CCCCCC' # pale grey

# palette
set palette maxcolors 8
set palette defined ( 0 '#B3E2CD',\
		1 '#FDCDAC',\
		2 '#CDB5E8',\
		3 '#F4CAE4',\
		4 '#D6F5C9',\
		5 '#FFF2AE',\
		6 '#F1E2CC',\
		7 '#CCCCCC' )
########################################

set style line 11 lc rgb "#AA9999" lt 1 lw 1
set style line 12 lc rgb "#443333" lt 1 lw 1

set key top left textcolor linestyle 11
set xrange [0:*]
set yrange [-1:10]
set ytic 0,2
set grid ls 12

set border linewidth 0 ls 12
set ytics textcolor linestyle 11
set xtics textcolor linestyle 11 rotate
set xlabel tc ls 11 offset 0,-3
unset xzeroaxis

set style data linespoints
set datafile separator ","
set key autotitle columnhead
plot for [i=2:*] "result.txt" using 1:i:xtic(1)