kontra.init();
            
// variables
SPRITE_SIZE = 20
SCREEN_SIZE = 20;
INITIAL_TAIL = 5;
FPS = 15

let apple = kontra.sprite({
    x: 3,
    y: 3,

    // required for a rectangle sprite
    width: SPRITE_SIZE,
    height: SPRITE_SIZE,
    color: 'red',
    
    render: function() {
        this.context.fillStyle = this.color;
        this.context.fillRect(this.x * SPRITE_SIZE + 1, this.y * SPRITE_SIZE + 1, this.width - 1, this.height - 1);
    }
});

let snake = kontra.sprite({
    x: 10,
    y: 10,           
    tail: INITIAL_TAIL,
    trail: [],

    // required for a rectangle sprite
    width: SPRITE_SIZE,
    height: SPRITE_SIZE,
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
        this.x = this.x > SCREEN_SIZE -1 ? 0 : this.x < 0 ? SCREEN_SIZE - 1 : this.x;
        this.y = this.y > SCREEN_SIZE -1 ? 0 : this.y < 0 ? SCREEN_SIZE - 1 : this.y;                

        this.trail.push({ x: this.x, y: this.y});
        this.advance();
        
        while(this.trail.length > this.tail){
            this.trail.shift();
        }

        this.checkCollision();
    },
    
    render: function() {
        this.context.fillStyle = this.color;
        this.trail.forEach(element => {
            this.context.fillRect(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, this.width - 1, this.height - 1);
        });
    }
});

function checkInput() {
    if(snake.dx == 0){
        if(kontra.keys.pressed('left')) {
            snake.dx = -1;
            snake.dy = 0;
        }
        if(kontra.keys.pressed('right')) {
            snake.dx = 1;
            snake.dy = 0;
        }
    }
    if(snake.dy == 0) {
        if(kontra.keys.pressed('down')) {
            snake.dx = 0;
            snake.dy = 1;
        }
        if(kontra.keys.pressed('up')) {
            snake.dx = 0;
            snake.dy = -1;
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

let loop = kontra.gameLoop({
    fps: FPS,
    clearCanvas: true,

    update: function() {
        checkInput();
        checkPickup();
        snake.update();
    },

    render: function() {
        snake.render();
        apple.render();
    }
});

loop.start();
