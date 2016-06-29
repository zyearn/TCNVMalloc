set title ""
set xlabel 'Number of malloc'
set ylabel 'Physical memory consumptiom(K)'
# set yrange [1.6:2.1]
set key box
set key top left
set terminal png size 800,500
set output 'eval_2.png'

plot 'eval_2.res' u 1:2 w lp pt 5 title "Wamalloc",\
'eval_2.res' u 1:3 w lp pt 7 title "NVMalloc"
