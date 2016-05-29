# ----- CONFIGURE YOUR EDITOR TO USE 4 SPACES PER TAB ----- #
import pymysql as db
import settings
import sys


def connection():
    ''' User this function to create your connections '''
    con = db.connect(
        settings.mysql_host, 
        settings.mysql_user, 
        settings.mysql_passwd, 
        settings.mysql_schema)
    
    return con

def updateRank(rank1, rank2, movieTitle):
    print "rank1:",rank1, "rank2:", rank2, "movie_title:",movieTitle
    avg_cnt,count,avg_rank=0,0,0.0

    con=connection()       
    cur=con.cursor()
    
    try:    
        r1,r2=float(rank1),float(rank2)
        if(r1>=0.0 and r1<=10.0 and r2>=0.0 and r2<=10.0):
            print "Valid rank."
 
            sql="select rank from movie where title=%s"
            cur.execute(sql, (movieTitle,),)
            
            results=cur.fetchall()        
            count=len(results)
            
            if count>0:#an eho apotelesmata
                if (count==1):#an vrika mia tainia me ayton ton titlo
                    if(results[0]==(None,)):#me  null rank tainias
                        print "resulting row has a NULL rank."
                        avg_cnt=2
                        avg_rank=(r1+r2)/avg_cnt
                    else:#i me non-null rank tainias
                        avg_cnt=3
                        avg_rank=(r1+r2+results[0][0])/avg_cnt
                    sql="update movie set rank=%s where title=%s"
                    try:
                        cur.execute(sql,(avg_rank,movieTitle,),)
                        con.commit()
                    except:
                        con.rollback()
                else:#an vrika pollaples tainies me idio titlo
                    print "More than 1 movies with same name!"
            
        else:
            print "Invalid rank (not <10 or >0)!"    
    except ValueError:
       print "Given an invalid Input Type (only float type rank permitted)!"
    finally:#panta kleine to connection.
         con.close()
    
    print "average of %s rank values is: %s"%(avg_cnt,avg_rank)
    
    if(count<>1):
        status="error"
    else:
        status="ok"
        
    return[("status",),(status,),]

def colleaguesOfColleagues(actorId1, actorId2):
    con=connection()
    cur=con.cursor()
    sql="select distinct mov.title,rolc.actor_id,rold.actor_id, \
        rola.actor_id,rolb.actor_id \
        from role as ROLC,role as ROLD,role as rola, role as rolb,movie as mov \
        where \
        rolc.movie_id=mov.movie_id and \
        rola.movie_id=rolb.movie_id and rola.actor_id<rolb.actor_id \
        and rolc.actor_id<rold.actor_id and rola.actor_id<>rolc.actor_id \
        and rolb.actor_id<>rold.actor_id and rola.actor_id<>rold.actor_id \
        and rolb.actor_id<>rolc.actor_id \
        and exists (select ac.movie_id from role as ac where \
        rola.movie_id=rolc.movie_id and ac.movie_id=rola.movie_id) \
        and exists (select bd.movie_id from role as bd where  \
        rolb.movie_id=rold.movie_id and bd.movie_id=rolb.movie_id) \
        and rolc.movie_id=rold.movie_id  \
                and rola.actor_id=%s and rolb.actor_id=%s \
                group by rolc.actor_id,rold.actor_id"
    
    cur.execute(sql, (actorId1,actorId2))  
    results=cur.fetchall()
    output=[("movieTitle", "colleagueOfActor1", "colleagueOfActor2", "actor1","actor2"),]
    for i in results:
        output.append(i)
        
    con.close()
    return output
   
