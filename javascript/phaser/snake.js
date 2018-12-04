// variables
SPRITE_SIZE = 20
SCREEN_SIZE = 20
INITIAL_TAIL = 5
FPS = 15;

var config = {
    type: Phaser.AUTO,
    width: SCREEN_SIZE * SPRITE_SIZE,
    height: SCREEN_SIZE * SPRITE_SIZE,
    fps: { 
        target: FPS,
        forceSetTimeOut: false
    },
    scene: {
        create: create,
        update: update
    }
};

let apple = {
    x: 3,
    y: 3,
    graphics: null,
    create: function(game){
        this.graphics = game.add.graphics({ fillStyle: { color: 0xff0000 } });
    },
    
    update: function() {
        this.graphics.clear();
        this.graphics.fillRectShape({x: this.x * SCREEN_SIZE + 1, y: this.y * SCREEN_SIZE + 1, width: SPRITE_SIZE -1, height: SPRITE_SIZE - 1});
    }   
}

let snake = {
    x: 10,
    y: 10,
    dx: 0,
    dy: 0,
    tail: INITIAL_TAIL,
    trail: [],
    graphics: null,
    create: function(game){
        this.graphics = game.add.graphics({ fillStyle: { color: 0x00ff00 } });
    },

    update: function() {
        this.x = (this.x + this.dx + SCREEN_SIZE) % SCREEN_SIZE;
        this.y = (this.y + this.dy + SCREEN_SIZE) % SCREEN_SIZE;

        this.trail.push({x: this.x, y: this.y});
        while(this.trail.length > this.tail){
            this.trail.shift();
        }

        this.graphics.clear();
        this.graphics.fillRectShape({x: this.x * SCREEN_SIZE + 1, y: this.y * SCREEN_SIZE + 1, width: SPRITE_SIZE -1, height: SPRITE_SIZE - 1});
    },    
}

var snakeGame = new Phaser.Game(config);

function create () {
    apple.create(this);
    snake.create(this);
    snake.dx = 1;
}

function update () {
    snake.update();
    apple.update();
}