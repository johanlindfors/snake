extern crate sdl2; 

use sdl2::pixels::Color;
use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use std::time::Duration;
use sdl2::video::{Window};
use sdl2::render::{Canvas};

fn handle_input(event_pump: &mut sdl2::EventPump) -> bool {
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
    return false;
}

fn update() {

}

fn draw(i: u8, canvas: &mut Canvas<Window>) -> u8 {
    let j = (i + 1) % 255;
    canvas.set_draw_color(Color::RGB(j, 64, 255 - j));
    canvas.clear();
    return j;
}

pub fn main() {
    let sdl_context = sdl2::init().unwrap();
    let video_subsystem = sdl_context.video().unwrap();
 
    let window = video_subsystem.window("snake", 400, 400)
        .position_centered()
        .build()
        .unwrap();
 
    let mut canvas = window.into_canvas().build().unwrap();
 
    canvas.set_draw_color(Color::RGB(0, 255, 255));
    canvas.clear();
    canvas.present();
    let mut event_pump = sdl_context.event_pump().unwrap();
    let mut i = 0;
        
    'running: loop {
        if handle_input(&mut event_pump) {
            break 'running;
        }
        // The rest of the game loop goes here...
        update();
        i = draw(i, &mut canvas);
        canvas.present();
        ::std::thread::sleep(Duration::new(0, 1_000_000_000u32 / 60));
    }
}