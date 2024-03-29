var gPictures = ["flap0_300.png", "flap45_300.png", "flap90_300.png", "flap135_300.png", "flap180_300.png"];
var gUpdateIntervalInMiliSec = 150;
var gIndex = 0;
var gDirection = -1;
var gBasePath = "https://tobiwern.github.io/FanFlapControl"
const OPEN = 1;
const CLOSE = -1;
setInterval(updatePicture, gUpdateIntervalInMiliSec); 
setInterval(requestSettingsFromESP, 1000*5); 
preloadImages();

function updatePicture() {
    if ((gIndex >= 0) && (gIndex < gPictures.length)) {
        innerHTML = '<img src="'+ gBasePath + '/pictures/' + gPictures[gIndex] + '" onclick="toggleDirection()" width="300" height="259">'
        document.getElementById("flap").innerHTML = innerHTML;
    }
    gIndex += gDirection;
    if (gIndex > gPictures.length - 1) { gIndex = gPictures.length - 1; }
    if (gIndex < 0) { gIndex = 0; }
}

function preloadImages() {
    for (pic of gPictures) {
        url = gBasePath + "/pictures/" + pic;
        new Image().src = url;
    }
}
function toggleDirection() {
    var direction;
    if (gDirection == 1) {
        direction = CLOSE;
    } else {
        direction = OPEN;
    }
    sendSettingsToESP(direction);
//    updatePicture();
}

function openFlap() {
//    gDirection = OPEN;
    sendSettingsToESP(OPEN);
//    updatePicture();
}

function closeFlap() {
//    gDirection = CLOSE;
    sendSettingsToESP(CLOSE);
//    updatePicture();
}

///// SERVER

function requestSettingsFromESP() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            value = this.responseText;
//            document.getElementById("message").innerHTML = "<br>Value = " + value;
            if (value == "closed") {
                gIndex = 0; 
                gDirection = CLOSE;
                document.getElementById("message").innerHTML = "Lüfterklappe geschlossen";
            } else {
                gIndex = gPictures.length - 1;
                gDirection = OPEN;
                document.getElementById("message").innerHTML = "Lüfterklappe geöffnet";
            }
            updatePicture();
        }
    };
    xhttp.open("GET", "request_settings", true);
    xhttp.send();
}

function sendSettingsToESP(direction) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            response = this.responseText;
            if (response == "closed") {
                document.getElementById("message").innerHTML = "Lüfterklappe geschlossen";
                gDirection = CLOSE;
            } else {
                document.getElementById("message").innerHTML = "Lüfterklappe geöffnet";
                gDirection = OPEN;
            }
        }
    };
    if (direction == CLOSE) {
        value = "close";
    } else {
        value = "open";
    }
    xhttp.open("GET", "set_flap?value=" + value, true);
    xhttp.send();
}

