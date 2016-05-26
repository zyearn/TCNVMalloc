set title ""
set xlabel 'Number of thread'
set ylabel 'Average time in us'
# set yrange [-0.5:3]
set key box
set key top left
set terminal png
set output 'eval_6.png'

plot 'eval_6.res' u 1:2 w lp pt 5 title "wamalloc",\
'eval_6.res' u 1:3 w lp pt 7 title "NVMalloc",\
'eval_6.res' u 1:4 w lp pt 9 title "glibc"