$(document).ready(function(){
	var rateH;
  console.log("heart works");
  Chart.defaults.global.responsive = true;
  var count = 10;
  var data = {
	  labels : ["1","2","3","4","5", "6", "7", "8", "9", "10"],
		datasets : [
		  {
		  	label: "HeartBeat vs time",
            backgroundColor: "rgba(204, 0, 0,0.4)",
            borderColor: "rgba(204, 0, 0, 1)",
			data : [50, 53, 56, 59, 52, 55, 58, 51, 54, 57, 60]
			//data : [temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp]
			}
		]
  };
  function recieveHeart() {
  $.get('http://arduino7.local/arduino/heartpin', function( g ){
    rateH = g;           
    $("#hbeat").text( g + "BPM");
    console.log("heart" + g);
  });
}  
  //this is ugly, don't judge me
  var updateHData = function(oldData){
    var labels = oldData["labels"];
    var heartset = oldData["datasets"][0]["data"];
    
    labels.shift();
    count+=2;
    labels.push(count.toString());
    //var newDataA = heartset[9] + (20 - Math.floor(Math.random() * (41)));
    var newDataA = rateH;
    heartset.push(newDataA);
    heartset.shift(); 
  };
      
  var optionsAnimation = {
    //Boolean - If we want to override with a hard coded scale
    scaleOverride : true,
    //** Required if scaleOverride is true **
    //Number - The number of steps in a hard coded scale
    scaleSteps : 10,
    //Number - The value jump in the hard coded scale
    scaleStepWidth : 5,
    //Number - The scale starting value
    scaleStartValue : 0,
    responsive: true
  }
  
  // Not sure why the scaleOverride isn't working...
  var optionsNoAnimation = {
    animation : false,
    //Boolean - If we want to override with a hard coded scale
    scaleOverride : true,
    //** Required if scaleOverride is true **
    //Number - The number of steps in a hard coded scale
    scaleSteps : 20,
    //Number - The value jump in the hard coded scale
    scaleStepWidth : 10,
    //Number - The scale starting value
    scaleStartValue : 0,
    responsive: true,
  }
  
  //Get the context of the canvas element we want to select
  var ctxheart = document.getElementById("heartchart").getContext("2d");
  var optionsAnimation = {animation : false}
  var htchart = Chart.Line(ctxheart, {
    data: data,
    options: optionsAnimation
  });

  //myNewChart.Line(data, optionsAnimation);

  setInterval(function(){
  	var ctxheart = document.getElementById("heartchart").getContext("2d");
    updateHData(data);
    htchart = Chart.Line(ctxheart, {
    	data: data,
    	options: optionsNoAnimation
  	});
    ;}, 2000 );
    setInterval(recieveHeart, 2000);
});