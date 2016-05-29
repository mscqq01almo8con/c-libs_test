# --------------------------------------
# --------------------------------------
/*

Μεταπτυχιακό Πρόγραμμα Ηλεκτρονικού Αυτοματισμού - Τμήμα φυσικής

Όνομα φοιτητή: 	Αλέξιος Κυριαζής

ΑΜ: 			2012509

Η/Μ: 			Δευτέρα 3 Μαρτίου 2014


*/
# --------------------------------------
# --------------------------------------

DROP PROCEDURE IF EXISTS ValidateQuery;
DELIMITER //
CREATE PROCEDURE ValidateQuery(IN qNum INT, IN queryTableName VARCHAR(255))
BEGIN
	DECLARE cname VARCHAR(64);
	DECLARE done INT DEFAULT FALSE;
	DECLARE cur CURSOR FOR SELECT c.column_name FROM information_schema.columns c WHERE 
c.table_schema='movies' AND c.table_name=queryTableName;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	# Add the column fingerprints into a tmp table
	DROP TABLE IF EXISTS cFps;
	CREATE TABLE cFps (
  	  `val` VARCHAR(50) NOT NULL
	) 
	ENGINE = InnoDB;

	OPEN cur;
	read_loop: LOOP
		FETCH cur INTO cname;
		IF done THEN
      			LEAVE read_loop;
    		END IF;
		
		DROP TABLE IF EXISTS ordered_column;
		SET @order_by_c = CONCAT('CREATE TABLE ordered_column as SELECT ', cname, ' FROM ', queryTableName, ' ORDER BY ', cname);
		PREPARE order_by_c_stmt FROM @order_by_c;
		EXECUTE order_by_c_stmt;
		
		SET @query = CONCAT('SELECT md5(group_concat(', cname, ', "")) FROM ordered_column INTO @cfp');
		PREPARE stmt FROM @query;
		EXECUTE stmt;

		INSERT INTO cFps values(@cfp);
		DROP TABLE IF EXISTS ordered_column;
	END LOOP;
	CLOSE cur;

	# Order fingerprints
	DROP TABLE IF EXISTS oCFps;
	SET @order_by = 'CREATE TABLE oCFps as SELECT val FROM cFps ORDER BY val'; 
	PREPARE order_by_stmt FROM @order_by;
	EXECUTE order_by_stmt;

	# Read the values of the result
	SET @q_yours = 'SELECT md5(group_concat(val, "")) FROM oCFps INTO @yours';
	PREPARE q_yours_stmt FROM @q_yours;
	EXECUTE q_yours_stmt;

	SET @q_fp = CONCAT('SELECT fp FROM fingerprints WHERE qnum=', qNum,' INTO @rfp');
	PREPARE q_fp_stmt FROM @q_fp;
	EXECUTE q_fp_stmt;

	SET @q_diagnosis = CONCAT('select IF(@rfp = @yours, "OK", "ERROR") into @diagnosis');
	PREPARE q_diagnosis_stmt FROM @q_diagnosis;
	EXECUTE q_diagnosis_stmt;

	INSERT INTO results values(qNum, @rfp, @yours, @diagnosis);

	DROP TABLE IF EXISTS cFps;
	DROP TABLE IF EXISTS oCFps;
END//
DELIMITER ;

# --------------------------------------

# Execute queries (Insert here your queries).

# Validate the queries
drop table if exists results;
CREATE TABLE results (
  `qnum` INTEGER  NOT NULL,
  `rfp` VARCHAR(50)  NOT NULL,
  `yours` VARCHAR(50)  NOT NULL,
  `diagnosis` VARCHAR(10)  NOT NULL
)
ENGINE = InnoDB;


# -------------
# Q1
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

