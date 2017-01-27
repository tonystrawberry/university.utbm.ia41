:- module( diamant, [
	findNearestDiamond/6
] ).

:- use_module( a_star ).

isDiamond(2).

/*
	findNearestDiamond(+Pos, +Map, +Map, +Size, -Path, +Compteur)
*/

/*
	Trouver le chemin vers le diamant le plus proche
*/

findNearestDiamond(_, _, [], _, [], _).

/*
	Case 1 : Premier diamant trouvé
*/

findNearestDiamond(Pos, Map, [E|R], Size, Path, I) :-
	I1 is I+1,
	E =:= 2, !, 

	a_star(Pos, I, TempPath, Map, Size),
	length(TempPath, Long),
	fnd(Pos, Map, R, Size, TempPath, Long, I1, NewPath),

	Path = NewPath.

/*
	Case 1 : Continuation de la recherche
*/

findNearestDiamond(Pos, Map, [E|R], Size, Path, I) :-
	I1 is I+1,
	E \= 2,
	findNearestDiamond(Pos, Map, R, Size, Path, I1).

/*
	fnd(+Pos, +Map, +Map, +Size, -Path, +Compteur)
*/

/*
	Appelé dès que le premier diamant a été trouvé et compare les longueurs 
	entre le chemin le plus court actuel et le nouveau chemin trouvé
	Appel récursif avec le plus court chemin entre les deux
*/

fnd(Pos, Map, [E|R], Size, TempPath, Long, I, NewPath) :-
	I1 is I + 1,
	isDiamond(E), !,

	a_star(Pos, I, TempPath1, Map, Size),
	length(TempPath1, Long1),  

	((
		Long =:= 0, !, 
		fnd(Pos, Map, R, Size, TempPath1, Long1, I1, NewPath)
	);(
		Long1 =:= 0, !,
		fnd(Pos, Map, R, Size, TempPath, Long, I1, NewPath)
	);(
		Long1 < Long, !,
		fnd(Pos, Map, R, Size, TempPath1, Long1, I1, NewPath)
	);(
		Long1 >= Long, !,
		fnd(Pos, Map, R, Size, TempPath, Long, I1, NewPath)
	)).


fnd(Pos, Map, [_|R], Size, TempPath, L, I, NewPath) :- 
	I1 is I + 1,
	fnd(Pos, Map, R, Size, TempPath, L, I1, NewPath).

fnd(_, _, [], _, L, _, _, L).