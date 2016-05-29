#!/usr/bin/env ruby
 
# Long explicit way:
result = []
(1...8).each do |n|
    if n > 5
        result << n*100
    end
end
p result
# result [600, 700]
 
# Concise way:
p [1,2,3,4,5,6,7].select{|n| n > 5}.map{|n| n*100}
# result [600, 700]
