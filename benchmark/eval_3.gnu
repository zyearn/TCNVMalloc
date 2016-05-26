set title ""
set xlabel 'Numbers of malloc'
set ylabel 'Average allocation frequency per block'
set yrange [1.2:2.15]
set key box
set terminal png
set output 'eval_3.png'

plot 'eval_3.res' u 1:2 w lp pt 5 title "wamalloc",\
'eval_3.res' u 1:3 w lp pt 7 title "NVMalloc"