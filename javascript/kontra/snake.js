// variables
SPRITE_SIZE = 20
SCREEN_SIZE = 20;
INITIAL_TAIL = 5;
FPS = 15

let { Sprite, GameLoop, initKeys } = kontra;
let { canvas, context } = kontra.init('surface');

let apple = Sprite({
    X: 3,
    Y: 3,
    width: SPRITE_SIZE - 1,
    height: SPRITE_SIZE - 1,
    color: 'red',

    render: function() {
        this.context.fillStyle = this.color;
        this.context.fillRect(this.X * SPRITE_SIZE, this.Y * SPRITE_SIZE, this.width, this.height);
    }
});

let snake = Sprite({
    X: 10,
    Y: 10,
    width: SPRITE_SIZE - 1,
    height: SPRITE_SIZE - 1,
    color: 'green',

    tail: INITIAL_TAIL,
    trail: [],

    checkCollision: function() {
        this.trail.forEach(element => {
            if(element.X == this.X && element.Y == this.Y) {
                if(snake.tail > INITIAL_TAIL){
                    console.log("Scored: %i", this.tail - INITIAL_TAIL);
                }
                this.X = this.Y = 10;
                this.velocity.x = this.velocity.y = 0;
                this.tail = INITIAL_TAIL;
            }
        });
    },

    update: function() {
        this.X = (this.X + this.velocity.x + SCREEN_SIZE) % SCREEN_SIZE;
        this.Y = (this.Y + this.velocity.y + SCREEN_SIZE) % SCREEN_SIZE;

        this.checkCollision();

        this.trail.push({X: this.X, Y: this.Y});
        while(this.trail.length > this.tail){
            this.trail.shift();
        }
    },

    render: function() {
        this.context.fillStyle = this.color;
        this.trail.forEach(element => {
            this.context.fillRect(element.X * SPRITE_SIZE, element.Y * SPRITE_SIZE, this.width, this.height);
        });
    }
});

function checkInput() {
    if(snake.velocity.x == 0){
        if(kontra.keyPressed('left')) {
            snake.velocity.x = -1;
            snake.velocity.y = 0;
        }
        if(kontra.keyPressed('right')) {
            snake.velocity.x = 1;
            snake.velocity.y = 0;
        }
    }
    if(snake.velocity.y == 0) {
        if(kontra.keyPressed('down')) {
            snake.velocity.x = 0;
            snake.velocity.y = 1;
        }
        if(kontra.keyPressed('up')) {
            snake.velocity.x = 0;
            snake.velocity.y = -1;
        }
    }
}

function generateApple() {
    needNewApple = false;
    do {
        needNewApple = false;
        apple.X = Math.floor(Math.random() * SCREEN_SIZE);
        apple.Y = Math.floor(Math.random() * SCREEN_SIZE);
        snake.trail.forEach(element => {
            needNewApple |= (element.X == apple.X && element.Y == apple.Y);
        });
    } while(needNewApple)
}

function checkPickup() {
    if(apple.X == snake.X && apple.Y == snake.Y){
        snake.tail++;
        generateApple();
    }
}

let loop = GameLoop({
    fps: FPS,
    clearCanvas: true,

    update: function() {
        checkInput();
        snake.update();
        checkPickup();
    },

    render: function() {
        apple.render();
        snake.render();
    }
});

initKeys();
loop.start();