def actorPairs(actorId):
    con=connection()      
    cur=con.cursor()
    sql="select distinct rol2.actor_id\
        from role as rol1,role as rol2,movie_has_genre as mhg1,genre as gen\
        where rol1.actor_id=%s\
        and rol1.movie_id=mhg1.movie_id\
        and mhg1.genre_id=gen.genre_id \
        and rol1.actor_id<>rol2.actor_id\
        and rol2.actor_id not in (select distinct r2.actor_id \
        from role as r1,role as r2,movie_has_genre as mg1,movie_has_genre as mg2\
        where r1.movie_id=mg1.movie_id and r1.actor_id=rol1.actor_id\
        and mg2.movie_id=r2.movie_id and mg1.genre_id=mg2.genre_id)\
        and (\
                        (\
                        select count(distinct mhg.genre_id) \
                        from movie_has_genre as mhg,role as r1\
                        where rol1.movie_id=mhg.movie_id\
                        and r1.actor_id=rol1.actor_id)\
                        +\
                        (\
                        select count(distinct mhg.genre_id) \
                        from movie_has_genre as mhg,role as r2\
                        where rol2.movie_id=mhg.movie_id\
                        and r2.actor_id=rol2.actor_id)\
                )>=7"
    cur.execute(sql,(int(actorId),),)
    results=cur.fetchall()
    output=[("actor2Id",),]
    for i in results:
        output.append(i)

    con.close()
    return output
	
def selectTopNactors(n):
    con=connection()
    cur=con.cursor()
    
    sql="select genre_id from genre"
    cur.execute(sql)
    results=cur.fetchall()
    output=[("genreName", "actorId", "numberOfMovies",),]
    try:
        genre_range=[]
        for row in results:
            genre_range.append(int(row[0]))
        results=[]
        for genre in genre_range:
            sql="select gen.genre_name,rol.actor_id,count(*)\
                from movie_has_genre as mhg,role as rol,genre as gen\
                where mhg.movie_id=rol.movie_id\
                and mhg.genre_id=%s  and mhg.genre_id=gen.genre_id\
                group by mhg.genre_id,rol.actor_id\
                order by mhg.genre_id asc,count(distinct mhg.movie_id) desc limit %s"
            cur.execute(sql,(genre,int(n)))
            temp_results=cur.fetchall()
     
            for row in temp_results:
                results.append(row)

        for i in results:
            output.append(i)
    except ValueError:
        print "%s is not a valid Int literal!"%n
    finally:
        con.close()
    return output

def traceActorInfluence(actorId):

##  Douleyoume os eksis: Katarxin vriskoume tous amesa epirreasmenous
##actors mazi me ta antistoixa eidi, kalwntas ti synartisi 'has_relation'.
##Tous prosthetoume sto grafo 'total' kai prosthetoume ton arxiko actorId
##sti lista previous wste na min ton ksanaepiskeftoume. Katopin, gia kathe
##eyriskomeno actor sygkekrimenou eidous, vriskoume anadromika (synartiseis
##'recursive' kai 'has_relation') tous dikous tou epirreasmenous actors kai
##prosthetoume ton ekastote actor sti lista 'previous' wste na min ton
##ksanaepiskeftoume. Etsi dimiourgeitai o grafos apo edges tripletes (edges)
##me ithopoio pou epirreazei, ithopoio pou epirreazetai kai eidos. Prin ta
##dosoume stin eksodo ws output, ta epeksergazomaste katallila (kykloi,
##diplotypa idioi ithopoioi me diaforetika eidi klp)


    global total    #grafos metavatikou egkleismou mesw influence
    global previous #komvoi tou grafou pou exw eksetasei
    output=[("influencedActorId",),]
    
    try:
        total,previous=[],set([])
        results=has_relation(actorId)#vgazw ta arxika amesa relations gia ton actorId.
        #results: a group of triplets: triplets(triplet[0] = actor that influences,
                                    #   triplet[1] = influenced actor,
                                    #   triplet[2] = specific genre,)
        for row in results:
            total.append(row)       #oi amesa influenced mpainoun sto grafo
        
        if results<>[]:             #an eixa amesa influenced vriskw anadromika tous ypoloipous.
            previous.add(long(actorId)) #dimiourgo proigoumeno me ton komvo actorId
                                        #wste na min ton ksanaepiskeftw.

                                 #symplirwnoume ton ypoloipo grafo dinontas tis tripletes
            recursive(results)   #twn results kai psaxnoume "tous influenced twn influenced",
                                 #anadromika eksetazontas ta ws pros to dedomeno eidos.
            
            #removing cycles------------------------------------------------------
            for row1 in total:
                for row2 in total:
                    if row1[0]==row2[1] and row1[1]==row2[0] and row1[2]==row2[2]:
                        total.remove(row2)

            for row in total:
                if row[1]==long(actorId):
                    total.remove(row)
            #------------------------------------------------------removing cycles

            total=list(set(total))

        else:
            print "No influence for actor %s!"%actorId
        
        for row in total:
            if (row[1],) not in output:#afairesi diplotypwn dioti de mas noiazei pia to eidos
                output.append((row[1],))#p.x. (actor1,genre1,),(actor1,genre2,)->(actor1,genre1,)

    except ValueError,UnicodeEncodeError:
        print "Invalid Input!"

    return output

