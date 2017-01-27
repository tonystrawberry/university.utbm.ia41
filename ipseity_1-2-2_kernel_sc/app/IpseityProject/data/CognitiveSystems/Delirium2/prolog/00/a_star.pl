:- module( astar, [
	a_star/5
] ).

:- use_module(library(lists)).

isObstacle(3).
isObstacle(4).
isObstacle(5).
isObstacle(6).

isSortie(21).
isObstacle(23).
isObstacle(24).
isObstacle(25).
isObstacle(26).
isObstacle(27).
isObstacle(28).
isObstacle(50).


/*
	a_star(+InitialState, +FinalState, -Path, +Perceipt, +Size)
*/

/* 
	Algorithme de l'astar servant à trouver un chemin à partir d'un point de départ et de la destination
*/

a_star(InitialState, FinalState, Path, Perceipt, Size) :-

	/* 
		Forme de l'openList 
		[[Cout1, Chemin1], [Cout2, Chemin2], ...] où Cout(n) < Cout(n+1) et Chemin(n) sont des listes de chemins
	*/

	/* 
		Forme de la closedList 
		[Position1, Position2, ...] où Position est l'index d'une case ayant été déja visitée et vérifiée
	*/
	nb_setval(openList, [[[0,0], [InitialState]]]),
	nb_setval(closedList, []),
	expand_a_star(Path, Perceipt, FinalState, Size).

% Heuristique entre un point A et un point B 
getHeuristicValue(A, H, B, Size) :- 
	Xa is A mod Size,
	Ya is A // Size,
 	Xb is B mod Size,
	Yb is B // Size,
	H is abs(Xb-Xa) + abs(Yb-Ya).

getTop(State,Perceipt,Top, Size) :-
	State > Size,
	Top1 is State - Size,
	nth0(Top1, Perceipt, P),
	nb_getval(closedList, ClosedList), not(member(Top1, ClosedList)),
	not(isObstacle(P)), !,
	Top is Top1.

getTop(_,_,Top, _) :-
	Top is -1.

getBottom(State,Perceipt,Bottom, Size) :-
	length(Perceipt, L),
	State < L - Size,
	Bottom1 is State + Size,
	nth0(Bottom1, Perceipt, P), 
	nb_getval(closedList, ClosedList), not(member(Bottom1, ClosedList)),
	not(isObstacle(P)), !,
	Bottom is Bottom1.

getBottom(_,_,Bottom, _) :-
	Bottom is -1.

getRight(State,Perceipt,Right, Size) :-
	State mod Size <  Size - 1,
	Right1 is State + 1, 
	nth0(Right1, Perceipt, P),
	nb_getval(closedList, ClosedList), not(member(Right1, ClosedList)),
	not(isObstacle(P)), !, 
	Right is Right1.

getRight(_,_,Right, _) :-
	Right is -1.

getLeft(State,Perceipt,Left, Size) :-
	State mod Size > 0,
	Left1 is State - 1, 
	nth0(Left1, Perceipt, P), 
	nb_getval(closedList, ClosedList), not(member(Left1, ClosedList)),
	not(isObstacle(P)), !,
	Left is Left1.

getLeft(_,_,Left, _) :-
	Left is -1.

/*
	getAllAccessibleStates(+State, +Perceipt, -AccessibleStatesList, +Size)
*/

/*
	Retourne la liste des cases adjacentes à un point accessibles (ignore les cases où il y a un obstacle)
*/

getAllAccessibleStates(State, Perceipt, AccessibleStatesList, Size) :-
	getTop(State,Perceipt,Top,Size),
	getBottom(State,Perceipt,Bottom, Size),
	getRight(State,Perceipt,Right, Size),
	getLeft(State,Perceipt,Left, Size),
	AccessibleStatesList = [Top,Bottom,Right,Left].
	

/* 
	expand_a_star(-Path, +Perceipt, -FinalState, +Size)
*/

/*
	Prédicat récursif expand_a_star pour trouver le chemin avec l'algorithme astar
*/

/*
	Cas 1 : Chemin trouvé
*/

expand_a_star(Path, _, FinalState, _) :- 
	nb_getval(openList, OpenList),
	OpenList = [[_, Path]|_],
	Path = [FinalState|_],
	!.

/*
	Cas 2 : Chemin non trouvé
*/

expand_a_star([], _, _, _) :-
	nb_getval(openList, []),
	!.

/*
	Cas 3 : En cours de recherche
*/
expand_a_star(Path, Perceipt, FinalState, Size) :-

	
	nb_getval(openList, [[Cost, BestPath] | OthersOpen]),
	nb_getval(closedList, ClosedList),

	BestPath = [LastNode | _],

	nb_setval(openList, OthersOpen),

	% ajouter le noeud courant a la liste fermée
	append(ClosedList, [LastNode], ClosedList2),
	nb_setval(closedList, ClosedList2),

	% trouver tous les cases accessibles
	getAllAccessibleStates(LastNode, Perceipt, AccessibleStatesList, Size),

	% ajouter a la liste ouverte les cases accessibles
	addPath(BestPath, Cost, AccessibleStatesList, FinalState, Size),

	% recursivité
	expand_a_star(Path, Perceipt, FinalState, Size).

/*
	addPath(+BestPath, +Cost, +AccessibleStatesList, +FinalState, +Size)
*/

/*
	Ajoute les nouveaux chemins générés à la liste ouverte et trie la liste 
	ouverte en fonction du cout pour obtenir le meilleur chemin en tête de liste
*/

addPath(_, _, [], _, _) :- 
	nb_getval(openList, OpenList),
	sortOpenList(OpenList, NewOpenList),
	nb_setval(openList, NewOpenList).

addPath(BestPath, Cost, [-1|R], FinalState, Size) :- 
	!, addPath(BestPath, Cost, R, FinalState, Size ).

addPath(BestPath, [G,F], [S | OtherNodes], FinalState, Size) :- 

	% Recupere les noeuds et les poids
	% Calcule le nouveau cout
	getHeuristicValue(S, H, FinalState, Size),
	G1 is G + 1,
	F1 is G1 + H,
	
	NewCost = [G1,F1],

	% Ajoute a la liste ouverte le nouveau chemin
	append([S], BestPath, NewPath),
	addToOpenList(NewCost, NewPath),

	% Recursivité
	addPath(BestPath, [G,F], OtherNodes, FinalState, Size).

/*
	addToOpenList(+NewCost, +NewPath)
*/

/*
	Ajoute le chemin avec son cout correspondant à la liste ouverte
*/

addToOpenList(NewCost, NewPath) :-
	nb_getval(openList, OpenList),

	append([[NewCost, NewPath]], OpenList, NewOpenList),

	nb_setval(openList, NewOpenList).


/*
	sortOpenList(+List, +Sorted)
*/

/*
	Trie la liste avec la méthode du quick_sort en fonction du coût F (croissant)
*/

sortOpenList(List,Sorted):- 
	q_sort(List,[],Sorted).

q_sort([],Acc,Acc).

q_sort([[[H,I],J]|T],Acc,Sorted):-
	pivoting(I,T,L1,L2),
	q_sort(L1,Acc,Sorted1),
	q_sort(L2,[[[H,I],J]|Sorted1],Sorted).

pivoting(_,[],[],[]).
pivoting(I,[[[X,Y],Z]|T],[[[X,Y],Z]|L],G):- Y > I, pivoting(I,T,L,G).
pivoting(I,[[[X,Y],Z]|T],L,[[[X,Y],Z]|G]):- Y =< I, pivoting(I,T,L,G).