use sdl2::pixels::Color;
use sdl2::rect::{Rect};
use sdl2::render::{Canvas};
use sdl2::video::{Window};
use crate::game_objects::constants::{SPRITE_SIZE};

pub struct Apple {
    pub x: i32,
    pub y: i32
}

impl Apple {
    pub fn draw(&mut self, canvas: &mut Canvas<Window>) {
        canvas.set_draw_color(Color::RGB(255,0,0));
        let width = (SPRITE_SIZE - 1) as u32;
        let height = (SPRITE_SIZE - 1) as u32;
        canvas.fill_rect(
            Rect::new(self.x * SPRITE_SIZE + 1, self.y * SPRITE_SIZE + 1, width, height))
            .expect("Failed to draw the apple");
    }
}
