require 'sinatra'
require 'sinatra/reloader' if development?

get '/' do
slim :home

end

get '/about' do
@title = "All About This Website"
slim :about
end

get '/contact' do
slim :contact
end

not_found do
slim :not_found
end

get '/fake-error' do
status 500
"Status Code 500: \n\n 	\t It's ok"
end
