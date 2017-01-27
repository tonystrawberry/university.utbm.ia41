:- module( monster, [
	monster/4,
	fromMonsters/3
] ).


isCaseVide(0).
isHerbe(1).


isMonster(23).
isMonster(24).
isMonster(25).
isMonster(26).
isMonster(27).
isMonster(28).

/*
  monster(+Vision, +PreviousVision, +Size, -ModifiedVision)
*/

/*
	A partir de la map actuelle et la map antérieure, 
	retourne la map de prédiction avec les nouvelles positions des monstres 
*/

/* 
	Cas 1 : Première itération du jeu 
	La PreviousVision n'existe pas et il faut la générer

	Prochaine destination des monstres par ordre :
		Top, 
		Right,
		Bottom,
		Left
*/

monster(Vision, [], Size, ModifiedVision) :- 
	!,
	% Liste des positions des monstres actuels
	findAllMonsters(Vision, ListMonsters),
	newPositionOfMonstersFirst(Vision, ListMonsters, Size, ListMonsters1),

	predictedMapV2(Vision, ListMonsters, ListMonsters1, Size, ModifiedVision).

/* 
	Cas 2 : Pas la première itération du jeu 
	La PreviousVision existe déja et est exploité pour avoir la direction actuelle du monstre
	et prédire sa destination suivante

	Algo de déplacement des monstres :
		Le monstre cherche d'abord à gauche de sa direction actuelle,
		sinon continue dans sa direction actuelle,
		sinon cherche à droite, 
		sinon fait demi-tour, 
		sinon reste sur place.
*/

monster(Vision, PreviousVision, Size, ModifiedVision) :-

	predict(Vision, PreviousVision, Size, ModifiedVision).


newPositionOfMonstersFirst(_,[], _, []).
newPositionOfMonstersFirst(Map, [MPosition|R], Size, [NewMPosition|R1]):-
	(
		(
			getTop(MPosition, Map, P, Size), 
			nth0(P, Map, Element), 
			isCaseVide(Element), !,
			NewMPosition is P
		);
		(
			getRight(MPosition, Map, P, Size), 
			nth0(P, Map, Element), 
			isCaseVide(Element), !,
			NewMPosition is P 
		);
		(
			getBottom(MPosition, Map, P, Size), 
			nth0(P, Map, Element), 
			isCaseVide(Element), !,
			NewMPosition is P
		);
		(	getLeft(MPosition, Map, P, Size),
			nth0(P, Map, Element), 
			isCaseVide(Element), 
			NewMPosition is P
		)
	), 

	newPositionOfMonstersFirst(Map, R, Size,R1).

/*
  predict(+Vision, +PreviousVision, +Size, -ModifiedVision)
*/

/*
	A partir de la map actuelle et la map antérieure, 
	retourne la map de prédiction avec les nouvelles positions des monstres 
*/

predict(Vision, PreviousVision, Size, PredictedVision):-
	% Liste des positions des monstres actuels
	findAllMonsters(Vision, ListMonsters),
	% Liste des positions des monstres anterieurs
	findAllMonsters(PreviousVision, PreviousMonsters),

	% Determine les futurs positions des monstres
	newPositionOfMonsters(Vision, ListMonsters, PreviousMonsters, Size, ListMonsters1),

	% Ajoute des murs invisibles autour des monstres 
	predictedMapV2(Vision, ListMonsters, ListMonsters1, Size, PredictedVision).

predictedMapV2(Vision, ListMonsters, ListMonsters1, Size, PredictedVision) :-
	deleteMonsters(Vision, ListMonsters, NewVision),
	addMonsters(NewVision, ListMonsters1, ModifiedVision2), 
	% Add walls
	predictedMap(ModifiedVision2, ListMonsters1, Size, PredictedVision).

deleteMonsters(L, [], L).

deleteMonsters(Vision, [E|R], NewVision) :-
	replaceIndex(Vision, E, 0, NewVisionTemp),
	deleteMonsters(NewVisionTemp, R, NewVision).

addMonsters(L, [], L).

addMonsters(NewVision, [E|R], NewVision2) :-
	replaceIndex(NewVision, E, 23, NewVisionTemp),
	addMonsters(NewVisionTemp, R, NewVision2).

replaceIndex([_|T], 0, X, [X|T]).
replaceIndex([H|T], I, X, [H|R]):- I > -1, NI is I-1, replaceIndex(T, NI, X, R), !.
replaceIndex(L, _, _, L).

/*
  newPositionOfMonsters(+Vision, +ListMonsters, +PreviousMonsters, +Size, -ListMonsters1) 
*/

/*
	A partir de la liste des positions des monstres actuelle et la liste des positions des monstres antérieure, 
	retourne la liste des positions de prédiction des monstres future 
*/

