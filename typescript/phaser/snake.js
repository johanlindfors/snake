/// <reference path="matter.d.ts"/>
/// <reference path="phaser.d.ts"/>
var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
var Constants = {
    SPRITE_SIZE: 20,
    SCREEN_SIZE: 20,
    FRAMES_PER_SECOND: 15,
    INITIAL_TAIL: 5
};
var BodyPosition = /** @class */ (function () {
    function BodyPosition() {
    }
    return BodyPosition;
}());
var Queue = /** @class */ (function () {
    function Queue() {
        this._store = [];
    }
    Queue.prototype.push = function (val) {
        this._store.push(val);
    };
    Queue.prototype.pop = function () {
        return this._store.shift();
    };
    Queue.prototype.length = function () {
        return this._store.length;
    };
    return Queue;
}());
var Apple = /** @class */ (function (_super) {
    __extends(Apple, _super);
    function Apple(scene, x, y) {
        var _this = _super.call(this, scene, "apple") || this;
        _this.x = x;
        _this.y = y;
        _this.graphics = scene.add.graphics();
        scene.add.existing(_this);
        return _this;
    }
    Apple.prototype.update = function () {
        this.graphics.clear();
        this.graphics.fillStyle(0xff0000);
        this.graphics.fillRect(this.x * Constants.SCREEN_SIZE + 1, this.y * Constants.SCREEN_SIZE + 1, Constants.SPRITE_SIZE - 1, Constants.SPRITE_SIZE - 1);
    };
    return Apple;
}(Phaser.GameObjects.GameObject));
var Snake = /** @class */ (function (_super) {
    __extends(Snake, _super);
    function Snake(scene, x, y) {
        var _this = _super.call(this, scene, "snake") || this;
        _this.x = x;
        _this.y = y;
        _this.dx = 0;
        _this.dy = 0;
        _this.tail = Constants.INITIAL_TAIL;
        _this.trail = Array();
        _this.graphics = scene.add.graphics();
        scene.add.existing(_this);
        return _this;
    }
    Snake.prototype.checkCollision = function (x, y) {
        var result = false;
        for (var _i = 0, _a = this.trail; _i < _a.length; _i++) {
            var element = _a[_i];
            if (element.x == x && element.y == y) {
                result = true;
            }
        }
        return result;
    };
    Snake.prototype.update = function () {
        this.x = (this.x + this.dx + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;
        this.y = (this.y + this.dy + Constants.SCREEN_SIZE) % Constants.SCREEN_SIZE;
        if (this.checkCollision(this.x, this.y)) {
            this.tail = Constants.INITIAL_TAIL;
            this.x = 10;
            this.y = 10;
            this.dx = 0;
            this.dy = 0;
        }
        this.trail.push({ x: this.x, y: this.y });
        while (this.trail.length > this.tail) {
            this.trail.shift();
        }
        this.graphics.clear();
        this.graphics.fillStyle(0x00ff00);
        for (var _i = 0, _a = this.trail; _i < _a.length; _i++) {
            var element = _a[_i];
            this.graphics.fillRect(element.x * Constants.SCREEN_SIZE + 1, element.y * Constants.SCREEN_SIZE + 1, Constants.SPRITE_SIZE - 1, Constants.SPRITE_SIZE - 1);
        }
        ;
    };
    return Snake;
}(Phaser.GameObjects.GameObject));
var SnakeGameScene = /** @class */ (function (_super) {
    __extends(SnakeGameScene, _super);
    function SnakeGameScene() {
        return _super.call(this, {
            key: "SnakeGameScene"
        }) || this;
    }
    SnakeGameScene.prototype.create = function () {
        this.snake = new Snake(this, 10, 10);
        this.generateApple();
    };
    SnakeGameScene.prototype.generateApple = function () {
        if (this.apple == undefined) {
            this.apple = new Apple(this, 3, 3);
        }
        else {
            do {
                this.apple.x = Phaser.Math.RND.between(0, Constants.SCREEN_SIZE - 1);
                this.apple.y = Phaser.Math.RND.between(0, Constants.SCREEN_SIZE - 1);
            } while (this.snake.checkCollision(this.apple.x, this.apple.y));
        }
        this.apple.update();
    };
    SnakeGameScene.prototype.update = function (time) {
        var cursors = this.input.keyboard.createCursorKeys();
        if (this.snake.dx == 0) {
            if (cursors.left.isDown) {
                this.snake.dx = -1;
                this.snake.dy = 0;
            }
            else if (cursors.right.isDown) {
                this.snake.dx = 1;
                this.snake.dy = 0;
            }
        }
        if (this.snake.dy == 0) {
            if (cursors.up.isDown) {
                this.snake.dx = 0;
                this.snake.dy = -1;
            }
            else if (cursors.down.isDown) {
                this.snake.dx = 0;
                this.snake.dy = 1;
            }
        }
        this.snake.update();
        if (this.snake.checkCollision(this.apple.x, this.apple.y)) {
            this.snake.tail += 1;
            this.generateApple();
        }
    };
    return SnakeGameScene;
}(Phaser.Scene));
var config = {
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
    backgroundColor: "#000000"
};
var SnakeGame = /** @class */ (function (_super) {
    __extends(SnakeGame, _super);
    function SnakeGame(config) {
        return _super.call(this, config) || this;
    }
    return SnakeGame;
}(Phaser.Game));
window.onload = function () {
    var game = new SnakeGame(config);
};
