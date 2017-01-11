set title ""
set xlabel 'Number of malloc'
set ylabel 'Average allocation frequency per block'
set yrange [1.2:2.15]
set key box
set terminal png size 800,500
set output 'eval_3_1024.png'

plot 'eval_3_1024.res' u 1:2 w lp pt 5 title "Wamalloc",\
'eval_3_1024.res' u 1:3 w lp pt 7 title "NVMalloc"
