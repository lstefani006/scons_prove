<html>
	<head>
	<script src='jquery-ui-1.10.0/external/jquery/jquery.js"></script>
	<script src='jquery-ui-1.10.0/jquery-ui.js"></script>
	<title>Hello World</title> 
	<body>     

<select id="selectmenu">
	<option>Slower</option>
	<option>Slow</option>
	<option selected="selected">Medium</option>
	<option>Fast</option>
	<option>Faster</option>
</select>


<?php 
for ($a = 0; $a < 10; ++$a)
	echo "\t\t<p id='p$a'>Ciao, Luca! $a</p>\n";
?> 

<button id='b'>Ciao</button>
<script>
$('#b').click(id_click)
	function id_click() {
		console.log('ciao\n')
		$('#p1').hide()
	}

$( "#selectmenu" ).selectmenu();



</script>
	</body> 
</html> 
