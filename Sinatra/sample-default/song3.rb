require 'sequel'
require 'rubygems'
require 'sinatra'
require 'sinatra/sequel'
require 'sqlite3'

DB= Sequel.connect('sqlite://4sbase.db')


	DB.create_table?(:artists) do
  		primary_key :id
  		String  :name
  		String	:artist_pic
  		
	end
	DB.create_table?(:albums) do
 		primary_key :id
  		foreign_key :artist_id, :artists
  		String 		:title
  		Integer		:year
  		String		:album_pic
	end

			
class Album<Sequel::Model
  many_to_one :artist
end
class Artist<Sequel::Model
  one_to_many :albums
end


get '/albums' do
	#@albums = Album.all
	@ds1=Album.order(:title).select(:title,:id)
	slim :albums
end

get '/albums/new' do
	@album=Album.new
	@artists=Artist.all
	slim :new_album #POST->/albums   , params: the elements of the album.
end

get '/albums/:id' do
#i methodos  get dinei id.
#i methodos  where dinei to zitoumeno object.
	@album = Album.get(params[:id])#vres to album ID
	@ds1=Album.where(:id =>@album)#me to ID vres to album
	@ds2=Artist.where(:id => @ds1.get(:artist_id))#me ayto 
													#to album vres ton artist tou
	slim :show_albums
end

get '/albums/:id/edit' do
@album = Album.get(params[:id])
#@ds1=Album.where(:id =>@album)
@album2=Album[:id=>@album]
@artists=Artist.all
slim :edit_album
end

post '/albums' do
album=Album.create(params[:album])
redirect to("/albums/#{album.id}")
end

put '/albums/:id' do
album = Album.get(params[:id])
Album[:id=>album].update(params[:album2])
redirect to("/albums/#{album}")
end

get '/artists' do
#@artists=Artist.all
@ds1=Artist.order(:name).reverse.select(:name,:id)
slim :artists
end

get '/artists/new' do
@artist=Artist.new
slim :new_artist
end

get '/artists/:id' do
@artist=Artist.get(params[:id])
@ds1=Artist.where(:id =>@artist)
#@results=ds1.get(:name)
@albums=Album.where(:artist_id => @artist).all
slim :show_artists
end


post '/artists' do
artist=Artist.create(params[:artist])
redirect to("/artists/#{artist.id}")
end

get '/artists/:id/edit' do
@artist =Artist.get(params[:id])
@artist2=Artist[:id=>@artist]
slim :edit_artist
end

put '/artists/:id' do
artist = Artist.get(params[:id])
Artist[:id=>artist].update(params[:artist2])
redirect to("/artists/#{artist}")
end

delete '/albums/:id' do
album=Album.get(params[:id])
Album.where(:id=>album).destroy
redirect to('/albums')
end

delete '/artists/:id' do
artist=Artist.get(params[:id])
Artist.where(:id=>artist).destroy
redirect to('/artists')
end

get '/albums/:id/upload' do
@id=Album.get(params[:id])
@album=Album.where(:id=>@id)
slim :album_upload
end

get '/albums/:id/nophoto' do
@we="test"
slim :no_photo
end


get '/artists/:id/nophoto' do
slim :no_photo
end

post '/albums/:id/upload' do
id=Album.get(params[:id])
unless params[:myfile] && (tmpfile = params[:myfile][:tempfile]) && (name = params[:myfile][:filename])
	redirect to("/albums/#{id}/nophoto")
end

filename=params['myfile'][:filename]
File.open('public/uploads/' + params['myfile'][:filename], "w") do |f|
    f.write(params['myfile'][:tempfile].read)
    
  end

  photo_name=File.basename('public/uploads/' + params['myfile'][:filename],'.*')    
new_file=File.open('public/uploads/' + "#{photo_name}.txt","w"){|f|

time=f.ctime
f.write("#{time}")
f.write("hello world")}
Album[:id=>id].update(:album_pic=>filename)  

redirect to("/albums/#{id}")
end

get '/artists/:id/upload' do
@id=Artist.get(params[:id])
@artist=Artist.where(:id=>@id)
slim :artist_upload
end

post '/artists/:id/upload' do
id=Artist.get(params[:id])
#filename=params['myfile'][:filename]
unless params[:myfile] && (tmpfile = params[:myfile][:tempfile]) && (name = params[:myfile][:filename])
	redirect to("/artists/#{id}/nophoto")
end	
	filename=params['myfile'][:filename]
	File.open('public/uploads/' + params['myfile'][:filename], "w") do |f|
    	f.write(params['myfile'][:tempfile].read)

	end

	photo_name=File.basename('public/uploads/' + params['myfile'][:filename],'.*')    
	new_file=File.open('public/uploads/' + "#{photo_name}.txt","w"){|f|

	time=f.ctime
	f.write("#{time}")
	f.write("hello world")}


	Artist[:id=>id].update(:artist_pic=>filename)  

	redirect to("/artists/#{id}")


 

end
