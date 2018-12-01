require 'gosu'

SCREEN_SIZE = 20
SPRITE_SIZE = 20
INITIAL_TAIL = 5
FRAMES_PER_SECOND = 15
RED = 0xffff0000
GREEN = 0xff00ff00
BLUE = 0xff0000ff

class Drawable
    attr_accessor :x, :y

    def initialize x, y, color
        @x = x
        @y = y
        @color = color
        @width = SPRITE_SIZE - 1
        @height = SPRITE_SIZE - 1
    end

    def draw
        Gosu.draw_rect x * SPRITE_SIZE + 1, y * SPRITE_SIZE + 1, @width, @height, @color
    end
end

class Snake < Drawable
    attr_accessor :dx, :dy, :tail, :trail

    def initialize x, y, color
        super x,y,color
        @dx = @dy = 0
        @trail = Array.new
        @tail = INITIAL_TAIL
    end

    def checkCollision x, y
        @trail.each do |element| 
            if x == element.x and y == element.y
                return true
            end
        end
        return false
    end

    def update
        @x += dx
        @y += dy

        if checkCollision @x, @y
            @tail = INITIAL_TAIL
            @x = @y = 10
            @dx = @dy = 0
        end
        
        @x = @x % SCREEN_SIZE
        @y = @y % SCREEN_SIZE

        @trail << Struct.new(:x, :y).new(@x, @y)
        while @trail.length > @tail
            @trail.shift
        end
    end

    def draw
        @trail.each do |element| 
            Gosu.draw_rect element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, @width, @height, @color
        end
    end
end

class SnakeWindow < Gosu::Window
    def initialize
        super SPRITE_SIZE * SCREEN_SIZE, SPRITE_SIZE * SCREEN_SIZE
        self.caption = 'Snake'
        self.update_interval = 1000 / FRAMES_PER_SECOND

        @apple = Drawable.new 3, 10, RED
        @snake = Snake.new 10, 10, GREEN
        @prng = Random.new
    end

    def resetApple
        loop do 
            @apple.x = @prng.rand SCREEN_SIZE
            @apple.y = @prng.rand SCREEN_SIZE
            break unless @snake.checkCollision @apple.x, @apple.y
        end
    end

    def checkPickup
        if @snake.x == @apple.x and @snake.y == @apple.y
            @snake.tail += 1
            resetApple
        end
    end

    def handleInput
        if @snake.dx == 0
            if Gosu.button_down? Gosu::KB_LEFT
                @snake.dx = -1
                @snake.dy = 0
            end
            if Gosu.button_down? Gosu::KB_RIGHT
                @snake.dx = 1
                @snake.dy = 0
            end
        end
        if @snake.dy == 0
            if Gosu.button_down? Gosu::KB_UP
                @snake.dx = 0
                @snake.dy = -1
            end
            if Gosu.button_down? Gosu::KB_DOWN
                @snake.dx = 0
                @snake.dy = 1
            end
        end
        if Gosu.button_down? Gosu::KB_ESCAPE
            close
        end
    end

    def update
        handleInput
        checkPickup
        @snake.update
    end

    def draw
        @apple.draw
        @snake.draw
    end
end

SnakeWindow.new.show
