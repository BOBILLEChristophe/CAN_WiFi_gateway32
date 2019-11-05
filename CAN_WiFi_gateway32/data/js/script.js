var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);
 connection.onopen = function () {
     connection.send('Connect ' + new Date());
 };
 connection.onerror = function (error) {
     console.log('WebSocket Error ', error);
 };
 connection.onmessage = function (e) {  
    console.log('Server: ', e.data);
};
 connection.onclose = function(){
     console.log('WebSocket connection closed');
};




var satellites = {};
var curId = null;            // Id du sat selectionne
var satellitesInitVal = {};  // Objet pour contenir les valeurs par defaut

// Valeurs affichees a l'initialisation
satellitesInitVal.aigSpeed = 150;
satellitesInitVal.aigPosMin = 1500;
satellitesInitVal.aigPosMax = 1500;

satellitesInitVal.ledBright = 100;
satellitesInitVal.ledBrightTime = 100;
satellitesInitVal.ledFadingTime = 100;
satellitesInitVal.ledOnTime = 100;
satellitesInitVal.ledPeriod = 1000;

// Valeurs min et max attribuees aux elements
satellitesInitVal.aigSpeedMin = 100;
satellitesInitVal.aigSpeedMax = 200;
satellitesInitVal.aigPosMinMin = 1300;
satellitesInitVal.aigPosMinMax = 1700;
satellitesInitVal.aigPosMaxMin = 1300;
satellitesInitVal.aigPosMaxMax = 1700;

satellitesInitVal.ledBrightMin = 0;
satellitesInitVal.ledBrightMax = 255;
satellitesInitVal.ledBrightTimeMin = 0;
satellitesInitVal.ledBrightTimeMax = 255;
satellitesInitVal.ledFadingTimeMin = 0;
satellitesInitVal.ledFadingTimeMax = 255;
satellitesInitVal.ledOnTimeMin = 0;
satellitesInitVal.ledOnTimeMax = 255;
satellitesInitVal.ledPeriodMin = 0;
satellitesInitVal.ledPeriodeMax = 1500;


document.addEventListener('DOMContentLoaded', setup, false); // Ecouteur sur chargement de la page -> appele la fonction "setup" ci dessous

function setup () { // Initialise au chargement de la page les elements du DOM, cases a cocher, champs de saisie etc...
	var url = "data.json";
	sendReq(setSat, url);
}

var setSat = function (data) {	
	satellites = jQuery.parseJSON(data); // objet "satellites" contenant les informations sur les satellites.
	satellitesAncVal = jQuery.parseJSON(data);
	menuSat(satellites);	// Creation du menu deroulant
	console.log(satellites);
}

var menuSat = function (satellites) {
	// Pour chaque element de l'objet "satellites", creation d'une ligne de menu
	for( var i = 0; i < satellites.length; i++) {
			$('#menuSat').append('<option value="'+ satellites[i].id +'">'+ satellites[i].id +'</option>');
	}
	// Creation du menu deroulant LED
	for( var i = 0; i < 10; i++) {
			$('#menuLed').append('<option value="'+ i+'">'+ i +'</option>');
	}
	
}

