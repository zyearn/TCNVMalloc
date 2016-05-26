set title ""
set xlabel 'numbers of malloc'
set ylabel 'average allocation time per block'
set yrange [1.9:2.1]
set key box
set terminal png
set output 'eval_1.png'

plot 'eval_1.res' u 1:2 w lp pt 5 title "wamalloc",\
'eval_1.res' u 1:3 w lp pt 7 title "NVMalloc"
