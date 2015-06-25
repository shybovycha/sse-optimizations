all: generate_arrays sum max

generate_arrays: generate_arrays.rb
	ruby generate_arrays.rb

max: max.cpp
	g++ -o max -msse4 max.cpp

sum: sum.cpp
	g++ -o sum -msse4 sum.cpp
