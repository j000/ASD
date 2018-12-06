set term pngcairo background "#171111" size 1200,900

########################################
# line styles for ColorBrewer Pastel2
# author: Anna Schneider

set linetype 1 lc rgbcolor '#B3E2CD' pt 1
set linetype 2 lc rgbcolor '#FDCDAC' pt 3
set linetype 3 lc rgbcolor '#CDB5E8' pt 5
set linetype 4 lc rgbcolor '#F4CAE4' pt 7
set linetype 5 lc rgbcolor '#D6F5C9' pt 9
set linetype 6 lc rgbcolor '#FFF2AE' pt 11
set linetype 7 lc rgbcolor '#F1E2CC' pt 13
set linetype 8 lc rgbcolor '#CCCCCC' pt 15

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

set style line 11 lt 1 lw 1 lc rgbcolor "#AA9999"
set style line 12 lt 1 lw 1 lc rgbcolor "#443333"

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
plot for [i=2:*] filename using 1:i:xtic(1)
