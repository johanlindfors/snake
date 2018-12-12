// variables
SPRITE_SIZE = 20
SCREEN_SIZE = 20
INITIAL_TAIL = 5
FRAMES_PER_SECOND = 15;

class Snake extends Phaser.GameObjects.GameObject {

    constructor(scene) {
        super(scene);
        this.graphics = scene.add.graphics({ fillStyle: { color: 0x00ff00 } });
        this.x = 10;
        this.y = 10;
        this.dx = 0;
        this.dy = 0;
        this.tail = INITIAL_TAIL;
        this.trail = [];
    }

    checkCollision(x, y) {
        let result = false;
        this.trail.forEach(element => {
            if(element.x == x && element.y == y)
                result = true;
        });
        return result;
    }

    update() {
        this.x = (this.x + this.dx + SCREEN_SIZE) % SCREEN_SIZE;
        this.y = (this.y + this.dy + SCREEN_SIZE) % SCREEN_SIZE;

        if(this.checkCollision(this.x, this.y)){
            this.tail = INITIAL_TAIL;
            this.x = 10;
            this.y = 10;
            this.dx = 0;
            this.dy = 0;
            return;
        }

        this.trail.push({x: this.x, y: this.y});
        while(this.trail.length > this.tail){
            this.trail.shift();
        }

        this.graphics.clear();
        this.trail.forEach(element => {
            this.graphics.fillRectShape({x: element.x * SCREEN_SIZE + 1, y: element.y * SCREEN_SIZE + 1, width: SPRITE_SIZE -1, height: SPRITE_SIZE - 1});
        });
    }
}

class Apple extends Phaser.GameObjects.GameObject {

    constructor(scene) {
        super(scene);
        this.graphics = scene.add.graphics({ fillStyle: { color: 0xff0000 } });
        this.x = 3;
        this.y = 3;
        this.update();
    }

    update() {
        this.graphics.clear();
        this.graphics.fillRectShape({x: this.x * SCREEN_SIZE + 1, y: this.y * SCREEN_SIZE + 1, width: SPRITE_SIZE -1, height: SPRITE_SIZE - 1});
    }
}

class SnakeGameScene extends Phaser.Scene {

    constructor() {
        super("snakeGame");
    }

    create () {
        this.apple = new Apple(this);
        this.add.existing(this.apple);

        this.snake = new Snake(this);
        this.add.existing(this.snake);

        this.physics.world.setFPS(FRAMES_PER_SECOND);
    }

    update () {
        let cursors = this.input.keyboard.createCursorKeys();
        if(this.snake.dx == 0) {
            if (cursors.left.isDown) {
                this.snake.dx = -1;
                this.snake.dy = 0;
            } else if (cursors.right.isDown) {
                this.snake.dx = 1;
                this.snake.dy = 0;
            }
        }
        if(this.snake.dy == 0 ) {
            if (cursors.up.isDown) {
                this.snake.dx = 0;
                this.snake.dy = -1;
            } else if (cursors.down.isDown) {
                this.snake.dx = 0;
                this.snake.dy = 1;
            }
        }

        this.snake.update();
        
        if(this.snake.checkCollision(this.apple.x, this.apple.y)){
            this.snake.tail += 1;
            this.generateApple();
        }
    }

    generateApple() {
        do {
            this.apple.x = Math.floor(Math.random() * SCREEN_SIZE);
            this.apple.y = Math.floor(Math.random() * SCREEN_SIZE);
        } while(this.snake.checkCollision(this.apple.x, this.apple.y))
        this.apple.update();
    }
}

window.onload = function() {
    var config = {
        type: Phaser.AUTO,
        width: SCREEN_SIZE * SPRITE_SIZE,
        height: SCREEN_SIZE * SPRITE_SIZE,
        fps: {
            target: FRAMES_PER_SECOND,
        },
        physics: {
            default: "arcade",
            arcade: {
                fps: FRAMES_PER_SECOND
            }
        },
        scene: [ SnakeGameScene ],
    };

    let snakeGame = new Phaser.Game(config);
}
