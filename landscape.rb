#ruby landscape.rb 1.txt 1.png
require 'chunky_png'

def intToColor(color)
	if color < -40
		return ChunkyPNG::Color.rgba(0, 0, 32, 255)
	elsif color < -10
		return ChunkyPNG::Color.rgba(0, 0, 64 + color/2,255)
	elsif color < 1
		return ChunkyPNG::Color.rgba(0, 0, 128,255)
	elsif color < 10 #sand
		return ChunkyPNG::Color.rgba(255, 255, 180 - color*5, 255)
	elsif color < 25 #grass
		return ChunkyPNG::Color.rgba(0, 128 - color*4, 0,255)
	elsif color < 60 #brown #8B 45 13
		return ChunkyPNG::Color.rgba(139 - color*2, 69 - color, 20 - color/3,255)
	elsif color < 100 # white
		return ChunkyPNG::Color.rgba(155 + color, 155 + color, 155 + color,255)
	end
	return ChunkyPNG::Color.rgba(255,255,255,255)
end

def makePNG(filename)
	f = File.open(filename, "r")
	size = f.readline.to_i
	len = size*size
	a = Array.new(len)
	png = ChunkyPNG::Image.new(size, size, ChunkyPNG::Color::TRANSPARENT)
	size.times do |i| 
		strings = f.readline.split(' ')
		size.times do |j| 
			png[j,i] = intToColor(strings[j].to_i)
		end
	end 
	return png.to_blob;
end

File.open(ARGV[1], "wb+") do |f|
	f.write(makePNG(ARGV[0]))
end
