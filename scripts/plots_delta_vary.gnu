set logscale y;
set logscale x;
CIRCLEPT = 6
TRIANGLEPT = 9
set style line 1 lc rgb 'black' lw 2 pt 5 ps 2  # square
set style line 2 lc rgb 'black' lw 2 pt 7 ps 2  # circle
set xlabel "Delta";
set xtics (20, 40, 80, 160, 320, 640, 1280, 2560, 5120);
set terminal png size 1200,800 enhanced font "Helvetica,20"


set title "Independent dataset" ;
set ylabel "Number of comparisons";
set output '../plots/ind_comps_delta_vary.png'
plot 'data1.dat' using 1:2 title "Indexed Two Pass" with linespoints ls 1,\
 'data1.dat' using 1:3   title "Two Pass" with linespoints ls 2;

set title "Correlated dataset" ;
set ylabel "Number of comparisons";
set output '../plots/cor_comps_delta_vary.png'
plot 'data2.dat' using 1:2 title "Indexed Two Pass" with linespoints ls 1,\
 'data2.dat' using 1:3   title "Two Pass" with linespoints ls 2;

set title "Anti-Correlated dataset" ;
set ylabel "Number of comparisons";
set output '../plots/ant_comps_delta_vary.png'
plot 'data3.dat' using 1:2 title "Indexed Two Pass" with linespoints ls 1,\
 'data3.dat' using 1:3   title "Two Pass" with linespoints ls 2;

set title "Independent dataset" ;
set ylabel "Time taken (ms)";
set output '../plots/ind_time_delta_vary.png'
plot 'data4.dat' using 1:2 title "Indexed Two Pass" with linespoints ls 1,\
 'data4.dat' using 1:3   title "Two Pass" with linespoints ls 2;

set title "Correlated dataset" ;
set ylabel "Time taken (ms)";
set output '../plots/cor_time_delta_vary.png'
plot 'data5.dat' using 1:2 title "Indexed Two Pass" with linespoints ls 1,\
 'data5.dat' using 1:3   title "Two Pass" with linespoints ls 2;

set title "Anti-Correlated dataset" ;
set ylabel "Time taken (ms)";
set output '../plots/ant_time_delta_vary.png'
plot 'data6.dat' using 1:2 title "Indexed Two Pass" with linespoints ls 1,\
 'data6.dat' using 1:3   title "Two Pass" with linespoints ls 2;

unset logscale y;
set yrange[0:100];
set ytics 10;
set title "Independent dataset" ;
set ylabel "Perecentage comparisons";
set output '../plots/ind_comps_per_delta_vary.png'
plot 'data1.dat' using 1:4 title "Indexed Two Pass" with linespoints ls 1;

set title "Correlated dataset" ;
set output '../plots/cor_comps_per_delta_vary.png'
plot 'data2.dat' using 1:4 title "Indexed Two Pass" with linespoints ls 1;

set title "Anti-Correlated dataset" ;
set output '../plots/ant_comps_per_delta_vary.png'
plot 'data3.dat' using 1:4 title "Indexed Two Pass" with linespoints ls 1;
