#!/bin/bash

time ../bin/landscape --size 512 --output ../output/1.map --seed 1 --rivers_number 10 --river_width 3 --roughness 0.2 --height 5 --out_height 0 --algorithm diamond_square && ruby landscape.rb output/1.map output/diamond_square.png
#time ./landscape --size 512 --output output/1.map --seed 1 --roughness 0.2 --algorithm hill_algorithm --noise --islands 4 && ruby landscape.rb output/1.map output/hill_algorithm.png
#time ./landscape --size 512 --output output/1.map --seed 1 --persistence 0.7 --frequency 0.01 --amplitude 0.25 --algorithm perlin_noise && ruby landscape.rb output/1.map output/perlin_noise.png