/*-----------------------------------------------------------------------*/
#epilegoume ton titlo tainias me tis parakato proypotheseis
select MOV.title 
	from movie as MOV,actor as ACT ,
		 role AS ROLE,genre as GEN ,
		 movie_has_genre as MHG
	where 	#kanoume join sta relations kai epilegoume seires
		ACT.actor_id=ROLE.actor_id AND ROLE.movie_id=MOV.movie_id
		AND MHG.genre_id=GEN.genre_id AND MHG.movie_id=MOV.movie_id
		AND GEN.genre_name="Comedy" AND ACT.last_name="Allen" ;
/*-----------------------------------------------------------------------*/

CALL ValidateQuery(1, 'q');
drop table if exists q;
# -------------


# -------------
# Q2
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

/*-----------------------------------------------------------------------*/

#epilegoume eponymo skinotheti kai titlo tainias
select DIR.last_name , MOV.title

from director as DIR , movie as MOV,movie_has_director as MHD
#opou: i tainia plirei tis eksis proipotheseis:
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
/*-----------------------------------------------------------------------*/

CALL ValidateQuery(2, 'q');
drop table if exists q;
# -------------


# -------------
# Q3
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

/*-----------------------------------------------------------------------*/
select distinct ACT.last_name 
from actor as ACT, genre as GEN,movie as MOV
#katarxin o ithopoios paizei se tainia me skinotheti me to idio eponymo
where  exists(	select distinct ACT.last_name 
				from  director as DIR ,
					movie_has_director as MHD,role as ROLE
				where MOV.movie_id=MHD.movie_id
						and MHD.director_id=DIR.director_id
						and ACT.actor_id=ROLE.actor_id
						and ROLE.movie_id=MOV.movie_id
						and ACT.last_name=DIR.last_name) 
#o ithopoios paizei se tainia skinotheti me allo eponymo
and exists ( 	select distinct ACT.last_name 
				from  movie as MOV, director as OTHER_DIR ,
					movie_has_director as MHOND,role as ROLE,
					movie_has_genre as MHG
			/*MHOND=Movie Has 'Other Name' Director*/
				where MOV.movie_id=MHOND.movie_id
						and MHOND.director_id=OTHER_DIR.director_id
						and ACT.actor_id=ROLE.actor_id
						and ROLE.movie_id=MOV.movie_id
						and ACT.last_name<>OTHER_DIR.last_name
						and MOV.movie_id=MHG.movie_id 
						and MHG.genre_id=GEN.genre_id)
#kai yparxei tainia idiou eidous (GEN2.genre_id=GEN.genre_id) pou exei skinothetisei
#skinothetis (DIR) me to idio eponymo (ACT.last_name), alla den paizei o ithopoios (ACT)
and exists(	select MOV2.movie_id
				from director as DIR,movie as MOV2, 
					actor as ACT,genre as GEN2,
					movie_has_director as MHD, 
					movie_has_genre as MHG#
				where GEN2.genre_name=GEN.genre_name and 
						GEN2.genre_id=MHG.genre_id and
						MHG.movie_id=MOV2.movie_id and
						DIR.last_name=ACT.last_name and
						DIR.director_id=MHD.director_id and
						MOV2.movie_id=MHD.movie_id and
						Not exists (select ACT.Actor_id
									 from role as ROLE1
									 where ACT.actor_id = ROLE1.actor_id
										 and ROLE1.movie_id=MOV2.movie_id)
			);
/*-----------------------------------------------------------------------*/


CALL ValidateQuery(3, 'q');
drop table if exists q;
# -------------


# -------------
# Q4
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.
/*

/*-----------------------------------------------------------------------*/
#Anti gia 'if' kanoume mia domi me union kai amoivaios apokleiomena apotelesmata
(#an yparxei tainia me ayta ta kritiria, epistrefoume mia stili me "yes"
	select  "yes" as Existence
	from movie as MOV , genre as GEN, movie_has_genre as MHG
	where GEN.genre_name="Drama" and MHG.genre_id=GEN.genre_id
		and MHG.movie_id=MOV.movie_id and MOV.year='1995'
)
union
(#an den yparxei tainia me ayta ta kritiria, epistrefoume mia stili me "no"
	select  "no" as Existence 
	from movie as MOV 
	where not exists	(	select  MOV.movie_id
							from movie as MOV ,genre as GEN,movie_has_genre as MHG
							where GEN.genre_name="Drama" and MHG.genre_id=GEN.genre_id
									and MHG.movie_id=MOV.movie_id and MOV.year='1995')
);
/*-----------------------------------------------------------------------*/


