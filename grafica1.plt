set terminal png
set output "grafica1.png"
set title "Trabajo Simulación Redes. Grupo 5"

set xlabel "Número de usuarios"
set ylabel "Uso del enlace (%)"

set xrange [1:20]
set yrange [0:50]

plot "trabajo_final1.dat" title "uso_enlace - usuarios" with yerrorlines

pause -1
