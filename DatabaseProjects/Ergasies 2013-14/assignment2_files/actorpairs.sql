/*actorPairs: Η συνάρτηση αυτή δέχεται ως όρισμα τον κωδικό ενός ηθοποιού. Βρίσκει
τους ηθοποιούς ο καθένας από τους οποίους έχει παίξει σε διαφορετικά είδη ταινιών από
αυτόν αλλά που μαζί μ’ αυτόν έχει παίξει σε τουλάχιστον επτά διαφορετικά είδη.
Επιστρέφει το εξής:
○ Τους κωδικούς των ηθοποιών που πληρούν την παραπάνω συνθήκη.
Το ερώτημα 3 τροποποιείται (η bold λέξη είναι και η αλλαγή) σε:

Η συνάρτηση αυτή δέχεται ως όρισμα τον κωδικό ενός ηθοποιού. 
Βρίσκει τους ηθοποιούς ο καθένας από τους οποίους έχει παίξει 
μόνο σε διαφορετικά είδη ταινιών από αυτόν αλλά που μαζί μ’ αυτόν 
έχει παίξει σε τουλάχιστον επτά διαφορετικά είδη. Επιστρέφει το εξής:

    Τους κωδικούς των ηθοποιών που πληρούν την παραπάνω συνθήκη.

Όσοι φοιτητές έχουν ήδη υλοποιήσει το ερώτημα στην παλιά του μορφή μπορούν να το παραδώσουν κανονικά αρκεί να αναφέρουν είτε σε σχόλια στον κώδικα είτε σε κάποιο readme ποια έκδοση υλοποιούν.
*/

select distinct rol2.actor_id
from role as rol1,role as rol2,movie_has_genre as mhg1,genre as gen

where rol1.actor_id=933/*pare dedomeno ithopoio*/
and rol1.movie_id=mhg1.movie_id
and mhg1.genre_id=gen.genre_id 
and rol1.actor_id<>rol2.actor_id
and rol2.actor_id not in (select distinct r2.actor_id 
from role as r1,role as r2,movie_has_genre as mg1,movie_has_genre as mg2
where r1.movie_id=mg1.movie_id and r1.actor_id=rol1.actor_id
and mg2.movie_id=r2.movie_id and mg1.genre_id=mg2.genre_id)
and (
		(
		select count(distinct mhg.genre_id) 
		from movie_has_genre as mhg,role as r1
		where rol1.movie_id=mhg.movie_id
		and r1.actor_id=rol1.actor_id)
		+
		(
		select count(distinct mhg.genre_id) 
		from movie_has_genre as mhg,role as r2
		where rol2.movie_id=mhg.movie_id
		and r2.actor_id=rol2.actor_id)
	)>=7