CALL ValidateQuery(4, 'q');
drop table if exists q;
# -------------


# -------------
# Q5
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

/*-----------------------------------------------------------------------*/
#epilegoume eponyma zeygon skinotheton pou gyrisan tainies
#anamesa sto 2000 kai to 2006 kai exoun gyrisei 
#toulaxiston 6 alla eidi o kathenas
select distinct DIR2.last_name as direc1,DIR1.last_name as direc2
from movie as MOV,director as DIR1, director as DIR2,
	movie_has_director as MHD1,movie_has_director as MHD2
where DIR1.director_id=MHD1.director_id and DIR2.director_id=MHD2.director_id
	and MHD1.movie_id = MOV.movie_id and MHD2.movie_id = MOV.movie_id 
	and DIR1.director_id<DIR2.director_id
	and MHD1.director_id<MHD2.director_id 
	and (MOV.year between 2000 and 2006) 
/* prepei o skinothetis na exei gyrisei toulaxiston 6 eidi*/
	and (	select COUNT(distinct GEN.genre_name)
			from movie as MOV1 ,
				genre as GEN,movie_has_genre as MHG1,
				movie_has_director as MHD1,movie_has_director as MHD2
			where  DIR1.director_id=MHD1.director_id
			and DIR2.director_id=MHD2.director_id
			and MHD1.movie_id = MOV1.movie_id
			and	MHD2.movie_id = MOV1.movie_id 
			and DIR1.director_id<DIR2.director_id
			and MHD1.director_id<MHD2.director_id
			and	GEN.genre_id=MHG1.genre_id
			and MHG1.movie_id=MOV1.movie_id)>=6;
/*-----------------------------------------------------------------------*/


CALL ValidateQuery(5, 'q');
drop table if exists q;
# -------------


# -------------
# Q6
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

/*-----------------------------------------------------------------------*/
#Metrame arithmo skinotheton ana onomateponymo ithopoiou
select distinct ACT.first_name as A,ACT.last_name as B,count(*) as cnt
from actor as ACT,director as DIR
#gia ton opoio kseroume oti exei paiksei akrivos se 3 tainies
where ACT.actor_id in (select ACT.actor_id
						from actor as ACT,role as ROL
						where ACT.actor_id=ROL.actor_id
						group by ROL.actor_id
						having COUNT(*)=3 )
#kai o skinothetis exei skinothetisei aytes tis tainies.
 and DIR.director_id in (select MHD.director_id
						from movie_has_director as MHD,role as RL
						where ACT.actor_id=RL.actor_id
						and MHD.movie_id=RL.movie_id)
#omadopoiontas ana ithopoio.
group by ACT.actor_id;
/*-----------------------------------------------------------------------*/


CALL ValidateQuery(6, 'q');
drop table if exists q;
# -------------


# -------------
# Q7
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

/*-----------------------------------------------------------------------*/
select distinct GEN.genre_id as r,count(distinct MHD.director_id) as c
from movie_has_genre as MHG,movie_has_director MHD,genre as GEN
/*syndeoume eidi me directors oste na tous aparithmisoume meta*/
where MHG.movie_id=MHD.movie_id and GEN.genre_id=MHG.genre_id
	and MHG.genre_id in (	
				/*Edw vriskoume eidi  pou anikoun se tainies me ena mono eidos*/
							select distinct MHG.genre_id
							from movie_has_genre as MHG
							/*Edo vriskoume tainies pou exoun akrivos ena eidos*/
							where MHG.movie_id in (select distinct MHG.movie_id
													from movie_has_genre as MHG
													group by MHG.movie_id
													having count(*)=1)
						 )
