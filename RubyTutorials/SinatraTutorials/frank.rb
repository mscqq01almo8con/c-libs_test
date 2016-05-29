require 'sinatra'
require 'sinatra/reloader' if development?
get '/frank' do
name = "Frank2"
"Hello #{name}"
end
