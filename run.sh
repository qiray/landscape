#!/bin/bash

time ./landscape --size 512 --output 1.txt --roughness 0.2 --height 5 --out_height 0 --algorithm diamond_square && ruby landscape.rb 1.txt output/diamond_square.png
time ./landscape --size 512 --output 1.txt --roughness 0.2 --algorithm hill_algorithm --noise --islands 4 && ruby landscape.rb 1.txt output/hill_algorithm.png
time ./landscape --size 512 --output 1.txt --persistence 0.7 --frequency 0.01 --amplitude 0.25 --algorithm perlin_noise && ruby landscape.rb 1.txt output/perlin_noise.png
