<html><head><title>DAQ</title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
<div id="main">
<h1>cro?</h1>
<p>

<br>

<script src="http://ajax.googleapis.com/ajax/libs/jquery/2.1.1/jquery.min.js"></script>
<script src="jquery.flot.js"></script>


<script type="text/javascript">
$(document).ready(function(){


var options={
	        lines: { show: true },
			series: {   lines: { show: true ,lineWidth:2}     },
			points: {show: false },
			grid: { hoverable: false, autoHighlight: false,clickable: true, color:'black',backgroundColor: { colors: ["#100", "#344"] } },
	    }  ; 

var plot = $.plot($('#plotarea'),[ ],options);
var timegap=14;
var samples=200;
function capture_data(){
	$.ajax({
		type: "POST",
		url: "DAQ.cgi",
		async:false,
		data: "command=start scope&arg1="+samples+"&arg2="+timegap+"&arg3=0&arg4=100",
		cache: false,
		success: function(result){
			console.log(result);
			}	
		});
};


function load_data(){
	$.ajax({
		type: "POST",
		async:false,
		url: "DAQ.cgi",
		data: "command=retrieve channel&arg1=1&arg2="+samples+"&arg3=0",
		cache: false,
		success: function(result){
				      var elements = [];
				      var e=$.parseJSON(result);
					for(i=0;i<e.length;i++) elements.push([i*timegap,e[i]]);
		$.plot("#plotarea", [{data: elements,color:'#ff5599',	lines: { show: true, fill: false }}],options);
			}	
		});

}


$("#submit").click(function(){capture_data();load_data();});

/*
function autorefresh(){
capture_data();
load_data();
setTimeout(autorefresh,1000);

};
autorefresh();
*/

});
</script>




 <div id="plotarea" style="color:#0000FF;width:max-width;height:400px">
</div> 

<input id="submit" type="button" value="CAPTURE">



</div>




</body></html>



