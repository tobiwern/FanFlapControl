const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset='utf-8'>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Lüfterklappensteuerung</title>
    <meta name="robots" content="noindex">
    <link rel="stylesheet" href="https://tobiwern.github.io/FanFlapControl/FanFlapControl.css">
    <script src="https://tobiwern.github.io/FanFlapControl/FanFlapControl.js?random=<?php echo filemtime('https://tobiwern.github.io/FanFlapControl/FanFlapControl.js'); ?>"></script> 
</head>

<body id='body' onload="requestSettingsFromESP()">
    <h1>Lüfterklappensteuerung</h1>
    <div class="frame">
        <div id="flap" class="flap"></div>
        <div id="message"></div>
    </div>
    <br>
    <div class=frame>
        <h2>
            <div class='centeredHeight'><img src='https://github.com/tobiwern/FanFlapControl/blob/main/pictures/github.svg?raw=true'> Bedienungsanleitung</div>
        </h2>
        Mehr Infos zur Lüfterklappensteuerung gibt es unter <a href='https://tobiwern.github.io/FanFlapControl/' target='_blank'>https://tobiwern.github.io/FanFlapControl/</a>
        <br><br>
    </div>
    <br>
    <div>
        <button class="button" onclick="openFlap()">Öffnen</button>
        <button class="button" onclick="closeFlap()">Schließen</button>
    </div>
</body>
</html>
)=====";
