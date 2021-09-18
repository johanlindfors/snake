// variables
SPRITE_SIZE = 20
SCREEN_SIZE = 20;
INITIAL_TAIL = 5;
FPS = 15

let { Sprite, GameLoop, initKeys } = kontra;
let { canvas, context } = kontra.init('surface');

let apple = Sprite({
    x: 3,
    y: 3,
    width: SPRITE_SIZE - 1,
    height: SPRITE_SIZE - 1,
    color: 'red',

    render: function() {
        this.context.fillStyle = this.color;
        this.context.fillRect(this.x * SPRITE_SIZE - this.x, this.y * SPRITE_SIZE - this.y, this.width, this.height);
    }
});

let snake = Sprite({
    x: 10,
    y: 10,
    width: SPRITE_SIZE - 1,
    height: SPRITE_SIZE - 1,
    color: 'green',

    tail: INITIAL_TAIL,
    trail: [],

    checkCollision: function() {
        this.trail.forEach(element => {
            if(element.x == this.position.x && element.y == this.position.y) {
                if(snake.tail > INITIAL_TAIL){
                    console.log("Scored: %i", this.tail - INITIAL_TAIL);
                }
                this.position.x = this.position.y = 10;
                this.velocity.x = this.velocity.y = 0;
                this.tail = INITIAL_TAIL;
            }
        });
    },

    update: function() {
        this.position.x = (this.position.x + this.velocity.x + SCREEN_SIZE) % SCREEN_SIZE;
        this.position.y = (this.position.y + this.velocity.y + SCREEN_SIZE) % SCREEN_SIZE;

        this.checkCollision();

        this.trail.push({x: this.position.x, y: this.position.y});
        while(this.trail.length > this.tail){
            this.trail.shift();
        }
    },

    render: function() {
        this.context.fillStyle = this.color;
        this.trail.forEach(element => {
            this.context.fillRect(element.x * SPRITE_SIZE - this.position.x, element.y * SPRITE_SIZE - this.position.y, this.width, this.height);
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
