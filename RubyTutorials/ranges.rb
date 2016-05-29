#!/usr/bin/ruby

# yparxoun kai RANGE objects.
$, =", "   # Array value separator
range1 = (1..10).to_a #to_a : to Array
range2 = ('bar'..'bat').to_a
range3 =(1..9).to_a
puts "#{range1}"
puts "#{range2}"
puts "#{range3}"

(1..5)        #==> 1, 2, 3, 4, 5
(1...5)       #==> 1, 2, 3, 4
('a'..'d')    #==> 'a', 'b', 'c', 'd'

 
# Assume a range
digits = 0..9

puts digits.include?(5)
ret = digits.min 
puts "Min value is #{ret}"

ret = digits.max
puts "Max value is #{ret}"

ret = digits.reject {|i| i < 5 }
puts "Rejected values are #{ret}"

digits.each do |digit|
   puts "In Loop #{digit}"
end


score = 70

result =case score
   when 0..40 
	 "Fail"
   when 41..60 
	"Pass"
   when 61..70 
	"Pass with Merit"
   when 71 .. 100 
	"Pass with Distinction"
   else 
	"Invalid Score"
end

puts result

if ((1..10) === 5)
  puts "5 lies in (1..10)"
end

if (('a'..'j') === 'c')
  puts "c lies in ('a'..'j')"
end

if (('a'..'j') === 'z')
  puts "z lies in ('a'..'j')"
end

 ((15+0)..(15+16)).each do |i|  #some hexadecimals!
print i.to_s(16),"\n"
end
