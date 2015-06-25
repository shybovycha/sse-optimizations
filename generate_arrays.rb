#!/usr/bin/ruby
File.write('float_array.h', "float fa[] = #{((1..1_000_000).map {|_| rand()}).to_s.tr('[]', '{}')};")
File.write('int_array.h', "int ia[] = #{((1..1_000_000).map {|_| rand(100)}).to_s.tr('[]', '{}')};")
