window.onload = function() {
    surface = document.getElementById("surface");
    ctx = surface.getContext("2d");
    document.addEventListener("keydown",keyPush);
    setInterval(game,1000/15);
}

// variables
playerX=playerY=10;
spriteSize=20
screenSize=20;
appleX=appleY=3;
velocityX=velocityY=0;
trail=[];
tail = 5;

function update() {
    playerX += velocityX;
    playerY += velocityY;
    if(playerX<0){
        playerX = screenSize-1;
    }
    if(playerY<0){
        playerY = screenSize-1;
    }
    if(playerX>screenSize-1){
        playerX = 0;
    }
    if(playerY>screenSize-1){
        playerY = 0;
    }

    checkCollision();

    checkPickup();

    trail.push({x:playerX,y:playerY});
    while(trail.length>tail){
        trail.shift();
    }
}

function checkCollision() {
   for(var i=0; i<trail.length;i++){
        if(trail[i].x==playerX && trail[i].y==playerY){
            tail = 5;
            playerX=playerY=10;
            velocityX=velocityY=0;            
        }
    }
}

function checkPickup() {
    if(appleX==playerX && appleY==playerY){
        tail++;
        appleX = Math.floor(Math.random()*screenSize);
        appleY = Math.floor(Math.random()*screenSize);
    }
}

function draw() {
    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, surface.width, surface.height);

    ctx.fillStyle = "lime";
    for(var i=0; i<trail.length;i++){
        ctx.fillRect(trail[i].x*spriteSize+1, trail[i].y*spriteSize+1, spriteSize-1, spriteSize-1);
    }

    ctx.fillStyle = "red";
    ctx.fillRect(appleX*spriteSize + 1, appleY*spriteSize + 1, spriteSize-1, spriteSize-1);
}

function game() {
    update();
    draw();
}

function keyPush(evt){
    switch(evt.keyCode){
        case 37:
            if(velocityX == 0) {
                velocityX=-1;velocityY=0;
            }
            break;
        case 38:
            if(velocityY == 0) {
                velocityX=0;velocityY=-1;
            }
            break;
        case 39:
            if(velocityX == 0) {
                velocityX=1;velocityY=0;
            }
            break;
        case 40:
            if(velocityY == 0) {
                velocityX=0;velocityY=1;
            }
            break;
    }
}