$(document).ready(function(){
	
	console.log("setup"); 
	$("#tab2").show();
	$("#tab3").hide();

	
	//$("#home").click(function(){
//        $("#tab1").show();
//		$("#tab2").hide();
//		$("#tab3").hide();
//    });
	
	$("#servo").click(function(){
        $("#tab1").hide();
		$("#tab2").show();
		$("#tab3").hide();
    });
	
    $("#led").click(function(){
        $("#tab1").hide();
		$("#tab2").hide();
		$("#tab3").show();
    });
	
	
	// Initialisation des elements graphiques
	
	// Servos
	$('#aigSpeedSlider').attr("min", satellitesInitVal.aigSpeedMin);
	$('#aigSpeedSlider').attr("max", satellitesInitVal.aigSpeedMax);
	$('#aigSpeedSlider').attr("value", satellitesInitVal.aigSpeed);
	$('#aigSpeedInput').attr("min", satellitesInitVal.aigSpeedMin);
	$('#aigSpeedInput').attr("max", satellitesInitVal.aigSpeedMax);
	$('#aigSpeedInput').attr("value", satellitesInitVal.aigSpeed);
	
	$('#aigPosMinSlider').attr("min", satellitesInitVal.aigPosMinMin);
	$('#aigPosMinSlider').attr("max", satellitesInitVal.aigPosMinMax);
	$('#aigPosMinSlider').attr("value", satellitesInitVal.aigPosMin);
	$('#aigPosMinInput').attr("min", satellitesInitVal.aigPosMinMin);
	$('#aigPosMinInput').attr("max", satellitesInitVal.aigPosMinMax);
	$('#aigPosMinInput').attr("value", satellitesInitVal.aigPosMin);
	
	$('#aigPosMaxSlider').attr("min", satellitesInitVal.aigPosMaxMin);
	$('#aigPosMaxSlider').attr("max", satellitesInitVal.aigPosMaxMax);
	$('#aigPosMaxSlider').attr("value", satellitesInitVal.aigPosMax);
	$('#aigPosMaxInput').attr("min", satellitesInitVal.aigPosMaxMin);
	$('#aigPosMaxInput').attr("max", satellitesInitVal.aigPosMaxMax);
	$('#aigPosMaxInput').attr("value", satellitesInitVal.aigPosMax);
	
	// Leds
	$('#ledBrightSlider').attr("min", satellitesInitVal.ledBrightMin);
	$('#ledBrightSlider').attr("max", satellitesInitVal.ledBrightMax);
	$('#ledBrightSlider').attr("value", satellitesInitVal.ledBright);
	$('#ledBrightInput').attr("min", satellitesInitVal.ledBrightMin);
	$('#ledBrightInput').attr("max", satellitesInitVal.ledBrightMax);
	$('#ledBrightInput').attr("value", satellitesInitVal.ledBright);
	
	$('#ledBrightTimeSlider').attr("min", satellitesInitVal.ledBrightTimeMin);
	$('#ledBrightTimeSlider').attr("max", satellitesInitVal.ledBrightTimeMax);
	$('#ledBrightTimeSlider').attr("value", satellitesInitVal.ledBrightTime);
	$('#ledBrightTimeInput').attr("min", satellitesInitVal.ledBrightTimeMin);
	$('#ledBrightTimeInput').attr("max", satellitesInitVal.ledBrightTimeMax);
	$('#ledBrightTimeInput').attr("value", satellitesInitVal.ledBrightTime);
	
	$('#ledFadingTimeSlider').attr("min", satellitesInitVal.ledFadingTimeMin);
	$('#ledFadingTimeSlider').attr("max", satellitesInitVal.ledFadingTimeMax);
	$('#ledFadingTimeSlider').attr("value", satellitesInitVal.ledFadingTime);
	$('#ledFadingTimeInput').attr("min", satellitesInitVal.ledFadingTimeMin);
	$('#ledFadingTimeInput').attr("max", satellitesInitVal.ledFadingTimeMax);
	$('#ledFadingTimeInput').attr("value", satellitesInitVal.ledFadingTime);
	
	$('#ledOnTimeSlider').attr("min", satellitesInitVal.ledOnTimeMin);
	$('#ledOnTimeSlider').attr("max", satellitesInitVal.ledOnTimeMax);
	$('#ledOnTimeSlider').attr("value", satellitesInitVal.ledOnTime);
	$('#ledOnTimeInput').attr("min", satellitesInitVal.ledOnTimeMin);
	$('#ledOnTimeInput').attr("max", satellitesInitVal.ledOnTimeMax);
	$('#ledOnTimeInput').attr("value", satellitesInitVal.ledOnTime);
	
	$('#ledPeriodSlider').attr("min", satellitesInitVal.ledPeriodMin);
	$('#ledPeriodSlider').attr("max", satellitesInitVal.ledPeriodeMax);
	$('#ledPeriodSlider').attr("value", satellitesInitVal.ledPeriod);
	$('#ledPeriodInput').attr("min", satellitesInitVal.ledPeriodMin);
	$('#ledPeriodInput').attr("max", satellitesInitVal.ledPeriodeMax);
	$('#ledPeriodInput').attr("value", satellitesInitVal.ledPeriod);
	
});

