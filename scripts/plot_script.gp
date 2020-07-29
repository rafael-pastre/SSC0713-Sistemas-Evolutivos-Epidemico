set term png
set output "./img/plot.png"
set title 'Melhor Modelo - AG'
set xrange [0:146]
set yrange [0:10000]
plot './data/output.csv' using 1:2 t'S', './data/output.csv' using 1:3 t'I', './data/output.csv' using 1:4 t'R', './data/output.csv' using 1:5 t'D'
