def test
   puts "You are in the method"
   yield
   puts "You are again back to the method"
   yield
end
test {puts "You are in the block"}


def test1
   yield 5
   puts "You are in the method test"
   yield 100
end
test1 {|i| puts "You are in the block #{i}"}

def test2(&block)
   block.call
end
test2 { puts "Hello World! from block 'test2'"}

def test3
   yield 5,"niaou"
   puts "You are in the method test"
   yield 100,"gab"
end
#test3 {|i,j| print "You are in the block ",i,j }
test3 {|i,j| puts "You are in the block #{i},#{j}" }

BEGIN { 
  # BEGIN block code 
  puts "BEGIN code block"
} 

END { 
  # END block code 
  puts "END code block"
}
  # MAIN block code 
puts "MAIN code block"