// Chargement du fichier json de configuration
var sendReq = function (callback, url, param) {
	$.ajax({
	  type: "POST",
	  url: url,
	  data: param,
	  dataType: "text",
	  success: function (data) {
            callback(data);
        }
	});
}

// Initialisation des sliders en fonction des valeurs contenues dans l'objet satellite.
var menuLedInit = function () {
	var num = document.getElementById("menuLed").selectedIndex;
	if(num > 0) {
		$('#ledBrightSlider').val(satellites[curId].led[num].max);
		$('#ledBrightTimeSlider').val(satellites[curId].led[num].brighteningTime);
		$('#ledFadingTimeSlider').val(satellites[curId].led[num].fadingTime);
		$('#ledOnTimeSlider').val(satellites[curId].led[num].onTime);
		$('#ledPeriodSlider').val(satellites[curId].led[num].period);
		
		$('#ledBrightInput').val(satellites[curId].led[num].max);
		$('#ledBrightTimeInput').val(satellites[curId].led[num].brighteningTime);
		$('#ledFadingTimeInput').val(satellites[curId].led[num].fadingTime);
		$('#ledOnTimeInput').val(satellites[curId].led[num].onTime);
		$('#ledPeriodInput').val(satellites[curId].led[num].period);
	}
}

var menuServoInit = function () {
	var num = document.getElementById("menuServo").selectedIndex;
	if(num > 0) {
		$('#aigSpeedSlider').val(satellites[curId].servo[0].speed);
		$('#aigPosMinSlider').val(satellites[curId].servo[0].min);
		$('#aigPosMaxSlider').val(satellites[curId].servo[0].max);
		
		$('#aigSpeedInput').val(satellites[curId].servo[0].speed);
		$('#aigPosMinInput').val(satellites[curId].servo[0].min);
		$('#aigPosMaxInput').val(satellites[curId].servo[0].max);
	}
}


var checkSat = function (x) { // Choix dans le menu deroulant 
	curId = satellites[x].id; // curId est l'identifiant unique du satellite selectionnee
	//$('#servoPin').val(satellites[x].servo[0].pin);
}


// Envoi sur le bus CAN d'une valeur modifiee pour les servos
var setServo = function (name, id, val, code, isTemp) {
	
	if(curId == null) { // Pas de satellite selectionne avec le menu
		alert("Selectionnez un satelilite !");
		console.log($('#'+name+'Slider').val("satellitesInitVal."+name));
		$('#'+name+'Slider').val("satellitesInitVal."+name);     // Le slider prend la valeur selectionnee
		$('#'+name+'Input').val("satellitesInitVal."+name);      // Le champ input prend la valeur selectionnee
	 	return;
	}
	

	$('#'+name+'Slider').val(val);
	$('#'+name+'Input').val(val);		
	
	
	var num = "00000"
	//var isTemp = "0";        // 0 reglage temporaire, 1 reglage permanent
	var len = 3;             // Valeur par defaut
	var type = "0000000"     // Servo
	
	var val = (parseInt(val, 10)).toString(16);
	//var val = (parseFloat(val, 10)).toString(16);
	
	if(val.length > 8) {
		len = 4;
		for (var i = val.length; i < 16; i++) {
			val = "0" + val;
		}
		var prem = val.substring(0, 8);
		var dern = val.substring(8, 16);
		val = prem+" "+dern;
	}
	else { // val < 8
		for (var i = val.length; i < 8; i++) {
			val = "0" + val;
		}
	}
	validation(num, type, len, code, val, isTemp);
}

