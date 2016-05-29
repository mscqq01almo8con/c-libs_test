def jump(name,params={})
before=params[:before]||'before'#an yparxei symbolo-kleidi :before (='before') pare 
								#tin hash value stin opoia antistoixei, alliws xrisimopoiise tin 
								#default timi 'before'
after=params[:after]||'after'	#ta idia gia tin after.
p "#{name}, #{before},...,#{after}"
end

params=Hash.new() #ftiaxnw hash me key-symbolo before kai after opote i
				# synartisi jump tha ta xrisimopoiisei
params[:before]='lul'
params[:after]='more lulz'
print params
jump("me",params)

#twra kalw jump xwris hash
jump("me again")#tha mou bgalei tis default times

