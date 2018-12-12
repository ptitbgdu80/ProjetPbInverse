# Output dans un fichier pdf
set terminal pdfcairo enhanced color font "Arial, 12" linewidth 2 fontscale 1.0 \
    size 10,6 # inches

set palette rgbformulae 7,5,15

set key

set output "solution.pdf"
plot "solution.txt" u 1:2:3 with image
