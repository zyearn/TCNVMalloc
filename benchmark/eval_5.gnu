set title ""
set xlabel 'Number of threads'
set ylabel 'Average time(us)'
set yrange [-0.5:3]
set key box
set terminal png size 800,500
set output 'eval_5.png'

plot 'eval_5.res' u 1:2 w lp pt 5 title "wamalloc",\
'eval_5.res' u 1:3 w lp pt 7 title "NVMalloc",\
'eval_5.res' u 1:4 w lp pt 9 title "glibc"
