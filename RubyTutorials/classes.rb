#!/usr/bin/ruby
class Customer
   @@no_of_customers=0
end

cust1 = Customer.new
cust2 = Customer.new

class Customer1
   @@no_of_customer=0
   def initialize(id, name, addr)
      @cust_id=id
      @cust_name=name
      @cust_addr=addr
	@@no_of_customer+=1
   end
   def hello
      puts "Hello Ruby!"
   end
   def display_details()
      puts "Customer id #@cust_id"
      puts "Customer name #@cust_name"
      puts "Customer address #@cust_addr"
	puts"No#@@no_of_customer"
    end
end


cust3 = Customer1. new("1", "John", "Wisdom Apartments, Ludhiya")
cust4 = Customer1. new("2", "John2", "Wisdom Apartments2, Ludhiya2")
cust5 = Customer1. new("3", "John3", "Wisdom Apartment3s, Ludhiya3")

cust3.hello
cust3.display_details()

class Accounts #methods public by default.
   def reading_charge #instance method
   end
   def Accounts.return_date #class method
   end
end

class C
	def kalese_tin_prive
		C.prive()	
	end
	def C.prive()
		puts "Prive!"
	end
end

c=C.new
#c=C.prive 			#---> Den tha ti vrei! Einai prive!
c.kalese_tin_prive 		#twra malista!
#c=C.new.kalese_tin_prive 	#an thelw se mia mono entoli na to kanw


class DD
@@a
	def makenew()
		@gg=1#ftiaxnw ki apo akyres synartiseis kainouries instance variables
		@@a=1
	end
end
dd=DD.new()
print dd.makenew
print
