var pictures = ["flap0.png", "flap45.png", "flap90.png", "flap135.png", "flap180.png"];
var gUpdateIntervalInMiliSec = 1000;
var index = 0;
var direction = -1;
var basePath = "https://tobiwern.github.io/FanFlapControl"
const OPEN = 1;
const CLOSE = -1;
setInterval(updatePicture, 1000); //150);
preloadImages();

function updatePicture() {
    document.getElementById("message").innerHTML = "Index = " + index;
    if ((index >= 0) && (index < pictures.length - 1)) {
        innerHTML = '<img src="'+ basePath + '/pictures/' + pictures[index] + '" onclick="toggleDirection()" width="300" height="259">'
        document.getElementById("flap").innerHTML = innerHTML;
    }
    index += direction;
    if (index > pictures.length - 1) { index = pictures.length - 1; }
    if (index < 0) { index = 0; }
}

function preloadImages() {
    for (pic of pictures) {
        url = basePath + "/pictures/" + pic;
        new Image().src = url;
    }
}
function toggleDirection() {
    if (direction == 1) {
        direction = CLOSE;
    } else {
        direction = OPEN;
    }
    sendSettingsToESP();
}

function openFlap() {
    direction = OPEN;
    sendSettingsToESP();
}

function closeFlap() {
    direction = CLOSE;
    sendSettingsToESP();
}

///// SERVER

function requestSettingsFromESP() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            value = this.responseText;
            document.getElementById("message").innerHTML = "<br>Value = " + value;
            if (value == "closed") {
                index = 0; 
                direction = CLOSE;
            } else {
                index = pictures.length - 1;
                direction = OPEN;
            }
            updatePicture();
        }
    };
    xhttp.open("GET", "request_settings", true);
    xhttp.send();
}

function sendSettingsToESP() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            response = this.responseText;
            if (response == "closed") {
                document.getElementById("message").innerHTML = "Lüfterklappe geschlossen";
            } else {
                document.getElementById("message").innerHTML = "Lüfterklappe geöffnet";
            }
        }
    };
    if (direction == -1) {
        value = "close";
    } else {
        value = "open";
    }
    xhttp.open("GET", "set_flap?value=" + value, true);
    xhttp.send();
}