group by GEN.genre_id;/*aparithmoume tous directors omadopoiontas kata eidos*/
/*-----------------------------------------------------------------------*/


CALL ValidateQuery(7, 'q');
drop table if exists q;
# -------------


# -------------
# Q8
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

/*-----------------------------------------------------------------------*/

select ACT.actor_id 
from actor as ACT, role as ROl, genre as GEN, 
	movie as MOV, movie_has_genre as MHG
/*theloume o actor na exei symmetoxi se tainia kapoiou eidous*/
where ACT.actor_id=ROl.actor_id
	and ROL.movie_id=MOV.movie_id
	and MOV.movie_id=MHG.movie_id
	and MHG.genre_id=GEN.genre_id
group by ACT.actor_id/*omadopoioume kata actor, oste na metrisoume ta eidi*/
having count(*)>20;/*osoi exoun paiksei se 21 eidi bgainoun sto apotelesma*/
/*-----------------------------------------------------------------------*/


CALL ValidateQuery(8, 'q');
drop table if exists q;
# -------------


# -------------
# Q9
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

/*-----------------------------------------------------------------------*/

select G1.genre_id as Genr1,G2.genre_id as Genr2,count(*) as CNT
from genre as G1,genre as G2,director as DIR
/*theloume ta diaforetika zeygi ton genres na min emfanizontai 2 fores (G1-G2,G2-G1)*/
where G1.genre_id<G2.genre_id and
/*theloume o director na exei gyrisei kai ta 2 eidi*/
DIR.director_id in 
					(	select MHD.director_id
						from movie_has_genre as MHG,
							movie_has_director as MHD
						where MHG.movie_id=MHD.movie_id
						and MHG.genre_id=G1.genre_id)
and DIR.director_id in 
					(	select MHD.director_id
						from movie_has_genre as MHG,
							movie_has_director as MHD						
						where MHG.movie_id=MHD.movie_id
						and MHG.genre_id=G2.genre_id)
/*omadopoioume kata ta diaforetika zeygi eidon gia na athroisoume tous directors*/
group by G1.genre_id ,G2.genre_id;
/*-----------------------------------------------------------------------*/


CALL ValidateQuery(9, 'q');
drop table if exists q;
# -------------


# -------------
# Q10
drop table if exists q;
create table q as # Do NOT delete this line. Add the query below.

/*-----------------------------------------------------------------------*/

#	Gia kathe zeygos genre_id,actor_id metrame tainies (count)
#oi opoies pliroun synolika ta parakato kritiria (sto where)
select distinct MHG.genre_id,ROL.actor_id,count(*) as c
from movie_has_genre as MHG,role as ROL
#	katarxin epilegoume ta eidi pou exoun tainies 
#stis opoies exei paiksei (kapoio rolo) o ithopoios
where MHG.movie_id=ROL.movie_id and 

/*episis theloume stis parapano tainies,
 na MIN yparxei tainia tou ithopoiou me ayto to eidos
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
group by MHG.genre_id,ROL.actor_id;


/*-----------------------------------------------------------------------*/



CALL ValidateQuery(10, 'q');
drop table if exists q;
# -------------

/*									~Ta 4 Updates~									*/
update fingerprints set fp = "16f08135cdf55d5116e839a4f4a4007d" where qnum = 6;
update fingerprints set fp = "0cbdfe3a95ecc5394304b2a74d22e8df" where qnum = 10;
update fingerprints set fp = "6f79e2b031bba7dd11bd9bc1c342d6a4" where qnum = 3;
update fingerprints set fp = "285dd7b03568bec0641bcae2d2566efd" where qnum = 7;
/*																					*/
DROP PROCEDURE IF EXISTS RealValue;
DROP PROCEDURE IF EXISTS ValidateQuery;
DROP PROCEDURE IF EXISTS RunRealQueries;