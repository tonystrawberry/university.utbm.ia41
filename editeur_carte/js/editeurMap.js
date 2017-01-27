/* Image de chargement */
loader = "<img src=\"images/loader.gif\" border=\"0\" />";

/* Fonction retournant une image de sol */
function imageBase(imgcomplete){
	if(imgcomplete)
		imagebase = '<img src="images/cases/Vide.jpg" alt="Vide" class="Vide" id="listi" style="margin: 0px" />';
    else
    	imagebase = "images/cases/Vide.jpg" ;
    return imagebase;
}

function AfficherElement(){
	
	var element = [
		"Vide", "Herbe", "Diam", "Stone",
		"StW1", "StW2", "StW3", "StW4",
		"S1W1", "S1W2", "Avatar1", 
		"M1", "M2", "Amoeba", "Exit"
	];
	
	var elements = "";
	for(i = 0;i<element.length;i++){
		elements += '<img src="images/cases/'+element[i]+'.jpg" id="listi" class="'+element[i]+'" alt="'+element[i]+'" />';
	}
	
	$("#elements").html(elements);
	
}

function GenereGrille(hauteur,largeur){
	grille = "<table id=\"MapEditor_viewMap\">";
	imagebase = imageBase(true);
	// parcours de la hauteur
	for(h=0;h<hauteur;h++){
		grille += "<tr>";
		for(l=0;l<largeur;l++){
			grille += "<td class=\""+h+"_"+l+"\"\">"+imagebase+"</td>";
		}
		grille += "</tr>";
	}
	
	grille += "</table>";
	$("#map").html(grille);
	
	return true;
}


/**
Les fonctions importantes pour le drag and drop
**/
function ActiveDragDrop(){

	function addDraggable(e1){
				e1.draggable({
					cancel: 'a.ui-icon',// clicking an icon won't initiate dragging
					revert: 'invalid', // when not dropped, the item will revert back to its initial position
					containment: $('#demo-frame').length ? '#demo-frame' : 'document', // stick to demo-frame if present
					helper: 'clone',
					cursor: 'move'
				});
				}
				
				function addDraggableb(e1){
				e1.draggable({
					cancel: 'a.ui-icon',// clicking an icon won't initiate dragging
					revert: 'invalid', // when not dropped, the item will revert back to its initial position
					containment: $('#demo-frame').length ? '#demo-frame' : 'document', // stick to demo-frame if present
					helper : 'clone',
					cursor: 'move'
				});
				}
				
				
				/**
					On rend draggable les elements du skin
				**/
				var $element = $("#elements"); var $reception = $("#map");
				//$('img',$element).addDraggable($(this));
				$element.find("img").each( function(){
						addDraggable($(this));
					}
					
				);
				
				
				$reception.find('td').each( function() {
					/** très important pour les changements de skin
					rend draggable les images qui ont été mises sur
					la carte
					**/
					$(this).find('img').each( function() {
							if($(this).attr("id") == "mis"){
							addDraggable($(this));
						}
					});
					
					//val = $(this).attr('class');
					$(this).droppable({
					accept: 'img' ,
					activeClass: 'ui-state-highlight',
					drop: function(ev, ui) {
						if($(ui.draggable).attr("id") == "listi"){
							var drag1 = (ui.draggable).clone();
							$(this).html(drag1);
							drag1.attr({ id:"mis" });
							addDraggableb(drag1);
						}else{
							var Drag = (ui.draggable).clone();
							$(ui.draggable).attr({src : imageBase(false), alt: "Vide", id:"listi"});
							$(ui.draggable).draggable( 'disable' );
							$(this).html(Drag);
							addDraggable(Drag);
							}
						
					}
				});
						
					}
				);
				
			
				
				
				$("#elements").droppable({
					accept: 'td > img',
					activeClass: 'ui-state-highlight',
					drop: function(ev, ui) {
						var drag1 = (ui.draggable).clone();
						//$(this).append(drag1);
						//addDraggableb(drag1);
						$("#trash").html(drag1);
						$(ui.draggable).attr({src : imageBase(false), alt: "Vide", id:"herb"});
						$(ui.draggable).draggable( 'disable' );
						//$("#"+idBoxMap).html(imageBase(skin,true));
					}
				});
	
	
}

function afficherCode(){
	
	var hauteur = $("#infoHeight").val();
    var largeur = $("#infoWidth").val();
	var name = $("#namecarte").val();
	
	code = '<Map Name="'+name+'" Width="'+largeur+'" Height="'+hauteur+'">\n';
	code += '<Data>\n';
	
	var arrayCases = [];
	var codeMap = $("#MapEditor_viewMap img").each(function(i){
	
		arrayCases[i] = $(this).attr("alt");
	
	});
	
	nbDiamants = 0;
	i=0;
	for(c=0;c<hauteur;c++){
	
		code += '"';
	
		for(l=0;l<largeur;l++){
		
			if(arrayCases[i] == "Vide") valeur = ' ';
			else if(arrayCases[i] == "Herbe") valeur = '.';
			else valeur = arrayCases[i];
			
			if(valeur == "Diam") nbDiamants++;
			
			code += valeur;
			if(l<(largeur-1))
				code += ',';
		
			i++;
		
		}
		
		code += '"\n';
	
	}
	
	code += '</Data>\n\n';
	
	code += '<Infos GameDifficulty="Easy">\n';
	code += '<Goals Player="1" Time="80">\n';
	code += '  <Goal Gather="'+nbDiamants+'" Item="Diam" Points="10" />\n';
	code += '</Goals>\n';
	code += '<Gravity Direction="0,1" />\n';
	code += ' </Infos>\n\n';

	code += '<Infos GameDifficulty="Medium">\n';
	code += ' <Goals Player="1" Time="80">\n';
	code += '	<Goal Gather="'+nbDiamants+'" Item="Diam" Points="10" />\n';
	code += ' </Goals>\n';
	code += ' <Gravity Direction="0,1" />\n';
	code += '</Infos>\n\n';

	code += '<Infos GameDifficulty="Hard">\n';
	code += ' <Goals Player="1" Time="80">\n';
	code += '	<Goal Gather="'+nbDiamants+'" Item="Diam" Points="10" />\n';
	code += ' </Goals>\n';
	code += ' <Gravity Direction="0,1" />\n';
	code += ' </Infos>\n\n';
   
	code += '</Map>';
	
	$("#code").val(code);
	
}

$(document).ready(function()
{
	$("input#boutonGenerer").click( function(){
			
		var hauteur = $("#hauteur").val();
		var largeur = $("#largeur").val();
			
		
		if (hauteur > 100 || hauteur < 4 || largeur < 4 || largeur > 100){
			alert('Impossible de générer la grille de la carte');
			return false;
		}
  
		GenereGrille(hauteur,largeur);
		$("#infoMapOk").val('1');	
        $("#infoHeight").val(hauteur);
        $("#infoWidth").val(largeur);
		
		AfficherElement();
		ActiveDragDrop();		
		return true;
			
	});
	
	$("input#save").click(function(){
		
		afficherCode();
		
	});

});