require 'sinatra'
require 'sinatra/reloader' if development?

get '/' do
	"Hello Sinatra!"
end

get '/:name' do
	name = params[:name]
	"Hi there #{name}!"
end

get '/frank' do
	name = "Frank2"
	"Hello #{name}"
end

get '/:one/:two/:three' do
	"first: #{params[:one]}, second: #{params[:two]}, \
	third: #{params[:three]}"
end

get '/what/time/is/it/in/:number/hours' do
	number = params[:number].to_i
	time = Time.now + number * 3600
	"The time in #{number} hours will be #{time.strftime('%I:%M %p')}"
end

get '/bet/:stake/on/:number' do
	stake = params[:stake].to_i
	number = params[:number].to_i
	roll = rand(6) + 1
	if number == roll
		"It landed on #{roll}. Well done, you win #{6*stake} chips"
	else
		"It landed on #{roll}. You lose your stake of #{stake} chips"
	end
end
