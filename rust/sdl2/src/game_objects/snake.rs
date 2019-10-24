use std::collections::VecDeque;
use sdl2::pixels::Color;
use sdl2::rect::{Rect};
use sdl2::render::{Canvas};
use sdl2::video::{Window};

use game_objects::position::{Position};
use game_objects::constants::{SPRITE_SIZE, SCREEN_SIZE, INITIAL_TAIL};

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

        self.trail.push_back(Position::new(self.x, self.y));
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
            canvas.fill_rect(
                Rect::new(element.x * SPRITE_SIZE + 1, element.y * SPRITE_SIZE + 1, width, height))
                .expect("Failed to draw the snake");
        }
    }
}
