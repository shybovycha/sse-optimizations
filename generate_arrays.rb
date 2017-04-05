#!/usr/bin/ruby
dest_dir = ARGV[0] || '.'

File.write(File.join(dest_dir, 'float_array.h'), "float fa[] = #{((1..1_000_000).map {|_| rand()}).to_s.tr('[]', '{}')};")

File.write(File.join(dest_dir, 'int_array.h'), "int ia[] = #{((1..1_000_000).map {|_| rand(100)}).to_s.tr('[]', '{}')};")
