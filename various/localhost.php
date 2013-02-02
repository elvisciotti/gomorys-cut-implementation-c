<?
$EXE = "Release\\ot.exe";



/*
gcc -O  -c *.c
gcc -O  *.o -o ot.exe


*/

if ($_POST)
{
	$e = array();
  
  if (!$_POST['comand'])
  {
    if (!intval($_POST['l']))
      $e[] = "larghezza non specificata";
    if (!intval($_POST['h']))
      $e[] = "altezza non specificata";
    if (!intval($_POST['m']) && $_POST['random'])
      $e[] = "numero pezzi non specificato";
      
    if (!$_POST['random'])  
    {
      $valori = str_replace("\n"," ", trim($_POST['p']));
      $valori = str_replace("  "," ", $valori);
      $valori = str_replace("  "," ", $valori);
      $valori = str_replace("  "," ", $valori);
      $valori = array_filter(explode(" ", $valori));
      
      //controllo che siano multili di 3
      if (count($valori) % 3 != 0)
        $e[] = "Pezzi non validi !";
      
      $_POST['m'] = count($valori) / 3;
      
    }
      
  }
  else
  {
    if (  0
        /*strpos($_POST['comand'], "--l")==false ||
        strpos($_POST['comand'], "--h")==false ||
        strpos($_POST['comand'], "--m")==false */
      )
      $e[] = "altezza, larghezza o #pezzi non specificato nella riga di comando";
  }
  
  if ($e)
  {
    ?><b>ERRORE INSERIMENTO: </b><br><?=implode("<br>", $e)?>
    <p><a href="javascript:history.back()">Indietro</a></p><?
    exit;
  }
  
  
  
  $nomeFile = sprintf("out_%dx%d_%dpezzi_%s.html", 
                          $_POST['l'],
                          $_POST['h'],
                          $_POST['m'],
                          time().ceil(microtime()*100000)
                          );
  //print "<pre>";
  if ($_POST['comand'])
  {
    $COMANDO = str_replace("output", "output2", $_POST['comand']);
  }
  else
  {
    $COMANDO = sprintf("--l %d --h %d --m %d %s %s",
                          $_POST['l'],
                          $_POST['h'],
                          $_POST['m'],
                          $_POST['random'] ? "":" --p ".implode(" ", $valori)."",
                          $_POST['pa'] ? " --pa ":""
                        );
  }
  
  $COMANDO = "$EXE $COMANDO --output $nomeFile";
  exec($COMANDO);
  system($COMANDO);
  passthru($COMANDO);
  //pcntl_exec($COMANDO);
  //header("Location:  $nomeFile");
  
  print '<hr>Calcolo effettuato: <br><a href="'.$nomeFile.'">'.$nomeFile.'</a><br />Comando eseguito: '.$COMANDO; 
  if (!file_exists($nomeFile)) print "<br /><font color=red>FILE NON PRESENTE !</font>";
 // else
  //  print 'Impossibile leggere il risultato !<br>('.$COMANDO.')';
  
	exit;
}

 ?>
<form action="<?=$pagina_corrente?>" method="post" name="form">
<table width="500" border="1" cellspacing="5" cellpadding="5">
  <tr>
    <td width="127"><strong>Piano di taglio</strong></td>
    <td width="373"><table border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td width="80" valign="middle" nowrap="nowrap">H=
          <input name="h" type="text" id="h" value="11" size="6" maxlength="6" /></td>
        <td bgcolor="#999999" height="50" width="126">&nbsp;</td>
      </tr>
      <tr>
        <td width="80">&nbsp;</td>
        <td align="center">L=
          <input name="l" type="text" id="l" value="17" size="7" maxlength="7" /></td>
      </tr>
    </table>      </td>
  </tr>
  <tr>
    <td><strong>Numero pezzi </strong></td>
    <td><input name="m" type="text" id="m" value="2" size="7" maxlength="7" onkeyup="this.form.p.size=this.value"/></td>
  </tr>
  <tr>
    <td><strong>Dimensioni pezzi </strong></td>
    <td>
	<label>
     <input name="random" type="radio"  value="1" />
     Pezzi casuali</label>
     <br>&nbsp;&nbsp;&nbsp;<label><input name="pa" type="checkbox" id="pa" value="checkbox" /> Profitto proporzionale all'area</label> 
   <br />
   <label>
     <input name="random" type="radio" value="0" checked="checked"  />
     Specifica dimensioni</label>
	:<br />
    <textarea name="p" cols="20" rows="10" id="p">5 5 26
2 2 1</textarea>
    <br />
    un pezzo per ogni linea nel formato:<br />
    &lt;larghezza&gt; &lt;altezza&gt; &lt;profitto&gt;<br />
    esempio (pezzo 200x300 con profitto 40 e pezzo 300x400 con profitto50): <br />
    <em>200 300 40<br />
    300 400 50
    </label>
    </em></td>
  </tr>

  <tr>
    <td><strong>Riga di comando </strong></td>
    <td><label>Comando:
        <input name="comand" type="text" id="comand" size="40" />
        <br />
    Se specificato, annulla tutte le opzioni sopra</label></td>
  </tr>
</table>

 <br>
 <input type=submit value="calcola">
</form>

<h2>Risultati già calcolati</h2>
<?
$handle=opendir(".");

$arFiles = array();
while (($file = readdir($handle))!==false)
{
	
	if (	substr($file,0,3) == "out"   )
	{
		?><a href="<?=$file?>"><?=$file?></a><br /><?
	}
}
closedir($handle);  ?>