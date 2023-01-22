var pictures = ["flap0.png", "flap45.png", "flap90.png", "flap135.png", "flap180.png"];
var gUpdateIntervalInMiliSec = 1000;
var index = 0;
var direction = -1;
var basePath = "https://tobiwern.github.io/FanFlapControl"
setInterval(updatePicture, 150);
preloadImages();

function updatePicture() {
    index += direction;
    if ((index >= 0) && (index < pictures.length - 1)) {
        innerHTML = '<img src="'+ basePath + '/pictures/' + pictures[index] + '" onclick="toggleDirection()" width="300">'
        document.getElementById("flap").innerHTML = innerHTML;
    }
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
        direction = -1;
    } else {
        direction = 1;
    }
    updatePicture();
}

function openFlap() {
    direction = 1;
    sendSettingsToESP();
    updatePicture();
}

function closeFlap() {
    direction = -1;
    sendSettingsToESP();
    updatePicture();
}

///// SERVER

function requestSettingsFromESP() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            value = this.responseText;
            document.getElementById("message").innerHTML = "<br>Value = " + value;
            if (value == "closed") {
                var index = pictures.length - 1;
                var direction = -1;
            } else {
                var index = 0;
                var direction = 1;
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
                document.getElementById("message").innerHTML = "<br>Lüfterklappe geschlossen";
            } else {
                document.getElementById("message").innerHTML = "<br>Lüfterklappe geöffnet";
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

