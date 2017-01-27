:- module( rock, [
	associationRocks/4,
	wallsRocks/5,
	fromFallingThings/4
] ).


isMonster(23).
isMonster(24).
isMonster(25).
isMonster(26).
isMonster(27).
isMonster(28).

isCaseVide(0).

isRock(3).

isObstacle(1).
isObstacle(2).
isObstacle(3).
isObstacle(4).
isObstacle(5).
isObstacle(6).
%isObstacle(20).
isObstacle(23).
isObstacle(24).
isObstacle(25).
isObstacle(26).
isObstacle(27).
isObstacle(28).
isObstacle(50).

/*
	wallsRocks(+Vision, +Pos, +NewRockState, +Size, -NewVision)
*/

/*
	En fonction des états actuels des rochers, ajoute des murs invisibles sur la map afin de 
	permettre notre personnage d'éviter les zones de danger (chutes de rochers)
*/

wallsRocks(Vision, Pos, NewRockState, Size, NewVision) :-
	TTT is Pos - (3 * Size),
	TTR is Pos - (2 * Size) + 1,
	TTL is Pos - (2 * Size) - 1,
	TT is Pos - (2 * Size),

	getPosWallsTTD([TTT, TTR, TTL], Vision, NewRockState, Size, PosWalls),
	getPosWallsTT(TT, Vision, NewRockState, Size, PosWalls1),
	append(PosWalls, PosWalls1, PosWalls2),
	replaceAllByWalls(Vision, PosWalls2, NewVision).


/*
	replaceAllByWalls(+Vision, +PosWalls, -NewVision)
*/

/*
	Ajout des murs invisibles sur la carte sur les positions à risque (code 50)
*/

replaceAllByWalls(Vision, [E|R], SecondVision) :-
	E \= -1, !,
	replace(Vision, E, 50, SecVision),
	replaceAllByWalls(SecVision, R, SecondVision).

replaceAllByWalls(Vision, [_|R], SecondVision) :-
	replaceAllByWalls(Vision, R, SecondVision).

replaceAllByWalls(L, [], L).



/* 
	replace(+List, +Position, +Element, -NewList)
*/

/*
	Remplace l'élément à la Position position de la liste par Element
*/

replace([_|T], 0, X, [X|T]).
replace([H|T], I, X, [H|R]):- I > -1, NI is I-1, replace(T, NI, X, R), !.
replace(L, _, _, L).

/*
	getPosWallsTT(D)(+Position, +Vision, +NewRockState, +Size, -PositionInvisibleWall)
*/

/*
	Vérifie si l'élément à la position Position est un rocher et 
	retourne la position des murs invisibles à placer (une position en dessous pour la position TT et 
	deux positions en dessous pour les positions TopTopRight et TopTopLeft)
*/

getPosWallsTT(E, Vision, NewRockState, Size, [E1]) :-
	E > 0,
	nth0(E, Vision, Element),
	isRock(Element), !,
	% isFalling ?
	getState(E, NewRockState, State),
	((
		State =:= 1, !,
		E1 is E + Size
	);(
		E1 is -1
	)).

getPosWallsTT(_, _, _, _, []).

getPosWallsTTD([], _, _,_,[]).

getPosWallsTTD([E|R], Vision, NewRockState, Size, [E1|R1]) :-
	E > 0,
	nth0(E, Vision, Element),
	isRock(Element), !,
	% isFalling ?
	getState(E, NewRockState, State),
	((
		State =:= 1, !,
		E1 is E + 2 * Size,
		getPosWallsTTD(R, Vision, NewRockState, Size, R1)
	);(
		E1 is -1,
		getPosWallsTTD(R, Vision, NewRockState, Size, R1)
	)).

getPosWallsTTD([_|R], Vision, NewRockState, Size, [E1|R1]) :-
	E1 is -1,
	getPosWallsTTD(R, Vision, NewRockState, Size, R1).

getState(E, NewRockState, State) :-
	memberSpecialPosState(E, NewRockState, PosState),
	PosState = [_, State].