var validServo = function (isTemp) {
	
	var name="";
	var id=""
	var val = 0;
	var code = "";
	
	name="aigSpeed";
	id="aigSpeedSlider"
	val = $("#aigSpeedSlider").val();
	code = "010";

	setServo(name, id, val, code, isTemp);
	
	name="aigPosMin";
	id="aigPosMinSlider"
	val = $("#aigPosMinSlider").val();
	code = "000";
	
	setServo(name, id, val, code, isTemp);
	
	name="aigPosMax";
	id="aigPosMaxSlider"
	val = $("#aigPosMaxSlider").val();
	code = "001";
	
	setServo(name, id, val, code, isTemp);
	
}


var testServo = function(x) {
	let isTemp = 0;
	if(x == 0) {
		name="aigPosMin";
		id="aigPosMinSlider"
		val = $("#aigPosMinSlider").val();
		code = "000";
	}
	
	else {
		name="aigPosMax";
		id="aigPosMaxSlider"
		val = $("#aigPosMaxSlider").val();
		code = "001";
	}
	
	setServo(name, id, val, code, isTemp);
}



function setLed(name, id, val, code) {
	
	if(curId == null) { // Pas de satellite selectionne avec le menu
		alert("Selectionnez un satelilite !");
		console.log($('#'+name+'Slider').val());
		$('#'+name+'Slider').val("satellitesInitVal."+name);     // Le slider prend la valeur selectionnee
		$('#'+name+'Input').val("satellitesInitVal."+name);      // Le champ input prend la valeur 
	 	return;
	}
	
	var num = document.getElementById("menuLed").selectedIndex; // N° de la led selectionnee
	if(num > 0) {                                               // Si une led est bien selectionnee
		
		$('#'+name+'Slider').val(val);
	    $('#'+name+'Input').val(val);
		
		num -= 1;
		num = num.toString(2);
		switch (num.length) {
		  case 1:
			num = "0000" + num;
			break;
		  case 2:
			num = "000" + num;
			break;
		  case 3:
			num = "00" + num;
			break;
		  case 4:
			num = "0" + num;
			break;
		}
		var isTemp = "0";        // 0 reglage temporaire, 1 reglage permanent
		var len = 3;             // Nombre d'octets de message envoyes
		var type = "0000001"     // Led
		// Parsing en binaire de la valeur
		var val = (parseInt(val, 10)).toString(2);
		for (var i = val.length; i < 8; i++) {
			val = "0" + val;
		}
		validation(num, type, len, code, val, isTemp);
	}
	else {
		alert("Selectionnez une led !");
		console.log($('#'+name+'Slider').val());
		$('#'+name+'Slider').val("satellitesInitVal."+name);     // Le slider prend la valeur selectionnee
		$('#'+name+'Input').val("satellitesInitVal."+name);      // Le champ input prend la valeur 
	}
}

function validation(num, type, len, code, val, isTemp) {
	var bit0 = isTemp+type
	var bit1 = num+code
	var bit2 = val
//(parseInt(val, 10)).toString(2);
bit0 = (parseInt(bit0, 2)).toString(16);
bit1 = (parseInt(bit1, 2)).toString(16);
bit2 = (parseInt(bit2, 2)).toString(16);

	var msg = "0x1FFFFF2";
	msg = msg + curId;
	msg = msg + " ";        // Espace
	msg = msg + "1";        // Type du message Id 29 bits
	msg = msg + " ";        // Espace
	msg = msg + len;        // Longueur du message a suivre en octets
	msg = msg + " ";        // Espace
	//msg = msg + isTemp;     // 0 reglage temporaire, 1 reglage permanent
	//msg = msg + type;       // 0 reglage servo, 1 reglage LED
	msg = msg + "0x"+bit0;
	msg = msg + " ";        // Espace
	//msg = msg + num;        // 0 reglage servo, 0 a 9 reglage LED
	//msg = msg + code        // Code du reglage
	msg = msg + "0x"+bit1;
	msg = msg + " ";        // Espace
	//msg = msg + val;
	msg = msg + "0x"+bit2;
	console.log(msg);
	connection.send(msg);
}
