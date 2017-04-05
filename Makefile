all: basic advanced

basic: src/basics.cpp
	g++ -o build/basics -msse4 src/basics.cpp

advanced: generate_arrays sum max

generate_arrays: generate_arrays.rb
	ruby generate_arrays.rb build/

max: src/max.cpp
	g++ -o build/max -msse4 -Ibuild src/max.cpp

sum: src/sum.cpp
	g++ -o build/sum -msse4 -Ibuild src/sum.cpp
