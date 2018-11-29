// variables
const SPRITE_SIZE = 20
const SCREEN_SIZE = 20
const INITIAL_TAIL = 5
const FPS = 15;

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

    draw(ctx: CanvasRenderingContext2D) : void {
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

    checkCollision(x: number, y: number) : boolean {
        for(var i = 0; i <this.trail.length; i++){
            if(this.trail[i].x == x && this.trail[i].y == y)
                return true;
        }
        return false;
    }

    update() : void {
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

    draw(ctx: CanvasRenderingContext2D) : void {
        ctx.fillStyle = this.color;
        this.trail.forEach(element => {
            ctx.fillRect(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, this.width, this.height);
        });
    }
}

class SnakeGame {
    apple: Apple = new Apple();
    snake: Snake = new Snake();
    ctx: CanvasRenderingContext2D;
    width: number;
    height: number;
        
    constructor() {
        let surface = <HTMLCanvasElement>document.getElementById("surface");
        this.ctx = surface.getContext("2d");
        this.width = surface.clientWidth;
        this.height = surface.clientHeight;
    }

    generateApple() : void {
        do {
            this.apple.x = Math.floor(Math.random() * SCREEN_SIZE);
            this.apple.y = Math.floor(Math.random() * SCREEN_SIZE);
        } while(this.snake.checkCollision(this.apple.x, this.apple.y));
    }

    clearScreen() : void {
        this.ctx.fillStyle = "black";
        this.ctx.fillRect(0, 0, this.width, this.height);
    }

    handleInput(evt: KeyboardEvent) {
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

    update() : void {
        this.snake.update();
        if(this.snake.checkCollision(this.apple.x, this.apple.y)) {
            this.snake.tail++;
            this.generateApple();
        }
    }

    draw() : void {
        this.clearScreen();
        this.snake.draw(this.ctx);
        this.apple.draw(this.ctx);
    }
}

let game = new SnakeGame();

function keyDown(evt) : void {
    game.handleInput(evt);
}

function gameLoop() : void {
    game.update();
    game.draw();
}

window.onload = function() {
    document.addEventListener("keydown", keyDown);
    setInterval(gameLoop, 1000 / FPS);
}
