go :- decide.

decide:-
  retractall(known(_,_,_)),
  druh(X),
  write('Zviera je'),write(X),write('!:)'),nl.
decide:-
  write('Nebolo mozne zviera identifikovat'),nl.
  
% Triedy

podtrieda(vajcorode) :-
	vyvoj(vajice).
podtrieda(vackovce) :-
	vyvoj(vak).
podtrieda(placentovce) :-
	vyvoj(placenta).
	
% Rady

rad(vtakozobce) :-
	podtrieda(vajcorode),
	okupacia(zem).
rad(kengury) :-
	podtrieda(vackovce),
	okupacia(zem),
	velkost(velke).
rad(bandikuty) :-
	podtrieda(vackovce),
	okupacia(zem),
	velkost(male).
rad(netopiere) :-
	podtrieda(placentovce),
	okupacia(vzduch).
rad(velryby) :-
	podtrieda(placentovce),
	okupacia(voda).	
rad(primaty) :-
	podtrieda(placentovce),
	okupacia(zem).

% druhy

druh(vtakopysk_divny):-
  rad(vtakozobce),
  povrch_tela(srst).
druh(jezura_australska):-
  rad(vtakozobce),
  povrchTela(bodce).

druh(valabia_trpaslicia):-
  rad(kengury),
  obydlie(nora).
druh(kengura_siva):-
  rad(kengury),
  vyskyt(juzna_australia).
druh(kengura_cervena):-
  rad(kengury),
  vyskyt(serna_australia).
druh(kengura_tmavochvosta):-
  rad(kengury),
  vyskyt(stred_australie).

druh(bandikut_dlhonosy):-
  rad(bandikuty),
  farba_srst(hneda).
druh(bandikut_kratkonosy):-
  rad(bandikuty),
  farba_srst(siva).
druh(bandikut_dlhonosy):-
  rad(bandikuty),                 
  farba_srst(tmavo_siva).


druh(netopier_velky):-
  rad(netopiere),                 
  rozpatie_kridel(37),
  vyskyt(europa),
  farba(sverlosiva).
druh(lietavec_stahovavy):-
  rad(netopiere),
  rozpatie_kridel(34),
  vyskyt(vychodna_azia),
  farba(popolavosiva).
druh(netopier_natalidae):-
  rad(netopiere),
  rozpatie_kridel(7),
  vyskyt(juzna_amerika),
  farba(hneda).
druh(ghost_bat):-
  rad(netopiere),
  rozpatie_kridel(12),
  vyskyt(severna_australia),
  farba(hneda).
druh(miniopterus_aelleni):-
  rad(netopiere),
  rozpatie_kridel(7),
  vyskyt(madagaskar),
  farba(tmavosiva).
druh(netopier_fijian_mastiff):-
  rad(netopiere),
  rozpatie_kridel(12),
  vyskyt(fiji),
  farba(hneda).


druh(velryba_biskajska):-
  rad(velryby),
  dlzka(13-17),
  vyskyt(subpolarne_pasmo),
  vaha(40-80).
druh(velryba_gronska):-
  rad(velryby),
  dlzka(14-18),
  vyskyt(polarne_oblasti),
  vaha(50-80).
druh(velryba_japonska):-
  rad(velryby),
  dlzka(14-18),
  vyskyt(severny_pacifik),
  vaha(50-80).
druh(velryba_siva):-
  rad(velryby),
  dlzka(13-17),
  vyskyt(severny_pacifik),
  vaha(14-35).
druh(purple_martin):-
  rad(velryby),
  dlzka(7-12),
  vyskyt(juzny_ocean),
  vaha(5-9).

druh(gorila_vychodna):-
  rad(primaty),
  vyska(1,3-1,9),
  potrava(listy),
  farba(tmavohneda).
druh(lemur_kata):-
  rad(primaty),
  vyska(0,5-0,6),
  farba_chvost(bielo_cierna),
  farba(tmavohneda).
druh(maciak_allenov):-
  rad(primaty),
  vyska(0,5-0,6),
  farba(svetlohneda).
	
% Getting answer from the user

vyvoj(X):- ask(vyvoj,X).
velkost(X):- menuask(velkost,X,[velke,stredne,male]).
okupacia(X):- menuask(okupacia,X,[zem,voda,vzduch]).
povrch_tela(X):- ask(povrch_tela,X).
obydlie(X):- ask(obydlie,X).
vyskyt(X):- ask(vyskyt,X).
rozpatie_kridel(X):- menuask(rozpatie_kridel_cm,X,[37,34,12,7]).
farba(X):- ask(farba,X).
vaha(X):- menuask(vaha_tony,X,[40-80,50-80,14-35,5-9]).
dlzka(X):- menuask(dlzka_meter,X,[13-17,14-18,7-12]).
potrava(X):- ask(potrava,X).
farba_chvost(X):- ask(farba_chvost,X).


ask(Attribute,Value):-          
  known(yes,Attribute,Value),
  !.                                
ask(Attribute,Value):-
  known(_,Attribute,Value),
  !, fail.
ask(Attribute,_):-
  known(yes,Attribute,_),
  !, fail.
  
ask(A,V):-
  write('Rozhodni: '),
  write(A:V),                      
  write('? (yes/no): '),
  read(Y),                       
  asserta(known(Y,A,V)),          
  Y = yes.                       

menuask(Attribute,Value,_):-
  known(yes,Attribute,Value),      
  !.
menuask(Attribute,_,_):-
  known(yes,Attribute,_),           
  !, fail.

menuask(Attribute,AskValue,Menu):-                            
  nl,write('Vyber z moznosti '),write(Attribute),write(':'),nl,
  write_menu(Menu),
  write('Napis moznost'),
  read(Num),nl,
  pick_menu(Num,AnswerValue,Menu),
  asserta(known(yes,Attribute,AnswerValue)),
  AskValue = AnswerValue.          

write_menu(Menu):-
  w_menu(1,Menu), !.            

w_menu(_,[]).
w_menu(N,[Item | Rest]):-       
  write(N),write('. '),write(Item),nl,
  NN is N + 1,
  w_menu(NN,Rest).

pick_menu(N,Val,Menu):-
  integer(N),                      
  pic_menu(1,N,Val,Menu), !.       
  pick_menu(Val,Val,_).             
  
pic_menu(_,_,none_of_the_above,[]). 
pic_menu(N,N, Item, [Item|_]).      
pic_menu(Ctr,N, Val, [_|Rest]):-
  NextCtr is Ctr + 1,               
  pic_menu(NextCtr, N, Val, Rest).
