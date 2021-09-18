// variables
SPRITE_SIZE = 20
SCREEN_SIZE = 20
INITIAL_TAIL = 5
FPS = 15;

var surface;
var ctx;

let Apple = {
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

let Snake = {
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
        this.x = (this.x + this.dx + SCREEN_SIZE) % SCREEN_SIZE;
        this.y = (this.y + this.dy + SCREEN_SIZE) % SCREEN_SIZE;

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

let game = {
    snake: Snake,
    apple: Apple,

    generateApple: function() {
        needNewApple = false;
        do {
            needNewApple = false;
            this.apple.x = Math.floor(Math.random() * SCREEN_SIZE);
            this.apple.y = Math.floor(Math.random() * SCREEN_SIZE);
            this.snake.trail.forEach(element => {
                needNewApple |= (element.x == this.apple.x && element.y == this.apple.y);
            });
        } while(needNewApple)
    },

    checkPickup: function() {
        if(this.apple.x == this.snake.x && this.apple.y == this.snake.y){
            this.snake.tail++;
            this.generateApple();
        }
    },

    update: function() {
        this.snake.update();
        this.checkPickup();
    },

    draw: function() {
        ctx.fillStyle = "black";
        ctx.fillRect(0, 0, surface.width, surface.height);

        this.snake.draw();
        this.apple.draw();
    },
}

function keyPush(evt){
    if(game.snake.dx == 0) {
        switch(evt.keyCode){
            case 37:
                game.snake.dx = -1;
                game.snake.dy = 0;
                break;
            case 39:
                game.snake.dx = 1;
                game.snake.dy = 0;
                break;
        }
    }
    if(game.snake.dy == 0){
        switch(evt.keyCode){
            case 38:
                game.snake.dx = 0;
                game.snake.dy = -1;
                break;
            case 40:
                game.snake.dx = 0;
                game.snake.dy = 1;
                break;
        }
    }
}

function gameLoop() {
    game.update();
    game.draw();
}

window.onload = function() {
    surface = document.getElementById("surface");
    ctx = surface.getContext("2d");
    document.addEventListener("keydown", keyPush);
    setInterval(gameLoop, 1000 / FPS);
}
