#!/usr/bin/ruby
$LOAD_PATH << '.'
require "support"


# Module defined in trig.rb file "estw oti"
module Trig
   PI = 3.141592654
   def Trig.sin(x)
	   print "yeah this is trig sin",x,"\n"
   end
   def Trig.cos(x)
	   print "yeah this is trig cos",x,"\n"
   end
end

print "hello, sin(5)",Trig.sin(5),"\n"


# Module defined in moral.rb file "estw oti"

module Moral
   VERY_BAD = 0
   BAD = 1
   def Moral.sin(badness)
   # ...
   end
end



class Decade
include Week
   no_of_yrs=10
   def no_of_months
      puts Week::FIRST_DAY
      number=10*12
      puts number
   end
end
d1=Decade.new
puts Week::FIRST_DAY
Week.weeks_in_month
Week.weeks_in_year
d1.no_of_months

module A
   def a1
	return "a1 "
   end
   def a2
	return "a2 "
   end
end
module B
   def b1
	return "b1 "
   end
   def b2
	return "b2 "
   end
end

class Sample #Mixin!
include A
include B
   def s1
	return "s1 "
   end
end

samp=Sample.new
print samp.a1,samp.a2,samp.b1,samp.b2,samp.s1


