months = Hash.new
months = Hash.new( "month" )

months = Hash.new "month"


months = Hash.new( "month" )

puts "#{months[0]}"
puts "#{months[72]}"


H = Hash["a" => 100, "b" => 200]

puts "#{H['a']}"
puts "#{H['b']}"

Y=Hash[  [1,"jan"] => "January" ,  [2,"feb"] => "February" ]

print Y[[1,"jan"]]
puts "#{Y[[1,"jan"]]}"

months=Y
keys = months.keys

puts "#{keys}"


init_hash =Hash['a'=>1,'b'=>2 ]

my_hash=init_hash
print my_hash ,"\n"

my_hash.store('c', 3) #a way to add a new hash value
print my_hash

my_hash['d']=4 #another way to add a new hash value
print my_hash,"\n"
my_hash['d']=45 #another way to add a new hash value
print my_hash,"\n"


