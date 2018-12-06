extern crate sdl2; 

mod snake {
    use std::time::Duration;
    use sdl2::pixels::Color;
    use sdl2::event::Event;
    use sdl2::keyboard::Keycode;
    use sdl2::rect::{Rect};
    use sdl2::render::{Canvas};
    use sdl2::video::{Window};

    pub const FRAMES_PER_SECOND: u32 = 15;
    pub const SPRITE_SIZE: u32 = 20;
    pub const SCREEN_SIZE: u32 = 20;
    pub const _INITIAL_TAIL: u32 = 5;

    pub struct SnakeGame {
        sdl_context: sdl2::Sdl,
        canvas: Canvas<Window>,
        event_pump: sdl2::EventPump
    }

    impl SnakeGame {
        pub fn new() -> SnakeGame {
            let context = sdl2::init().unwrap();
            let video_subsystem = context.video().unwrap();
        
            let window = video_subsystem.window("snake", SPRITE_SIZE * SCREEN_SIZE, SPRITE_SIZE * SCREEN_SIZE)
                .position_centered()
                .build()
                .unwrap();
        
            SnakeGame{
                canvas: window.into_canvas().build().unwrap(),
                event_pump: context.event_pump().unwrap(),
                sdl_context: context,
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
                        return true;
                    },
                    _ => { }
                }
            }
            false
        }

        pub fn update(&mut self) {

        }

        pub fn draw(&mut self) {
            self.canvas.set_draw_color(Color::RGB(0, 0, 0));
            self.canvas.clear();
            self.canvas.present();

            self.canvas.set_draw_color(Color::RGB(255,0,0));
            let _result = self.canvas.fill_rect(Rect::new(100, 100, SPRITE_SIZE, SPRITE_SIZE)).unwrap();
        }

        pub fn run(&mut self) {
            let mut index = 0;

            'running: loop {
                if self.handle_input() {
                    break 'running;
                }
                // The rest of the game loop goes here...
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