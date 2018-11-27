// variables
SPRITE_SIZE = 20
SCREEN_SIZE = 20
INITIAL_TAIL = 5
FPS = 15;

var surface;
var ctx;

let apple = {
    x: 3,
    y: 3,

    width: SPRITE_SIZE - 1,
    height: SPRITE_SIZE - 1,
    color: 'red',

    draw: function() {
        ctx.fillStyle = this.color;
        ctx.fillRect(this.x * SPRITE_SIZE + 1, this.y * SPRITE_SIZE + 1, this.width, this.height);
    }
};

let snake = {
    x: 10,
    y: 10,
    dx: 0,
    dy: 0,
    tail: INITIAL_TAIL,
    trail: [],

    width: SPRITE_SIZE - 1,
    height: SPRITE_SIZE - 1,
    color: 'green',

    checkCollision: function() {
        this.trail.forEach(element => {
            if(element.x == this.x && element.y == this.y) {
                if(snake.tail > INITIAL_TAIL){
                    console.log("Scored: %i", this.tail - INITIAL_TAIL);
                }
                this.x = this.y = 10;
                this.dx = this.dy = 0;
                this.tail = INITIAL_TAIL;        
            }
        });    
    },

    update: function() {
        this.x += this.dx;
        this.y += this.dy;
        this.x = this.x > SCREEN_SIZE -1 ? 0 : this.x < 0 ? SCREEN_SIZE - 1 : this.x;
        this.y = this.y > SCREEN_SIZE -1 ? 0 : this.y < 0 ? SCREEN_SIZE - 1 : this.y;
    
        this.checkCollision();
        
        this.trail.push({x:this.x, y:this.y});
        while(this.trail.length > this.tail){
            this.trail.shift();
        }
    },

    draw: function() {
        ctx.fillStyle = this.color;
        this.trail.forEach(element => {
            ctx.fillRect(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, this.width, this.height);
        });
    }
};

function keyPush(evt){
    if(snake.dx == 0) {
        switch(evt.keyCode){
            case 37:
                snake.dx = -1;
                snake.dy = 0;
                break;
            case 39:
                snake.dx = 1;
                snake.dy = 0;
                break;
        }
    }
    if(snake.dy == 0){
        switch(evt.keyCode){
            case 38:
            snake.dx = 0;
            snake.dy = -1;
            break;
        case 40:
            snake.dx = 0;
            snake.dy = 1;
            break;
        }
    }
}

function generateApple() {
    needNewApple = false;
    do {
        needNewApple = false;
        apple.x = Math.floor(Math.random() * SCREEN_SIZE);
        apple.y = Math.floor(Math.random() * SCREEN_SIZE);
        snake.trail.forEach(element => {
            needNewApple |= (element.x == apple.x && element.y == apple.y);
        });
    } while(needNewApple)
}

function checkPickup() {
    if(apple.x == snake.x && apple.y == snake.y){
        snake.tail++;
        generateApple();
    }
}

function update() {
    snake.update();
    checkPickup();
}

function draw() {
    ctx.fillStyle = "black";
    ctx.fillRect(0, 0, surface.width, surface.height);

    snake.draw();
    apple.draw();
}

function gameLoop() {
    update();
    draw();
}

window.onload = function() {
    surface = document.getElementById("surface");
    ctx = surface.getContext("2d");
    document.addEventListener("keydown",keyPush);
    setInterval(gameLoop, 1000 / FPS);
}