###=============================================================================


def has_relation(actorId):
    con=connection()
    cur=con.cursor()
    sql="select distinct rol2.actor_id ,gen.genre_id \
            from role as rol1,role as rol2,movie_has_genre as mhg1, \
                    movie_has_genre as mhg2,movie as mov1,movie as mov2, \
            actor as act1,actor as act2,genre as Gen \
            where rol1.actor_id=%s and \
            rol1.movie_id=mhg1.movie_id and \
            mov1.movie_id=mhg1.movie_id and \
            exists (\
            select rol.movie_id from role as rol where \
                                    rol1.movie_id=rol.movie_id and \
                                    mhg1.movie_id=rol1.movie_id and \
                                    rol2.actor_id=rol.actor_id) and \
            rol1.actor_id<>rol2.actor_id and \
            rol2.movie_id=mhg2.movie_id and \
            mhg1.genre_id=mhg2.genre_id and \
            mhg2.movie_id=mov2.movie_id and \
            mov2.year>mov1.year\
            and act1.actor_id=rol1.actor_id \
            and act2.actor_id=rol2.actor_id \
            and gen.genre_id=mhg1.genre_id"

    cur.execute(sql,(int(actorId),))
    results=cur.fetchall()
    output=[]
    for row in results:
        output.append((long(actorId),row[0],row[1],))

            
    con.close()
    return output


def also_related(triplet):
    con=connection()
    cur=con.cursor()
    sql="select distinct rol2.actor_id ,gen.genre_id \
            from role as rol1,role as rol2,movie_has_genre as mhg1, \
                    movie_has_genre as mhg2,movie as mov1,movie as mov2, \
            actor as act1,actor as act2,genre as Gen \
            where rol1.actor_id=%s and \
            rol1.movie_id=mhg1.movie_id and \
            mov1.movie_id=mhg1.movie_id and \
            exists (\
            select rol.movie_id from role as rol where \
                                    rol1.movie_id=rol.movie_id and \
                                    mhg1.movie_id=rol1.movie_id and \
                                    rol2.actor_id=rol.actor_id) and \
            rol1.actor_id<>rol2.actor_id and \
            rol2.movie_id=mhg2.movie_id and \
            mhg1.genre_id=mhg2.genre_id and \
            mhg2.movie_id=mov2.movie_id and \
            mov2.year>mov1.year\
            and act1.actor_id=rol1.actor_id \
            and act2.actor_id=rol2.actor_id \
            and gen.genre_id=mhg1.genre_id \
            and gen.genre_id=%s    "

    cur.execute(sql,(triplet[1],triplet[2],))
    results=cur.fetchall()
    output=[]
    for row in results:
        output.append((triplet[1],row[0],row[1],))
    
    con.close()
    return output


def recursive(triplets):
    #triplet=(actor_who_influences,Influenced_actor,specific_genre,)
    if triplets==[]:
        return
    else:
        result=[]
        for triplet in triplets:
            if ((triplet[1] not in previous)):
                #vriskoume epimerous epirroes gia to sygkekrimeno eidos
                result=also_related(triplet)
                
                #dimiourgoume istoriko komvon
                previous.add(long(triplet[1]))
                
                for i in result:
                    total.append(i)
                    
                recursive(result)
                
        return   
