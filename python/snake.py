import sys, math, random, pygame
from pygame.locals import *

sprite_size = 20
screen_size = 20
width = screen_size * sprite_size
height = screen_size * sprite_size
size = width, height 
black = 0, 0, 0
red = 255, 0, 0
green = 0, 255, 0
middle = math.floor(screen_size/2)
third = math.floor(screen_size/3)
initial_tail = 5

screen = pygame.display.set_mode(size)
clock = pygame.time.Clock()

class SnakeGame(object):
    def __init__(self):
        pygame.init()
        
        self._player = pygame.Vector2(middle, middle)
        self._apple = pygame.Vector2(third, third)
        self._speed = pygame.Vector2()
        self._trail = []
        self._tail = initial_tail

    def handle_input(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT: 
                sys.exit()
            elif event.type == KEYDOWN:
                if event.key == K_LEFT:
                    self._speed.x = -1
                    self._speed.y = 0
                elif event.key == K_RIGHT:
                    self._speed.x = 1
                    self._speed.y = 0
                elif event.key == K_UP:
                    self._speed.x = 0
                    self._speed.y = -1
                elif event.key == K_DOWN:
                    self._speed.x = 0
                    self._speed.y = 1                

    def check_collision(self):
        for body in self._trail:
            if body == self._player:
                if self._tail > initial_tail:
                    print(self._tail - initial_tail)
                self._tail = initial_tail
                self._trail = []
                self._player = pygame.Vector2(middle, middle)
                self._speed = pygame.Vector2(0,0)

    def check_pickup(self):
        if self._apple == self._player:
            self._tail += 1
            self._apple.x = random.randint(0, screen_size - 1)
            self._apple.y = random.randint(0, screen_size - 1)

    def update(self):
        self._player.x += self._speed.x
        self._player.y += self._speed.y

        if self._player.x < 0:
            self._player.x = screen_size - 1
        if self._player.y < 0:
            self._player.y = screen_size - 1
        if self._player.x > screen_size - 1:
            self._player.x = 0
        if self._player.y > screen_size - 1:
            self._player.y = 0

        self.check_collision()
        self.check_pickup()

        self._trail.append(pygame.Vector2(self._player.x, self._player.y))
        while len(self._trail) > self._tail:
            self._trail.pop(0)

    def draw(self):
        # clear screen
        screen.fill(black)

        # draw snake
        for body in self._trail:
            body_rect = Rect(body.x * sprite_size + 1, body.y * sprite_size + 1, sprite_size - 1, sprite_size - 1)
            pygame.draw.rect(screen, green, body_rect)

        # draw apple
        apple_rect = Rect(self._apple.x * sprite_size + 1, self._apple.y * sprite_size + 1, sprite_size - 1,  sprite_size - 1)
        pygame.draw.rect(screen, red, apple_rect)

    def run(self):
        while True:
            clock.tick(15)
            self.handle_input()
            self.update()
            self.draw()
            pygame.display.flip()

game = SnakeGame()
game.run()