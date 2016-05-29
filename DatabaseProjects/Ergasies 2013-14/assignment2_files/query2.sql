#epilegoume eponymo skinotheti kai titlo tainias
select DIR.last_name , MOV.title

from director as DIR , movie as MOV,movie_has_director as MHD
#opou: i tainia lirei tis eksis proipotheseis:
where  MOV.movie_id IN (	#paizei se aytin enas ithopoios me eponymo Allen
						select ROL.movie_id
						from actor as ACT,role as ROL
						where 	ACT.last_name="Allen"
							and ACT.actor_id=ROL.actor_id 
) 
# kai o skinothetis tis exei gyrisei ki alla eidi.
		and(select COUNT(DISTINCT MHG.genre_id)
			from 
				movie_has_director as MHD,movie_has_genre as MHG
			where 	DIR.director_id=MHD.director_id
				and MHD.movie_id=MHG.movie_id)>1

		and DIR.director_id=MHD.director_id
		and MHD.movie_id=MOV.movie_id;