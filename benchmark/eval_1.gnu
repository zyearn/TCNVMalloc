set title ""
set xlabel 'Number of malloc'
set ylabel 'Average allocation frequency per block'
set yrange [1.9:2.05]
set key box
set terminal png size 800,500
#set terminal png
set output 'eval_1_1024.png'

plot 'eval_1_1024.res' u 1:2 w lp pt 5 title "Wamalloc",\
'eval_1_1024.res' u 1:3 w lp pt 7 title "NVMalloc"
