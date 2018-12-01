// variables
SPRITE_SIZE = 20
SCREEN_SIZE = 20;
INITIAL_TAIL = 5;
FPS = 15

kontra.init();

let apple = kontra.sprite({
    x: 3,
    y: 3,

    width: SPRITE_SIZE - 1,
    height: SPRITE_SIZE - 1,
    color: 'red',
    
    render: function() {
        this.context.fillStyle = this.color;
        this.context.fillRect(this.x * SPRITE_SIZE + 1, this.y * SPRITE_SIZE + 1, this.width, this.height);
    }
});

let snake = kontra.sprite({
    x: 10,
    y: 10,           
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
        this.advance();
        this.x = (this.x + SCREEN_SIZE) % SCREEN_SIZE;
        this.y = (this.y + SCREEN_SIZE) % SCREEN_SIZE;

        this.checkCollision();

        this.trail.push({x: this.x, y: this.y});        
        while(this.trail.length > this.tail){
            this.trail.shift();
        }
    },
    
    render: function() {
        this.context.fillStyle = this.color;
        this.trail.forEach(element => {
            this.context.fillRect(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, this.width, this.height);
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
        snake.update();
        checkPickup();
    },

    render: function() {
        kontra.context.fillStyle = 'black';
        kontra.context.fillRect(0, 0, kontra.canvas.width, kontra.canvas.height);
        
        snake.render();
        apple.render();
    }
});

loop.start();