newPositionOfMonsters(Vision, ListMonsters, PreviousMonsters, Size, ListMonsters1) :-
	
	% Recuperer les alentours pour chaque monstre de la map anterieure
	findTargets(PreviousMonsters, Vision, Size, Alentours),

	% Alentours = [[E, [E,A,B,C,D]], [E2, [E2, A2,B2,C2,D2]],...]

	% Verifier si pour chaque element de Alentours, il est contenu dans ListMonsters
	% Si c'est le cas alors sortie d'une liste de sous liste de la forme [[PreviousPosition, ActuelPosition],...]

	associationPositionsMonstres(Alentours, ListMonsters, PrevAndCurrent),

	% checkDouble et checkDouble2 sont là pour corriger les incohérences 
	% (dans le cas où dans les alentours d'un monstre antérieur, il trouve deux monstres se situant sur ceux-ci, 
	% un test sera fait pour déterminer qui est le vrai monstre associé)
	checkDouble(PrevAndCurrent, PrevAndCurrent, SingleList, DoubleList),
	checkDouble2(DoubleList, SingleList, PrevAndCurrent2),

	% Renvoie la liste des positions des monstres futurs
	predictionNewPos(PrevAndCurrent2, Vision, Size, ListMonsters1).

/*
  findTargets(+PreviousMonsters, +Vision, +Size, -Alentours),
*/

/*
	Renvoie la liste des monstres avec leurs cases adjacentes 
	(on vérifiera si dans la liste antérieure, un monstre a été dans une de ces cases auquel cas on pourra 
	connaitre la position anterieure d'un monstre et ainsi predire sa direction)

	Alentours est de la forme
	[[PositionMonstre1, [PosActuelle1,Top1,Right1,Bottom1,Left1]], [PositionMonstre2, [PosActuelle2,Top2,Right2,Bottom2,Left2]],...]
*/

findTargets([], _, _, []).

findTargets([M|R], Map, Size, [[M, Neighbours]|R1]):- 
	findNeighbours(M, Map, Size, Neighbours),
	findTargets(R, Map, Size, R1).

	
findNeighbours(M, Map, Size, [M, Top, Right, Bottom, Left]) :-

	getTop(M, Map, Top, Size),
	getRight(M, Map, Right, Size),
	getBottom(M, Map, Bottom, Size),
	getLeft(M, Map, Left, Size).

getTop(State,_,Top, Size) :-
	State >= Size, !,
	Top is State - Size.

getTop(_,_,Top, _) :-
	Top is -1.

getBottom(State,Perceipt,Bottom, Size) :-
	length(Perceipt, L),
	State =< L - Size, !,
	Bottom is State + Size.

getBottom(_,_,Bottom, _) :-
	Bottom is -1.

getRight(State,_,Right, Size) :-
	State mod Size <  Size - 1, !,
	Right is State + 1.

getRight(_,_,Right, _) :-
	Right is -1.

getLeft(State,_,Left, Size) :-
	State mod Size > 0, !,
	Left is State - 1. 

getLeft(_,_,Left, _) :-
	Left is -1.

predictionNewPos([], _, _, []).

predictionNewPos([E|R], Vision, Size, [NewPos|R1]) :-

	newPosition(E, Vision, Size, NewPos),
	predictionNewPos(R, Vision, Size, R1).

