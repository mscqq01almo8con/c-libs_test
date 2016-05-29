#	Gia kathe zeygos genre_id,actor_id metrame tainies (count)
#oi opoies pliroun synolika ta parakato kritiria (sto where)
select distinct MHG.genre_id,ROL.actor_id,count(*) as c
from movie_has_genre as MHG,role as ROL
#	katarxin epilegoume ta eidi pou exoun tainies 
#stis opoies exei paiksei (kapoio rolo) o ithopoios
where MHG.movie_id=ROL.movie_id and 

/*episis theloume stis parapano tainies,
 na min yparxei tainia tou ithopoiou me ayto to eidos
, gia tin opoia: */
not exists (
		select MHD.movie_id,ROL.actor_id
		from movie_has_director as MHD
		/*o skinothetis pou gurise ayti tin tainia:*/
		where 	MHD.movie_id=MHG.movie_id
		and	MHD.director_id in 	(#-> na exei gyrisei ki alla eidi
						select distinct MHD.director_id
						from movie_has_genre as MHG,movie_has_director as MHD
						where MHD.movie_id=MHG.movie_id
						group by MHD.director_id
						having count(*)<>1)

		and MHD.director_id in (#->kai na exei gyrisei to sygkekrimeno eidos
						select distinct MHD.director_id
						from movie_has_genre as MHG1,movie_has_director as MHD
						where MHD.movie_id=MHG1.movie_id
						and MHG1.genre_id=MHG.genre_id)
		
		)
#omadopoioume me vasi to zitoumeno zeygos gia na athroisoume tis tainies.
group by MHG.genre_id,ROL.actor_id



