#!/usr/bin/ruby

begin
   file = open("/nonexistent_file")
   if file
      puts "File opened successfully"
   end
rescue
      file = STDIN
end
print file, "==", STDIN, "\n"


fname="asdasdasdasas3454325"
begin
   file = open(fname,"r")
   if file
      puts "File opened successfully"
   end
rescue
	puts "reached rescue section!"
   fname = "begin.rb"
   retry
end



begin
 # raise 'A test exception.'
 puts "I'm not raising exception"
rescue Exception => e
  puts e.message
  puts e.backtrace.inspect
else
   puts "Congratulations-- no errors!"
ensure
  puts "Ensuring execution"
end



begin  
    puts 'I am before the raise.'  
    raise 'An error has occurred.'  
    puts 'I am after the raise.'  
rescue  
    puts 'I am rescued.'  
end  
puts 'I am after the begin block.'  
