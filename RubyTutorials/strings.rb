myStr = String.new("THIS IS TEST")
foo = myStr.downcase

puts "#{foo}"
myStr+="%d"
myStr % 10
print (myStr % 10)   #print a formatted String!

puts "\n"

a="%05d" % 123

b="%-5s: %08x" % [ "ID", self.object_id ]

c="foo = %{foo}" % { :foo => 'bar' }

print a,"\n",b,"\n",c,"\n"
