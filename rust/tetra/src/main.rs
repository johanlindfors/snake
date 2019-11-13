extern crate tetra; 
extern crate rand;

use tetra::graphics::{self, Color, DrawParams, Texture, Vec2};
use tetra::{Context, ContextBuilder, State};
use std::time::Duration;
use std::collections::VecDeque;
use rand::{Rng};
use tetra::input::{self, Key};

pub const FRAMES_PER_SECOND: u32 = 15;
pub const SPRITE_SIZE: i32 = 20;
pub const SCREEN_SIZE: i32 = 20;
pub const INITIAL_TAIL: usize = 5;

pub struct Position {
    x: i32,
    y: i32
}

impl Position {
    pub fn new(x: i32, y: i32) -> Position {
        Position {
            x: x,
            y: y,
        }
    }
}

struct Apple {
    x: i32,
    y: i32,
    texture: Texture
}

impl Apple {
    pub fn new(ctx: &mut Context) -> tetra::Result<Apple> {
        Ok(Apple {
            x: 3,
            y: 3,
            texture: Texture::new(ctx, "./resources/red.png")?
        })
    }

    pub fn draw(&mut self, ctx: &mut Context) {        
        graphics::draw(
            ctx, 
            &self.texture,
            DrawParams::new()
                    .position(Vec2::new(
                        (self.x * SPRITE_SIZE) as f32, 
                        (self.y * SPRITE_SIZE) as f32))
                    .scale(Vec2::new(
                        (SPRITE_SIZE as f32) * 0.95, 
                        (SPRITE_SIZE as f32) * 0.95)
                    )
                );
    }
}

pub struct Snake {
    pub x: i32,
    pub y: i32,
    pub dx: i32,
    pub dy: i32,        
    pub trail: VecDeque<Position>,
    pub tail: usize,
    texture: Texture
}

impl Snake {

    pub fn new(ctx: &mut Context) -> tetra::Result<Snake> {
        Ok(Snake {
            x: 10, 
            y: 10, 
            dx: 0, 
            dy: 0, 
            trail: VecDeque::new(), 
            tail: INITIAL_TAIL,
            texture: Texture::new(ctx, "./resources/green.png")?
        })
    }

    pub fn check_collision(&mut self, x: i32, y: i32) -> bool {
        for element in &self.trail {
            if element.x == x && element.y == y {
                return true;
            }
        }
        return false;
    }
    
    pub fn update(&mut self) {
        let x = (self.x + SCREEN_SIZE + self.dx) % SCREEN_SIZE;
        let y = (self.y + SCREEN_SIZE + self.dy) % SCREEN_SIZE;
        if self.check_collision(x, y) {
            self.tail = INITIAL_TAIL;
            self.x = 10;
            self.y = 10;
            self.dx = 0;
            self.dy = 0;
        } else {
            self.x = x;
            self.y = y;
        }

        self.trail.push_back(Position::new(self.x, self.y));
        loop {
            if self.trail.len() <= self.tail {
                break;
            }
            self.trail.pop_front();
        }
    }

    pub fn draw(&mut self, ctx: &mut Context) {
        for element in &self.trail {
            graphics::draw(
                ctx, 
                &self.texture,
                DrawParams::new()
                        .position(Vec2::new(
                            (element.x * SPRITE_SIZE) as f32, 
                            (element.y * SPRITE_SIZE) as f32))
                        .scale(Vec2::new(
                            (SPRITE_SIZE as f32) * 0.95, 
                            (SPRITE_SIZE as f32) * 0.95)
                        )
                    );
        }
    }
}

pub struct SnakeGame {
    apple: Apple,
    snake: Snake
}

impl SnakeGame {
    fn new(ctx: &mut Context) -> tetra::Result<SnakeGame> {
        Ok(SnakeGame {
            apple: Apple::new(ctx)?,
            snake: Snake::new(ctx)?,
        })
    }

    fn handle_input(&mut self, ctx: &mut Context) {
        if input::is_key_pressed(ctx, Key::Left) {
            if self.snake.dx == 0 {
                self.snake.dx = -1;
                self.snake.dy = 0;
            }
        } else if input::is_key_pressed(ctx, Key::Right) {
            if self.snake.dx == 0 {
                self.snake.dx = 1;
                self.snake.dy = 0;
            }
        } else if input::is_key_pressed(ctx, Key::Up) {
            if self.snake.dy == 0 {
                self.snake.dx = 0;
                self.snake.dy = -1;
            }
        } else if input::is_key_pressed(ctx, Key::Down) {
            if self.snake.dy == 0 {
                self.snake.dx = 0;
                self.snake.dy = 1;
            }
        }
    }
}

impl State for SnakeGame {

    fn update(&mut self, ctx: &mut Context) -> tetra::Result {
        self.handle_input(ctx);

        self.snake.update();

        if self.snake.check_collision(self.apple.x, self.apple.y) {
            self.snake.tail += 1;
            loop {
                let x = rand::thread_rng().gen_range(0, SCREEN_SIZE);
                let y = rand::thread_rng().gen_range(0, SCREEN_SIZE);
                if !self.snake.check_collision(x,y) {
                    self.apple.x = x;
                    self.apple.y = y;
                    break;
                }
            }
        }

        Ok(())
    }

    fn draw(&mut self, ctx: &mut Context, _dt: f64) -> tetra::Result {
        graphics::clear(ctx, Color::rgb(0.0, 0.0, 0.0));

        self.apple.draw(ctx);
        self.snake.draw(ctx);

        Ok(())
    }
}


pub fn main() -> tetra::Result {
    let width = (SPRITE_SIZE * SCREEN_SIZE) as i32;
    let height = (SPRITE_SIZE * SCREEN_SIZE) as i32;

    ContextBuilder::new("snake", width, height)
        .quit_on_escape(true)
        .tick_rate(15.0)
        .build()?
        .run_with(SnakeGame::new)
}
