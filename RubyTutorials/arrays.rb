#!/usr/bin/ruby
names = Array.new
names = Array.new(20)

puts names.size  # This returns 20
puts names.length # This also returns 20

names = Array.new(4, "mac")
puts "#{names}"
print b=Array.new(3,'default')

nums = Array.new(10) { |e| e = e * 2 }

puts "#{nums}"

nums = Array.[](1, 2, 3, 4,5)

nums = Array[1, 2, 3, 4,5]

a=[1,3,5]

digits = Array(0..9)

puts "#{digits}"
b=Array['1','a',2,a]#eterogenes array!-exw kai nested arrays!
print b

print b=Array.new(3,'default')
a=[1,2]
b=[2,3]
print c=a&b
print d=c+a