/*
	fromFallingThings(+Vision, +Pos, +Size, +NewRockState)
*/

/*
	Renvoie true si le personnage est en danger de mort s'il ne bouge pas à cause de chutes de rochers
	Renvoie false sinon (aucun danger à rester sur place)

	Pour cela, vérifie les deux cases au dessus du personnage (Top et TopTop)
*/

fromFallingThings(Vision, Pos, Size, NewRockState) :- 
	T is Pos - Size,
	TT is Pos - Size * 2,
	(
		(haveToEscapeFromTop(Vision, T, NewRockState))
		;
		(haveToEscapeFromTopTop(Vision, TT, NewRockState))
	).

haveToEscapeFromTop(Vision, T, NewRockState) :-

	T > 0, nth0(T, Vision, EleTop), 
	isRock(EleTop),
	getState(T, NewRockState, State),
	State =:= 1.

haveToEscapeFromTopTop(Vision, TT, NewRockState) :- 

	TT > 0, nth0(TT, Vision, EleTopTop), 
	isRock(EleTopTop),
	getState(TT, NewRockState, State),
	State =:= 1.


/*
	associationRocks(+Vision, +Size, +RockState, -NewRockState)
*/

/*
	Actualise l'état des rochers de la vision actuelle (l'état est 1 si état 'chute' ; l'état est 0 si état immobile)
*/

associationRocks(Vision, _, [], NewRockState) :-
	findAllRocks(Vision, ListRocks),
	buildState(ListRocks, NewRockState).

associationRocks(Vision, Size, RockState, NewRockState) :-

	findAllRocks(Vision, ListRocks),  

	% Pour chaque rocher trouve son correspondant dans la liste de rochers anterieures 
	associate(Vision, Size, ListRocks, RockState, NewRockState).

findAllRocks(Vision, ListRocks) :- 
	fd(Vision, ListRocks, 0).

fd([E|R],[I|S], I) :- 
	isRock(E),
	I1 is I+1,
	fd(R, S, I1).

fd([E|R], S, I) :-
	not(isRock(E)),
	I1 is I+1,
	fd(R, S, I1).

fd([],[],_).

buildState([],[]).

buildState([E|R], [[E,0]|R1]) :-
	buildState(R, R1).

/*
	associate(+Vision, +Size, +ListRocks, +RockState, -NewRockState)
*/

/*
	A partir de la liste des rochers de la map actuelle et de la liste d'états des rochers de la map antérieure,
	retourne la nouvelle liste d'états des rochers issue de la map actuelle
*/

associate(_, _, [], _, []).
	
% Si trouve la meme position alors meme rocher et meme etat
associate(Vision, Size, [E|R], RockState, [PosState|R1]) :-
	memberSpecialPosState(E, RockState, PosState), !,
	associate(Vision, Size, R, RockState, R1).

% Si trouve rocher en train de tomber et qu'il y a encore une case vide
% en dessous alors etat 'chute'
associate(Vision, Size, [E|R], RockState, [[E,1]|R1]) :-
	Bot is E + Size, 
	nth0(Bot, Vision, EleBot),
	isCaseVide(EleBot), !, 
	associate(Vision, Size, R, RockState, R1).


% Si trouve rocher en train de tomber et quil y a obstacle 
% en dessous alors etat 'immobile'
associate(Vision, Size, [E|R], RockState, [[E,0]|R1]) :-
	Bot is E + Size, 
	nth0(Bot, Vision, EleBot),
	isObstacle(EleBot), !, 
	associate(Vision, Size, R, RockState, R1).

/*
	memberSpecialPosState(+Position, +RockState, -PosState)
*/

/*
	Vérifie si l'élément en position Position est dans la liste d'états des rochers et
	retourne l'état de ce rocher  (utilisé dans le cas où le rocher n'a pas changé de place depuis la dernière itération)
*/

memberSpecialPosState(E, [[E,State]|_], [E,State]) :- !. 

memberSpecialPosState(E, [[F,_]|R], PosState) :- E \= F, memberSpecialPosState(E, R, PosState).