:- module( exit, [
	findExit/2,
	findPathExit/5

] ).

:- use_module(library(lists)).
:- use_module( a_star ).

isExit(21).

/*
	findPathExit(+Pos,+ Map, +Size, -Path, +PosExit)
*/

/*
	Trouver le chemin vers la sortie
*/

findPathExit(Pos, Map, Size, Path, PosExit) :-
	a_star(Pos, PosExit, Path, Map, Size).

/*
	findExit(+Map, -PosExit)
*/

/*
	Trouver la position de la sortie
*/

findExit(Map, PosExit) :-
	isExit(X),
	nth0(PosExit, Map, X).
