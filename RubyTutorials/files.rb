#!/usr/bin/ruby

val1 = "This is variable one"
val2 = "This is variable two"
puts val1
puts val2

puts "\n\n\n\n\n\n\naaaa\t\t"  # with " " it recognizes the \n, \t
puts '\n\n\n\n\n\n\naaaa\t\t'  # with ' ' it doesn't.

print "\ngets: waiting for input:"
a=gets
puts a

=begin
 The print statement is similar to the puts statement. 
 The only difference is that the puts statement goes to the
 next line after printing the contents, whereas with the 
 print statement the cursor is positioned on the same line.
=end

print "Hello World"
print "Good Morning"


aFile = File.new("input.txt", "a+")
if aFile
   content=aFile.sysread(120)
   unless content==nil
	puts content	
   end   
else
   puts "Unable to open file!"
end

if aFile
   aFile.syswrite("A^+%\n")
else
   puts "Unable to open file!"
end


arr = IO.readlines("input.txt")
puts arr[0]
puts arr[1]
arr.each do |i|
	print i
end

IO.foreach("input.txt"){|block| puts block}

# Rename a file from test1.txt to test2.txt

if File::exists?( "test1.txt" ) 
	File.rename( "test1.txt", "test2.txt" )
end

if File::exists?( "test1.txt" )
# Delete file test2.txt
	File.delete("text1.txt")
end

file = File.new( "test.txt", "w" )
file.chmod( 0755 )

File.open("file.rb") if File::exists?( "file.rb" )


File.file?( "text.txt" )


# a directory
print File::directory?( "/usr/local/bin" ) # => true

# a file
print File::directory?( "test.txt" ) # => false



print "test.txt Readable?", File.readable?( "test.txt" )   ,"\n"# => true
print "test.txt Writeable?", File.writable?( "test.txt" )   ,"\n"# => true
print "test.txt Executable?", File.executable?( "test.txt" ) ,"\n"# => false

print "test.txt zero?", File.zero?( "test.txt" )      ,"\n"# => true

print "test.txt Size?", File.size?( "test.txt" )     ,"\n"# => 1002

print "test.txt Filetype?", File::ftype( "test.txt" )    ,"\n" # => file


print "ctime of test.txt: ",File::ctime( "test.txt" ),"\n"
print "mtime of test.txt: ",File::mtime( "test.txt" ),"\n"
print "atime of test.txt: ",File::atime( "test.txt" ),"\n"

