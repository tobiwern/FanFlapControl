var pictures = ["flap0.png", "flap45.png", "flap90.png", "flap135.png", "flap180.png"];
var gUpdateIntervalInMiliSec = 1000;
var index = 0;
var direction = -1;
setInterval(updatePicture, 150);
preloadImages();

function updatePicture() {
    index += direction;
    if ((index >= 0) && (index < pictures.length-1)) {
        innerHTML = '<img src="./pictures/' + pictures[index] + '" onclick="toggleDirection()" width="300">'
        document.getElementById("flap").innerHTML = innerHTML;
 //       new Promise((updatePicture) => {
 //           setTimeout(updatePicture, 1000);
 //        })
//        const promise = setTimeout(updatePicture(), 1000).then(updatePicture, null);

    }
    if(index > pictures.length-1){index = pictures.length-1;}
    if(index < 0 ){index = 0;}
}

function preloadImages(){
    for(pic of pictures){
        url = "./pictures/" + pic;
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
    updatePicture();
}

function closeFlap() {
    direction = -1;
    updatePicture();
}
