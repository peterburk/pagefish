# pagefish
Search engine with a static webserver<br/>

<b>Example usage</b><br/>
1. Go to <a href="https://peterburk.github.io/pagefish/index.html" target=_blank>https://peterburk.github.io/pagefish/index.html</a><br/>
2. Type a city name. <br/>
3. Click search. <br/>
4. See the results. <br/>
5. (optional) Copy the relation ID to <a href="https://mym.rtijn.org" target=_blank>MeetYourMappers</a><br/>
<br/>
<b>How the hashing algorithm works:</b><br/>
1. Convert every character in the query to a number. (Peter = 80, 101, 116, 101, 114) <br/>
2. Add up the total of those numbers (80 + 101 + 116 + 101 + 114 = 512).  <br/>
3. Get the Unicode character for that number (512 = Ȁ).  <br/>
4. Load the index file that uses that character as a filename. (GeoNamesIndex/Ȁ.txt) <br/>
5. Grep the query again within that file (clashes exist, e.g. "Dover" = 68 + 111 + 118 + 101 + 114 = 512) <br/>
<br/>
<b>Why this is important:</b><br/>
+ The router and filesystem help the search. <br/>
+ Characters are just numbers. It's faster to search shorter strings. <br/>
+ Server-side operations cost money. Client-side JavaScript is free. <br/>
+ The index can be stored on a static file server e.g. Github. <br/>
<br/>
<b>Known limitations:</b><br/>
- Fuzzy matching isn't supported (but this is not a problem for Chinese)<br/>
- Some characters aren't allowed in filenames, URLs, etc. <br/>
- The hash algorithm should be balanced so that index files are a similar size. <br/>
<br/>
<b>Benchmarks:</b><br/>
time grep -c "Paris" geonames-relations-name-id-lat-lon.txt <br/>
0.382s = 382 ms<br/>
<br/>
Typical search time (by searching a few city names): 30 ms. <br/>
<br/>
This gives a 10x speedup compared to grep. <br/>
<br/>
<b>How to build the index:</b><br/>
cc -lm geonamesToIndex.c -o geonamesToIndex <br/>
time cat geonames_relations.txt | ./geonamesToIndex | ./geonamesToIndex <br/>
17.185s<br/>
<br/>
<br/>
Having many small files in the index uses more space. But storage is cheap. <br/>
Original file geonames-relations-name-id-lat-lon.txt: 25.8 MB<br/>
Index size GeoNamesIndex: 94 MB<br/>
<br/>
<b>About the author</b><br/>
Peter Burkimsher is currently working for OSE, a memory card manufacturer in Kaohsiung, Taiwan. <br/>
I did several projects during my 3.5 years here, including writing bit-level drivers to control memory card testing equipment, a Win32 app to parse log files and send them to a SQL server, network monitoring, and Big Data processing. <br/>
I studied Electronic Systems Engineering at Lancaster University and graduated with first-class honours in 2011, before going off on Working Holiday visas to see the world. <br/>
Now I have 3 years continuous relevant work experience, I’m looking for a suitable job in New Zealand so I can get the Skilled Migrant Category visa. <br/>
(Canada’s Express Entry and Australia are also options).<br/>
<br/>