var tempval;
$(document).ready(function(){
	// Chart.defaults.global.responsive = true;
  Chart.defaults.global.responsive = true;
  var count = 10;
  var data = {
	  labels : ["1","2","3","4","5", "6", "7", "8", "9", "10"],
		datasets : [
		  {
		  	label: "Temperature vs time",
            backgroundColor: "rgba(204, 0, 0,0.4)",
            borderColor: "rgba(204, 0, 0, 1)",
			data : [0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30]
			//data : [temp, temp, temp, temp, temp, temp, temp, temp, temp, temp, temp]
			}
		]
  };
  function recieveTemp() {
    $.get('http://arduino7.local/arduino/temp', function( e ){
      tempval = e;           
      $("#temp").text( tempval + " *C");
      console.log("temp" + e);
    });
  }  
  //this is ugly, don't judge me
    var updateTData = function(oldData){
    var labels = oldData["labels"];
    var tempdata = oldData["datasets"][0]["data"];
    
    labels.shift();
    count+=4;
    labels.push(count.toString());
    //var newDataA = tempdata[9] + (20 - Math.floor(Math.random() * (41)));
    var newDataA = tempval;
    tempdata.push(newDataA);
    tempdata.shift(); 
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
  var ctxtemp = document.getElementById("myChart").getContext("2d");
  var optionsAnimation = {animation : false}
  var myNewChart = Chart.Line(ctxtemp, {
    data: data,
    options: optionsAnimation
  });

  //myNewChart.Line(data, optionsAnimation);

  setInterval(function(){
  	var ctxtemp = document.getElementById("myChart").getContext("2d");
    updateTData(data);
    myNewChart = Chart.Line(ctxtemp, {
    	data: data,
    	options: optionsNoAnimation
  	});
    ;}, 4000 );
  setInterval(recieveTemp, 4000);
});

