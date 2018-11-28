// variables
const SPRITE_SIZE = 20
const SCREEN_SIZE = 20
const INITIAL_TAIL = 5
const FPS = 15;

var surface: any;
var ctx: any;

class Point {
    x: number;
    y: number;
}

class Apple {
    x: number = 3;
    y: number = 3;

    width: number = SPRITE_SIZE - 1;
    height: number = SPRITE_SIZE - 1;
    color: string = 'red';

    draw(): void {
        ctx.fillStyle = this.color;
        ctx.fillRect(this.x * SPRITE_SIZE + 1, this.y * SPRITE_SIZE + 1, this.width, this.height);
    }
}

class Snake {
    x: number = 10;
    y: number = 10;
    dx: number = 0;
    dy: number = 0;
    tail: number = INITIAL_TAIL;
    trail: Array<Point> = [];

    width: number = SPRITE_SIZE - 1;
    height: number = SPRITE_SIZE - 1;
    color: string = 'green';

    checkCollision(x: number, y: number): boolean {
        for(var i = 0; i <this.trail.length; i++){
            if(this.trail[i].x == x && this.trail[i].y == y)
                return true;
        }
        return false;
    }

    update(): void {
        this.x += this.dx;
        this.y += this.dy;
        this.x = this.x > SCREEN_SIZE -1 ? 0 : this.x < 0 ? SCREEN_SIZE - 1 : this.x;
        this.y = this.y > SCREEN_SIZE -1 ? 0 : this.y < 0 ? SCREEN_SIZE - 1 : this.y;
    
        if(this.checkCollision(this.x, this.y)){
            this.x = this.y = 10;
            this.dx = this.dy = 0;
            this.tail = INITIAL_TAIL;        
        }
        
        this.trail.push({x:this.x, y:this.y});
        while(this.trail.length > this.tail){
            this.trail.shift();
        }
    }

    draw(): void {
        ctx.fillStyle = this.color;
        this.trail.forEach(element => {
            ctx.fillRect(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, this.width, this.height);
        });
    }
}

class SnakeGame {
    apple: Apple;
    snake: Snake;

    constructor() {
        this.apple = new Apple();
        this.snake = new Snake();

        document.addEventListener("keydown", this.keyPush);
        setInterval(this.gameLoop, 1000 / FPS);
    }

    keyPush(evt) {
        if(this.snake.dx == 0) {
            switch(evt.keyCode){
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
        if(this.snake.dy == 0){
            switch(evt.keyCode){
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
    }

    generateApple(): void {
        do {
            this.apple.x = Math.floor(Math.random() * SCREEN_SIZE);
            this.apple.y = Math.floor(Math.random() * SCREEN_SIZE);
        } while(this.snake.checkCollision(this.apple.x, this.apple.y))
    }

    update() {
        this.snake.update();
        if(this.snake.checkCollision(this.apple.x, this.apple.y)){
            this.snake.tail++;
            this.generateApple();
        }
    }

    draw() {
        ctx.fillStyle = "black";
        ctx.fillRect(0, 0, surface.width, surface.height);

        this.snake.draw();
        this.apple.draw();
    }

     gameLoop() {
        this.update();
        this.draw();
    }
}

window.onload = function() {
    surface = document.getElementById("surface");
    ctx = surface.getContext("2d");
    let game = new SnakeGame();
}
