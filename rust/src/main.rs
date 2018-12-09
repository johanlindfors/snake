extern crate sdl2; 
extern crate rand;

mod snake {
    use std::time::Duration;
    use sdl2::pixels::Color;
    use sdl2::event::Event;
    use sdl2::keyboard::Keycode;
    use sdl2::rect::{Rect};
    use sdl2::render::{Canvas};
    use sdl2::video::{Window};
    use std::collections::VecDeque;
    use rand::{Rng};

    pub const FRAMES_PER_SECOND: u32 = 15;
    pub const SPRITE_SIZE: i32 = 20;
    pub const SCREEN_SIZE: i32 = 20;
    pub const INITIAL_TAIL: usize = 5;

    pub struct Position {
        pub x: i32,
        pub y: i32
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
            let _result = canvas.fill_rect(Rect::new(self.x * SPRITE_SIZE + 1, self.y * SPRITE_SIZE + 1, width, height)).unwrap();
        }
    }

    pub struct Snake {
        pub x: i32,
        pub y: i32,
        pub dx: i32,
        pub dy: i32,        
        pub trail: VecDeque<Position>,
        pub tail: usize
    }

    impl Snake {

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

            self.trail.push_back(Position {x: self.x, y: self.y});
            loop {
                if self.trail.len() <= self.tail {
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
                let _result = canvas.fill_rect(Rect::new(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, width, height)).unwrap();
            }
        }
    }

    pub struct SnakeGame {
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
                apple: Apple { x: 3, y: 3 },
                snake: Snake {x:10, y: 10, dx: 0, dy: 0, trail: VecDeque::new(), tail: INITIAL_TAIL }
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
                        if self.snake.dx == 0 {
                            self.snake.dx = -1;
                            self.snake.dy = 0;
                        }
                    },
                    Event::KeyDown { keycode: Some(Keycode::Right), .. } => {
                        if self.snake.dx == 0 {
                            self.snake.dx = 1;
                            self.snake.dy = 0;
                        }
                    },
                    Event::KeyDown { keycode: Some(Keycode::Up), .. } => {
                        if self.snake.dy == 0 {
                            self.snake.dx = 0;
                            self.snake.dy = -1;
                        }
                    },
                    Event::KeyDown { keycode: Some(Keycode::Down), .. } => {
                        if self.snake.dy == 0 {
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
        }

        pub fn draw(&mut self) {
            self.canvas.set_draw_color(Color::RGB(0, 0, 0));
            self.canvas.clear();

            self.apple.draw(&mut self.canvas);
            self.snake.draw(&mut self.canvas);
        }

        pub fn run(&mut self) {
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

#[cfg(test)] 
mod test {
    use std::collections::{VecDeque};
    use snake::{Snake,Position};

    #[test]
    fn test_collision_empty_trail() {
        // arrange
        let mut snake = Snake {x:10, y: 10, dx: 0, dy: 0, trail: VecDeque::new(), tail: 5 };
        // act
        let result = snake.check_collision(0,0);
        // assert
        assert_eq!(result, false);
    }

    #[test]
    fn test_collision_with_element_in_trail() {
        // arrange
        let mut snake = Snake {x:10, y: 10, dx: 0, dy: 0, trail: VecDeque::new(), tail: 5 };
        snake.trail.push_back(Position {x: 2, y: 2});
        // act
        let result = snake.check_collision(2,2);
        // assert
        assert_eq!(result, true);
    }
}