newPosition([E,F], Vision, Size, NewPos) :- 
	E =:= F, !, ((F1 is F+Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
				(F1 is F+1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
				(F1 is F-1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
				(F1 is F-Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) 
				).


newPosition([E,F], Vision, Size, NewPos) :- 
	E + 1 =:= F, !, ((F1 is F-Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F+1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F+Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F-1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) 
					).

newPosition([E,F], Vision, Size, NewPos) :- 
	E - 1 =:= F, !, ((F1 is F+Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F-1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F-Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F+1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) 
					).

newPosition([E,F], Vision, Size, NewPos) :- 
	E + Size =:= F, !,  ((F1 is F+1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F+Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F-1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F-Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) 
					).

newPosition([E,F], Vision, Size, NewPos) :- 
	E - Size =:= F, !, ((F1 is F-1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F-Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F+1, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) ;
					(F1 is F+Size, nth0(F1, Vision, Element), isCaseVide(Element), !, NewPos is F1) 
					).


/*
  associationPositionsMonstres(+Alentours, +ListMonsters, -PrevAndCurrent),
*/

/*
	Retourne la liste des positions précédentes et courants des monstres en étudiant 
	la liste des alentours d'un monstre antérieur et de la liste des positions des monstres actuels
*/

associationPositionsMonstres([], _, []).

associationPositionsMonstres([[E,[A|R]]|Autre], ListMonsters, [[E,A]|R1]) :-
	member(A, ListMonsters), !,
	associationPositionsMonstres([[E,R]|Autre], ListMonsters, R1).

associationPositionsMonstres([[E,[A|R]]|Autre], ListMonsters, R1) :-
	not(member(A, ListMonsters)), !,
	associationPositionsMonstres([[E,R]|Autre], ListMonsters, R1).

associationPositionsMonstres([[_,[]]|Autre], ListMonsters, R1):-
	associationPositionsMonstres(Autre, ListMonsters, R1).

checkDouble2([],L,L).

checkDouble2([[E,N]|R], SingleList, PrevAndCurrent2) :-
	not_already_a_destination(N, SingleList), !,
	checkDouble2(R, [[E,N]|SingleList], PrevAndCurrent2).

checkDouble2([[_,_]|R], SingleList, PrevAndCurrent2) :- 
	checkDouble2(R, SingleList, PrevAndCurrent2).

not_already_a_destination(_, []).

not_already_a_destination(N, [[_,E]|R1]) :-
	N \= E,
	not_already_a_destination(N, R1).

checkDouble([], _, [], []).

checkDouble([[E,N]|R], PrevAndCurrent, [[E,N]|R1], DoubleList) :-
	single(E, PrevAndCurrent), !,
	checkDouble(R, PrevAndCurrent, R1, DoubleList).

checkDouble([[E,N]|R], PrevAndCurrent, PrevAndCurrent2, [[E,N]|R1]) :-
	checkDouble(R, PrevAndCurrent, PrevAndCurrent2, R1).

single(_, []).

single(E, [[N,_]|R1]) :-
	E \= N,
	single(E, R1).

/*
  predictedMap(+ModifiedVision, +ListMonsters, +Size, +PredictedVision).
*/

/*
	ModifiedVision est la liste de vision future avec les nouvelles positions de monstres
	Ajoute des murs invisibles (code 50) autour de ces monstres et retourne la liste
	Le joueur ne pourra pas accéder à ces cases là à la prochaine itération
*/

predictedMap(Vision, [E|R], Size, PredictedVision):-
	Top is E - Size,
	Bot is E + Size,
	Left is E - 1,
	Right is E + 1,


	MonsterWall = [Top, Bot, Left, Right],
	keepOnlyValid(Vision, MonsterWall, SecondVision),

	predictedMap(SecondVision, R, Size, PredictedVision).

predictedMap(L, [], _, L).

keepOnlyValid(Vision, L, SecondVision) :-
	keepInMap(Vision, L, L1),
	areObstacles(Vision, L1, L2),
	replaceAll(Vision, L2, SecondVision).

replaceAll(Vision, [E|R], SecondVision) :-
	E \= -1, !,
	replace(Vision, E, 50, SecVision),

	replaceAll(SecVision, R, SecondVision).

replaceAll(Vision, [_|R], SecondVision) :-
	replaceAll(Vision, R, SecondVision).

replaceAll(L, [], L).

replace([_|T], 0, X, [X|T]).
replace([H|T], I, X, [H|R]):- I > -1, NI is I-1, replace(T, NI, X, R), !.
replace(L, _, _, L).

areObstacles(Vision, [E|R], [E|R1]) :-	
	nth0(E, Vision, Ele),
	(isCaseVide(Ele);isHerbe(Ele)), !,
	areObstacles(Vision, R, R1).

areObstacles(Vision, [_|R], [-1|R1]) :-
	areObstacles(Vision, R, R1).

areObstacles(_, [], []).

keepInMap(Vision, [E|R], [E|R1]) :-
	length(Vision, L),
	E >= 0 , E < L,
	keepInMap(Vision, R, R1).

keepInMap(Vision, [E|R], [-1|R1]) :-
 	length(Vision, L),
 	(E < 0 ; E >= L),
 	keepInMap(Vision, R, R1).

 keepInMap(_,[],[]).

findAllMonsters(Vision, ListMonsters) :- 
	fd(Vision, ListMonsters, 0).

fd([E|R],[I|S], I) :-
	isMonster(E),
	I1 is I+1,
	fd(R, S, I1).

fd([E|R], S, I) :-
	not(isMonster(E)),
	I1 is I+1,
	fd(R, S, I1).

fd([],[],_).

/*
	fromMonsters(+Vision, +Pos, +Size)
*/

/*
	Renvoie true si le personnage est en danger de mort s'il ne bouge pas à cause des monstres (un monstre qui serait à côté du personnage)
	Renvoie false sinon (aucun danger à rester sur place)

	Pour cela, vérifie les cases autour du personnage (Top, Right, Left, Bottom)
*/

fromMonsters(Vision, Pos, Size) :- 
	Top is Pos - Size,
	Bot is Pos + Size,
	Left is Pos - 1,
	Right is Pos + 1,

	nth0(Top, Vision, EleTop),
	nth0(Bot, Vision, EleBot),
	nth0(Left, Vision, EleLeft),
	nth0(Right, Vision, EleRight),

	(	isMonster(EleTop) ; 
		isMonster(EleBot) ;
		isMonster(EleLeft) ;
		isMonster(EleRight) 
	).

