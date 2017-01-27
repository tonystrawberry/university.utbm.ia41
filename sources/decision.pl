/*

  Définition de l'IA du mineur

  Le prédicat move/12 est consulté à chaque itération du jeu.

*/

:- module( decision, [
	init/1,
	move/12
] ).

:- set_prolog_stack(global, limit(100 000 000 000 000 000 )).
:- use_module( a_star ).
:- use_module( monster ).
:- use_module( rock ).
:- use_module( diamant ).
:- use_module( exit ).

isEmpty([]).

isObstacle(3).
isObstacle(4).
isObstacle(5).
isObstacle(6).
isObstacle(23).
isObstacle(24).
isObstacle(25).
isObstacle(26).
isObstacle(27).
isObstacle(28).
isObstacle(50).

isMonster(23).
isMonster(24).
isMonster(25).
isMonster(26).
isMonster(27).
isMonster(28).

isCaseVide(0).

isRock(3).

init(_).



/*
  move( +L,+LP, +X,+Y,+Pos, +Size, +CanGotoExit, +Energy,+GEnergy, +VPx,+VPy, -ActionId )
*/

/*
  First case : movement to diamond
*/

move( L,_, _,_,Pos, Size, CanGotoExit, _,_, _,_, Action ) :-
	
	CanGotoExit =:= 0,

	% Récupère les anciennes listes de vision et d'états des monstres
	nb_getval(prev_list, LI),
	nb_getval(rock_state, RS),

	% Associe les nouvelles positions des rochers et les anciennes positions
	associationRocks(L, Size, RS, NRS),

	nb_setval(rock_state, NRS),

	% Verifie les cases TopTopRight, TopTopLeft, TopTop, Top et renvoie la nouvelle liste de vision
	wallsRocks(L, Pos, NRS, Size, L2),

	% Prédit la prochaine vision en tenant compte des prochaines destinations des monstres
	monster(L2, LI, Size, L3),

	% Cherche le chemin vers le plus proche diamant
	findNearestDiamond(Pos, L3, L3, Size, Path, 0),

	((
		% Chemin trouvé
		not(isEmpty(Path)), !,
		reverse(Path, Path1),

		% Prend le premier élément du chemin et choisit l'action correspondant pour se rendre sur celui ci
		removehead(Path1, Path2), 
		Path2 = [E|_],
		nextCell(Pos, E, Action1, Size),
		Action is Action1,
		nb_setval(prev_list, L)
	) ; (
		% Chemin non trouvé

		% Vérifie la nécessité de s'échapper ou non
		haveToEscape(L3, Pos, Size, NRS), !, 

		% Le personnage doit s'échapper 

		% Choisit la première case adjacente vide (pour s'échapper)
		escape(L3, Pos, Size, Destination),
		nextCell(Pos, Destination, Action1, Size),
		Action is Action1,
		nb_setval(prev_list, L)
	)).

/*
  Second case : movement to exit
*/ 

move( L,_, _,_,Pos, Size, CanGotoExit, _,_, _,_, Action ) :-

	CanGotoExit =:= 1,

	% Récupère les anciennes listes de vision et d'états des monstres
	nb_getval(prev_list, LI),
	nb_getval(rock_state, RS),

	% Associe les nouvelles positions des rochers et les anciennes positions
	associationRocks(L, Size, RS, NRS),

	nb_setval(rock_state, NRS),

	% Verifie les cases TopTopRight, TopTopLeft, TopTop, Top et renvoie la nouvelle liste de vision
	wallsRocks(L, Pos, NRS, Size, L2),

	% Prédit la prochaine vision en tenant compte des prochaines destinations des monstres
	monster(L2, LI, Size, L3),

	% Cherche le chemin vers la sortie
	findExit(L3, PosExit),


	findPathExit(Pos, L3, Size, Path, PosExit),


	((

		% Chemin trouvé

		not(isEmpty(Path)), !,
		reverse(Path, Path1),

		% Prend le premier élément du chemin et choisit l'action correspondant pour se rendre sur celui ci
		removehead(Path1, Path2), 
		Path2 = [E|_],
		nextCell(Pos, E, Action1, Size),
		Action is Action1,
		nb_setval(prev_list, L)

	) ; (	

		% Chemin non trouvé

		% Vérifie la nécessité de s'échapper ou non
		haveToEscape(L3, Pos, Size, NRS), !, 

		% Le personnage doit s'échapper 

		% Choisit la première case adjacente vide (pour s'échapper)
		escape(L3, Pos, Size, Destination),
		nextCell(Pos, Destination, Action1, Size),
		Action is Action1,
		nb_setval(prev_list, L)
	)).


/*
  Third case : no path found and no need to escape
*/ 

move( L,_, _,_,_, _, _, _,_, _,_, Action ) :- 
	nb_setval(prev_list, L),
	Action is 0.

/*
  nextCell(+Current, +Next, -Action, +Size)
*/

/*
Détermine l'action de mouvement en fonction de la position actuelle et la destination
*/

nextCell(C, C1, 1, _) :- 
	C1 =:= C + 1, 
	!.

nextCell(C, C1, 3, _) :- 
	C1 =:= C - 1, 
	!.

nextCell(C, C1, 2, Size) :- 
	C1 =:= C - Size, 
	!.

nextCell(C, C1, 4, Size) :- 
	C1 =:= C + Size, 
	!.


/*
  haveToEscape(+Vision, +Pos, +Size, -NewRockState)
*/

/*
	Renvoie true si le personnage doit s'échapper (à cause des monstres ou des chutes de rochers)
*/
haveToEscape(Vision, Pos, Size, NewRockState) :-
	fromMonsters(Vision, Pos, Size) ;
	fromFallingThings(Vision, Pos, Size, NewRockState).

/*
  escape(+Vision, +Pos, +Size, -Destination)
*/

/*
	Cherche une case vide autour du monstre pour s'échapper
*/
escape(Vision, Pos, Size, Destination) :- 
	Top is Pos - Size,
	Bot is Pos + Size,
	Left is Pos - 1,
	Right is Pos + 1,

	nth0(Top, Vision, EleTop),
	nth0(Bot, Vision, EleBot),
	nth0(Left, Vision, EleLeft),
	nth0(Right, Vision, EleRight),

	(	(isCaseVide(EleRight), Destination is Right) ;
		(isCaseVide(EleLeft), Destination is Left)  ;
		(isCaseVide(EleTop), Destination is Top) ;
		(isCaseVide(EleBot), Destination is Bot)   
	).

/*
	Utilitaires
*/

removehead([_|Tail], Tail).








