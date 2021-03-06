require 'unobservable'

def get str="/"
print "got!\n"
end

def get str="/more"
print "got more!\n"
end

get



class Button
  include Unobservable::Support

  attr_event :clicked
  attr_event :double_clicked

  def click(x, y)
    raise_event(:clicked, x, y)
  end

  def double_click(x, y)
    raise_event(:double_clicked, x, y)
  end
end

button  = Button.new
button.clicked.register {|x, y| puts "You just clicked: #{x} #{y}"}

button.click(2, 3)

