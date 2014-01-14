set terminal png
set output "grafica2.png"
set title "Trabajo Simulación Redes. Grupo 5"

set xlabel "Tasa de errores"
set ylabel "Uso del enlace (%)"

set xrange [0:0.4]
set yrange [0:50]

plot "trabajo_final_errores1.dat" title "uso_enlace - tasa_error " with yerrorlines

pause -1