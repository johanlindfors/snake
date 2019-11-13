extern crate tetra; 
extern crate rand;

use tetra::graphics::{self, Color, DrawParams, Texture};
use tetra::{Context, ContextBuilder, State};
use std::time::Duration;
use std::collections::VecDeque;
use rand::{Rng};

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
        // canvas.set_draw_color(Color::RGB(255,0,0));
        let width = (SPRITE_SIZE - 1) as u32;
        let height = (SPRITE_SIZE - 1) as u32;
        // canvas.fill_rect(
        //     Rect::new(self.x * SPRITE_SIZE + 1, self.y * SPRITE_SIZE + 1, width, height))
        //     .expect("Failed to draw the apple");
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
        // canvas.set_draw_color(Color::RGB(0,255,0));
        let width = (SPRITE_SIZE - 1) as u32;
        let height = (SPRITE_SIZE - 1) as u32;
        for element in &self.trail {
            // canvas.fill_rect(
            //     Rect::new(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, width, height))
            //     .expect("Failed to draw the snake");
        }
    }
}

pub struct SnakeGame {
    // canvas: Canvas<Window>,
    // event_pump: sdl2::EventPump,
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
    fn handle_input(&mut self) -> bool {
        // for event in self.event_pump.poll_iter() {
        //     match event {
        //         Event::Quit {..} |
        //         Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
        //             return true;
        //         },
        //         Event::KeyDown { keycode: Some(Keycode::Left), .. } => {
        //             if self.snake.dx == 0 {
        //                 self.snake.dx = -1;
        //                 self.snake.dy = 0;
        //             }
        //         },
        //         Event::KeyDown { keycode: Some(Keycode::Right), .. } => {
        //             if self.snake.dx == 0 {
        //                 self.snake.dx = 1;
        //                 self.snake.dy = 0;
        //             }
        //         },
        //         Event::KeyDown { keycode: Some(Keycode::Up), .. } => {
        //             if self.snake.dy == 0 {
        //                 self.snake.dx = 0;
        //                 self.snake.dy = -1;
        //             }
        //         },
        //         Event::KeyDown { keycode: Some(Keycode::Down), .. } => {
        //             if self.snake.dy == 0 {
        //                 self.snake.dx = 0;
        //                 self.snake.dy = 1;
        //             }
        //         },
        //         _ => { }
        //     }
        // }
        false
    }
}

impl State for SnakeGame {


    fn update(&mut self, ctx: &mut Context) -> tetra::Result {
        self.snake.update();

        if self.snake.check_collision(self.apple.x, self.apple.y) {
            self.snake.tail += 1;
            loop {
                let x = rand::thread_rng().gen_range(0, SPRITE_SIZE * SCREEN_SIZE);
                let y = rand::thread_rng().gen_range(0, SPRITE_SIZE * SCREEN_SIZE);
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
        // self.canvas.set_draw_color(Color::RGB(0, 0, 0));
        // self.canvas.clear();

        // self.apple.draw(&mut self.canvas);
        // self.snake.draw(&mut self.canvas);
        Ok(())
    }
}


pub fn main() -> tetra::Result {
    let width = (SPRITE_SIZE * SCREEN_SIZE) as i32;
    let height = (SPRITE_SIZE * SCREEN_SIZE) as i32;

    ContextBuilder::new("snake", width, height)
        .quit_on_escape(true)
        .build()?
        .run_with(SnakeGame::new)
}
