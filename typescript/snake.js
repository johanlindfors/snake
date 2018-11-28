// variables
var SPRITE_SIZE = 20;
var SCREEN_SIZE = 20;
var INITIAL_TAIL = 5;
var FPS = 15;
var surface;
var ctx;
var Point = (function () {
    function Point() {
    }
    return Point;
}());
var Apple = (function () {
    function Apple() {
        this.x = 3;
        this.y = 3;
        this.width = SPRITE_SIZE - 1;
        this.height = SPRITE_SIZE - 1;
        this.color = 'red';
    }
    Apple.prototype.draw = function () {
        ctx.fillStyle = this.color;
        ctx.fillRect(this.x * SPRITE_SIZE + 1, this.y * SPRITE_SIZE + 1, this.width, this.height);
    };
    return Apple;
}());
var Snake = (function () {
    function Snake() {
        this.x = 10;
        this.y = 10;
        this.dx = 0;
        this.dy = 0;
        this.tail = INITIAL_TAIL;
        this.trail = [];
        this.width = SPRITE_SIZE - 1;
        this.height = SPRITE_SIZE - 1;
        this.color = 'green';
    }
    Snake.prototype.checkCollision = function (x, y) {
        for (var i = 0; i < this.trail.length; i++) {
            if (this.trail[i].x == x && this.trail[i].y == y)
                return true;
        }
        return false;
    };
    Snake.prototype.update = function () {
        this.x += this.dx;
        this.y += this.dy;
        this.x = this.x > SCREEN_SIZE - 1 ? 0 : this.x < 0 ? SCREEN_SIZE - 1 : this.x;
        this.y = this.y > SCREEN_SIZE - 1 ? 0 : this.y < 0 ? SCREEN_SIZE - 1 : this.y;
        if (this.checkCollision(this.x, this.y)) {
            this.x = this.y = 10;
            this.dx = this.dy = 0;
            this.tail = INITIAL_TAIL;
        }
        this.trail.push({ x: this.x, y: this.y });
        while (this.trail.length > this.tail) {
            this.trail.shift();
        }
    };
    Snake.prototype.draw = function () {
        var _this = this;
        ctx.fillStyle = this.color;
        this.trail.forEach(function (element) {
            ctx.fillRect(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, _this.width, _this.height);
        });
    };
    return Snake;
}());
var SnakeGame = (function () {
    function SnakeGame() {
        this.apple = new Apple();
        this.snake = new Snake();
        document.addEventListener("keydown", this.keyPush);
        setInterval(this.gameLoop, 1000 / FPS);
    }
    SnakeGame.prototype.keyPush = function (evt) {
        if (this.snake.dx == 0) {
            switch (evt.keyCode) {
                case 37:
                    this.snake.dx = -1;
                    this.snake.dy = 0;
                    break;
                case 39:
                    this.snake.dx = 1;
                    this.snake.dy = 0;
                    break;
            }
        }
        if (this.snake.dy == 0) {
            switch (evt.keyCode) {
                case 38:
                    this.snake.dx = 0;
                    this.snake.dy = -1;
                    break;
                case 40:
                    this.snake.dx = 0;
                    this.snake.dy = 1;
                    break;
            }
        }
    };
    SnakeGame.prototype.generateApple = function () {
        do {
            this.apple.x = Math.floor(Math.random() * SCREEN_SIZE);
            this.apple.y = Math.floor(Math.random() * SCREEN_SIZE);
        } while (this.snake.checkCollision(this.apple.x, this.apple.y));
    };
    SnakeGame.prototype.update = function () {
        this.snake.update();
        if (this.snake.checkCollision(this.apple.x, this.apple.y)) {
            this.snake.tail++;
            this.generateApple();
        }
    };
    SnakeGame.prototype.draw = function () {
        ctx.fillStyle = "black";
        ctx.fillRect(0, 0, surface.width, surface.height);
        this.snake.draw();
        this.apple.draw();
    };
    SnakeGame.prototype.gameLoop = function () {
        this.update();
        this.draw();
    };
    return SnakeGame;
}());
window.onload = function () {
    surface = document.getElementById("surface");
    ctx = surface.getContext("2d");
    var game = new SnakeGame();
};
