#Dynamic site with relational database
#Stores songs and artists


require 'sinatra'
require 'rubygems'
require 'sass'
require 'slim'
require 'sequel'
require 'sinatra/sequel'
require './song3'

get '/styles.css' do
 scss :styles 
end

get '/' do
	slim :home
end

#get '/about' do
#	@title = "All About This Website"
#	slim :about
#end

get '/contact' do
	@title = "Contact Info"
	slim :contact
end


not_found do
	slim :not_found
end

