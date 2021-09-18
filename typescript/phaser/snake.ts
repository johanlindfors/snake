/// <reference path="matter.d.ts"/>
/// <reference path="phaser.d.ts"/>

let Constants = {
    SPRITE_SIZE: 20,
    SCREEN_SIZE: 20,
    FRAMES_PER_SECOND: 15,
    INITIAL_TAIL: 5
};

class BodyPosition {
    x: number;
    y: number;
}

class Queue<T> {
    _store: T[] = [];
    push(val: T) {
      this._store.push(val);
    }
    pop(): T | undefined {
      return this._store.shift();
    }
    length(): number {
        return this._store.length;
    }
}

class Apple extends Phaser.GameObjects.GameObject {
    x: number;
    y: number;
    graphics: Phaser.GameObjects.Graphics;

    constructor(scene, x: number, y: number) {
        super(scene, "apple");
        this.x = x;
        this.y = y;
        this.graphics = scene.add.graphics();
        scene.add.existing(this);
    }

    update() {
        this.graphics.clear();
        this.graphics.fillStyle(0xff0000);
        this.graphics.fillRect(
            this.x * Constants.SCREEN_SIZE + 1,
            this.y * Constants.SCREEN_SIZE + 1,
            Constants.SPRITE_SIZE -1,
            Constants.SPRITE_SIZE - 1
        );
    }
}

class Snake extends Phaser.GameObjects.GameObject {
    x: number;
    y: number;
    dx: number;
    dy: number;
    tail: number;
    trail: Array<BodyPosition>;
    graphics: Phaser.GameObjects.Graphics;

    constructor(scene: Phaser.Scene, x: number, y: number) {
        super(scene, "snake");
        this.x = x;
        this.y = y;
        this.dx = 0;
        this.dy = 0;
        this.tail = Constants.INITIAL_TAIL;
        this.trail = Array<BodyPosition>();

        this.graphics = scene.add.graphics();
        scene.add.existing(this);
    }

    checkCollision(x: number, y: number) : boolean {
        let result: boolean = false;
        for(let element of this.trail) {
            if(element.x == x && element.y == y) {
                result = true;
            }
        }
        return result;
    }

    update() {
        this.x = (this.x + this.dx + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;
        this.y = (this.y + this.dy + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;

        if(this.checkCollision(this.x, this.y)){
            this.tail = Constants.INITIAL_TAIL;
            this.x = 10;
            this.y = 10;
            this.dx = 0;
            this.dy = 0;
        }

        this.trail.push({x: this.x, y: this.y});
        while(this.trail.length > this.tail){
            this.trail.shift();
        }

        this.graphics.clear();
        this.graphics.fillStyle(0x00ff00);
        for(let element of this.trail) {
            this.graphics.fillRect(
                element.x * Constants.SCREEN_SIZE + 1,
                element.y * Constants.SCREEN_SIZE + 1,
                Constants.SPRITE_SIZE -1,
                Constants.SPRITE_SIZE - 1
            );
        };
    }
}

class SnakeGameScene extends Phaser.Scene {
    apple: Apple;
    snake: Snake;

    constructor() {
        super({
            key: "SnakeGameScene"
        });
    }

    create() : void {
        this.snake = new Snake(this, 10, 10);
        this.generateApple();
    }

    generateApple() {
        if(this.apple == undefined) {
            this.apple = new Apple(this, 3, 3);
        } else {
            do {
                this.apple.x = Phaser.Math.RND.between(0, Constants.SCREEN_SIZE - 1);
                this.apple.y = Phaser.Math.RND.between(0, Constants.SCREEN_SIZE - 1);
            } while(this.snake.checkCollision(this.apple.x, this.apple.y));
        }
        this.apple.update();
    }

    update(time) : void {
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

        if(this.snake.checkCollision(this.apple.x, this.apple.y)) {
            this.snake.tail += 1;
            this.generateApple();
        }
    }
}

const config: Phaser.Types.Core.GameConfig = {
    type: Phaser.AUTO,
    width: Constants.SCREEN_SIZE * Constants.SPRITE_SIZE,
    height: Constants.SCREEN_SIZE * Constants.SPRITE_SIZE,
    scene: [SnakeGameScene],
    fps: { target: Constants.FRAMES_PER_SECOND },
    input: {
        keyboard: true,
        mouse: false,
        touch: false,
        gamepad: false
    },
    backgroundColor: "#000000",
};

class SnakeGame extends Phaser.Game {
    constructor(config: Phaser.Types.Core.GameConfig) {
        super(config);
    }
}

window.onload = () => {
    var game = new SnakeGame(config);
};
