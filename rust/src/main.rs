extern crate sdl2; 

mod snake {
    use std::time::Duration;
    use sdl2::pixels::Color;
    use sdl2::event::Event;
    use sdl2::keyboard::Keycode;
    use sdl2::rect::{Rect};
    use sdl2::render::{Canvas};
    use sdl2::video::{Window};
    use std::collections::VecDeque;    

    pub const FRAMES_PER_SECOND: u32 = 15;
    pub const SPRITE_SIZE: i32 = 20;
    pub const SCREEN_SIZE: i32 = 20;
    pub const _INITIAL_TAIL: u32 = 5;

    struct Position {
        x: i32,
        y: i32
    }

    struct Apple {
        x: i32,
        y: i32
    }

    impl Apple {
        pub fn draw(&mut self, canvas: &mut Canvas<Window>) {
            canvas.set_draw_color(Color::RGB(255,0,0));
            let width = (SPRITE_SIZE - 1) as u32;
            let height = (SPRITE_SIZE - 1) as u32;
            canvas.fill_rect(Rect::new(self.x * SPRITE_SIZE + 1, self.y * SPRITE_SIZE + 1, width, height));
        }
    }

    struct Snake {
        pub x: i32,
        pub y: i32,
        pub dx: i32,
        pub dy: i32,        
        trail: VecDeque<Position>,
        tail: usize
    }

    impl Snake {

        // pub fn new() -> Snake {
        //     let trail: VecDeque<Position> = VecDeque::new();
            
        // }
        
        pub fn update(&mut self) {
            self.x = (self.x + SCREEN_SIZE + self.dx) % SCREEN_SIZE;
            self.y = (self.y + SCREEN_SIZE + self.dy) % SCREEN_SIZE;

            self.trail.push_back(Position {x: self.x, y: self.y});
            loop {
                if(self.trail.len() <= self.tail) {
                    break;
                }
                self.trail.pop_front();
            }
        }

        pub fn draw(&mut self, canvas: &mut Canvas<Window>) {
            canvas.set_draw_color(Color::RGB(0,255,0));
            let width = (SPRITE_SIZE - 1) as u32;
            let height = (SPRITE_SIZE - 1) as u32;
            for element in &self.trail {
                canvas.fill_rect(Rect::new(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, width, height));
            }
        }
    }

    pub struct SnakeGame {
        sdl_context: sdl2::Sdl,
        canvas: Canvas<Window>,
        event_pump: sdl2::EventPump,
        apple: Apple,
        snake: Snake
    }

    impl SnakeGame {
        pub fn new() -> SnakeGame {
            let context = sdl2::init().unwrap();
            let video_subsystem = context.video().unwrap();
            
            let width = (SPRITE_SIZE * SCREEN_SIZE) as u32;
            let height = (SPRITE_SIZE * SCREEN_SIZE) as u32;
            let window = video_subsystem.window("snake", width, height)
                .position_centered()
                .build()
                .unwrap();
        
            SnakeGame{
                canvas: window.into_canvas().build().unwrap(),
                event_pump: context.event_pump().unwrap(),
                sdl_context: context,
                apple: Apple { x: 3, y: 3},
                snake: Snake {x:10, y: 10, dx: 0, dy: 0, trail: VecDeque::new(), tail: 5 }
            }
        }

        pub fn handle_input(&mut self) -> bool {
            for event in self.event_pump.poll_iter() {
                match event {
                    Event::Quit {..} |
                    Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                        return true;
                    },
                    Event::KeyDown { keycode: Some(Keycode::Left), .. } => {
                        if(self.snake.dx == 0) {
                            self.snake.dx = -1;
                            self.snake.dy = 0;
                        }
                    },
                    Event::KeyDown { keycode: Some(Keycode::Right), .. } => {
                        if(self.snake.dx == 0) {
                            self.snake.dx = 1;
                            self.snake.dy = 0;
                        }
                    },
                    Event::KeyDown { keycode: Some(Keycode::Up), .. } => {
                        if(self.snake.dy == 0) {
                            self.snake.dx = 0;
                            self.snake.dy = -1;
                        }
                    },
                    Event::KeyDown { keycode: Some(Keycode::Down), .. } => {
                        if(self.snake.dy == 0) {
                            self.snake.dx = 0;
                            self.snake.dy = 1;
                        }
                    },
                    _ => { }
                }
            }
            false
        }

        pub fn update(&mut self) {
            self.snake.update();
        }

        pub fn draw(&mut self) {
            self.canvas.set_draw_color(Color::RGB(0, 0, 0));
            self.canvas.clear();
            self.canvas.present();

            self.apple.draw(&mut self.canvas);
            self.snake.draw(&mut self.canvas);
        }

        pub fn run(&mut self) {
            let mut index = 0;

            'running: loop {
                if self.handle_input() {
                    break 'running;
                }

                self.update();
                self.draw();

                self.canvas.present();
                ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / FRAMES_PER_SECOND));
            }
        }
    }
}

pub fn main() {
    let mut snake_game = snake::SnakeGame::new();
    snake_game.run();    
}