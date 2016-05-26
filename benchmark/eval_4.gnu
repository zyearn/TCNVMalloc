set title ""
set xlabel 'Number of malloc'
set ylabel 'Physical memory consumptiom(K)'
# set yrange [1.6:2.1]
set key box
set key top left
set terminal png
set output 'eval_4.png'

plot 'eval_4.res' u 1:2 w lp pt 5 title "wamalloc",\
'eval_4.res' u 1:3 w lp pt 7 title "NVMalloc"
