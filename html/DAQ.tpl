<html><head><title>DAQ</title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
<div id="main">
<h1>Show voltages and stuff</h1>
<p>
Daq version %version%.
<br>

<script type="text/javascript">
    document.write("\<script src='//ajax.googleapis.com/ajax/libs/jquery/1/jquery.min.js' type='text/javascript'>\<\/script>");
</script>

<script type="text/javascript">
$(document).ready(function(){
$("#submit").click(function(){
	$.ajax({
		type: "POST",
		url: "DAQ.cgi",
		data: $("#cmd").serialize(),
		cache: false,
		success: function(result){
			alert(result);
			}	
		});
return false;
});
});
</script>




<form id="cmd">
Command name: <input type="text" name="command"><br>
Argument 1: <input type="text" name="arg1"><br>
Argument 2: <input type="text" name="arg2"><br>
Argument 3: <input type="text" name="arg3"><br>
Argument 4: <input type="text" name="arg4"><br>
<input id="submit" type="button" value="Submit"><br>
</form>


<table>
  <tr>
    <th>Command name</th>
    <th>Argument 1</th>		
    <th>Argument2</th>
    <th>Argument3</th>
    <th>Argument4</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>version</td>
    <td>-</td>		
    <td>-</td>
    <td>-</td>		
    <td>-</td>
    <td>Returns the version of the DAQ</td>		
  </tr>
  <tr>
    <td>read flash</td>
    <td>location[0-63]</td>		
    <td>-</td>
    <td>-</td>
    <td>-</td>
    <td>Reads 16 bytes stored at the specified location</td>
  </tr>

  <tr>
    <td>capacitance</td>
    <td>1/2/3/0 constant current-[0.55uA,10x,100x,1000x]</td>		
    <td>charging time [micro seconds]</td>
    <td>-</td>
    <td>-</td>
    <td>Measure capacitance on IN1</td>
  </tr>

  <tr>
    <td>set dac</td>
    <td>0/1 - channel number</td>		
    <td>0-4095 [value scales to +-6.6V]</td>
    <td>-</td>
    <td>-</td>
    <td>sets the voltage on the specified channel</td>
  </tr>

  <tr>
    <td>set gain</td>
    <td>1/2/3/4 - PGA number</td>		
    <td>0/1/2/3/4/5/6 gain[1x,2x,4x,8x,10x,16x,32x]</td>
    <td>-</td>
    <td>-</td>
    <td>sets the gain of the programmable amplifiers</td>
  </tr>

  <tr>
    <td>get voltage</td>
    <td>0/1/2/3/4/5/6/7   channel number</td>		
    <td>--</td>
    <td>-</td>
    <td>-</td>
    <td>Returns 12-bit ADC result from the selected channel</td>
  </tr>

  <tr>
    <td>start scope</td>
    <td>1-800   total samples</td>		
    <td>8-10000  delay between each sample(1/8 uS  .  8=>1uS)</td>
    <td>0-3  Trigger channel number</td>
    <td>0-4095 Trigger level</td>
    <td>starts the four channel oscilloscope. Fetch data using 'retrieve channel'</td>
  </tr>


  <tr>
    <td>scope status</td>
    <td>--</td>		
    <td>--</td>
    <td>--</td>
    <td>--</td>
    <td>returns [bool conversion finished, int samples acquired]</td>
  </tr>


  <tr>
    <td>retrieve channel</td>
    <td>0->3 Channel number</td>		
    <td>0->800 number of samples</td>
    <td>0->(800-samples)  starting position/offset</td>
    <td>--</td>
    <td>Fetches data stored in the oscilloscope buffer</td>
  </tr>



</table>





</div>

</body></html>



