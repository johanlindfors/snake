extern crate sdl2; 

use std::time::Duration;
use snake::{FRAMES_PER_SECOND};

mod snake {
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

    pub fn init(sdl_context: &sdl2::Sdl) -> Canvas<Window> {
        let video_subsystem = sdl_context.video().unwrap();
    
        let window = video_subsystem.window("snake", SPRITE_SIZE * SCREEN_SIZE, SPRITE_SIZE * SCREEN_SIZE)
            .position_centered()
            .build()
            .unwrap();
    
        window.into_canvas().build().unwrap()
    }

    pub fn handle_input(event_pump: &mut sdl2::EventPump) -> bool {
        for event in event_pump.poll_iter() {
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

    pub fn update() {

    }

    pub fn draw(i: u8, canvas: &mut Canvas<Window>) -> u8 {
        canvas.set_draw_color(Color::RGB(0, 0, 0));
        canvas.clear();
        canvas.present();

        let next_index = (i + 1) % 255;
        canvas.set_draw_color(Color::RGB(next_index, 64, 255 - next_index));
        let _result = canvas.fill_rect(Rect::new(100, 100, SPRITE_SIZE, SPRITE_SIZE)).unwrap();
        next_index
    }
}

pub fn main() {
    let sdl_context = sdl2::init().unwrap();

    let mut canvas = snake::init(&sdl_context);
    let mut event_pump = sdl_context.event_pump().unwrap();
    let mut index = 0;
        
    'running: loop {
        if snake::handle_input(&mut event_pump) {
            break 'running;
        }
        // The rest of the game loop goes here...
        snake::update();
        index = snake::draw(index, &mut canvas);
        canvas.present();
        ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / FRAMES_PER_SECOND));
    }
}