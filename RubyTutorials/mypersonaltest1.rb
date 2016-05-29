
$positions={'gk'=>'Goalkeeper','df'=>'Defender','md'=>'Midfield','atk'=>'Attacker'}
$teams={'1'=>'Barca','2'=>'Mutd','3'=>'Real','4'=>'Dortmund'}
class Player
	@@playersNo
	@name
	@age
	@position
	@team
	@@isCoach
	MustBeOlderThan=18
	def initialize(name='John Doe',age=0,position='md')
		@name=name
		@age=age
		@position=position
		@@isCoach='NO'
	end

	def findposition
		$positions.each do |key, value|
			flag=0;
	   		if key==@position 
				print @name,"'s position is: " , value, "\n"
				flag=1
				unless key=='gk'
					print @name," is also not a Goalkeeper","\n"
				end
			end
			
		end
	end
	def isaCoach
		return @@isCoach
	end
	def ageQuery
		print "Age ",@age, "years old is:","\t"
		if @age<21
			print "We have a wonderkid in progress!"
		end
		
		case @age
		when 21 ... 27
			puts 'Normal age'
		when 28 ... 31	
			puts 'Golden Age!'
		else
			puts 'Too old goddammit!'
			
		end

		
	end
end
plyr1=Player.new('alx',23,'atk')
plyr1.findposition
plyr1.isaCoach
plyr1.ageQuery

def positioner(arg1)
	$positions.each do |key, value|
		if key==arg1
	 		print arg1," gives ",value,"\n"
		end
	end
 		print ""
end

print positioner('atk')
