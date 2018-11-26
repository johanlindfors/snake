import sys, math, random, pygame
from pygame.locals import *

SPRITE_SIZE = 20
SCREEN_SIZE = 20
BLACK = 0, 0, 0
RED = 255, 0, 0
GREEN = 0, 255, 0
MIDDLE = math.floor(SCREEN_SIZE/2)
THIRD = math.floor(SCREEN_SIZE/3)
INITIAL_TAIL = 5

width = SCREEN_SIZE * SPRITE_SIZE
height = SCREEN_SIZE * SPRITE_SIZE
size = width, height 

screen = pygame.display.set_mode(size)
clock = pygame.time.Clock()

class SnakeGame(object):
    def __init__(self):
        pygame.init()
        
        self._player = pygame.Vector2(MIDDLE, MIDDLE)
        self._apple = pygame.Vector2(THIRD, THIRD)
        self._speed = pygame.Vector2()
        self._trail = []
        self._tail = INITIAL_TAIL

    def handle_input(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT: 
                sys.exit()
            elif event.type == KEYDOWN:
                if self._speed.x == 0:
                    if event.key == K_LEFT:
                        self._speed.x = -1
                        self._speed.y = 0
                    elif event.key == K_RIGHT:
                        self._speed.x = 1
                        self._speed.y = 0
                if self._speed.y == 0:
                    if event.key == K_UP:
                        self._speed.x = 0
                        self._speed.y = -1
                    elif event.key == K_DOWN:
                        self._speed.x = 0
                        self._speed.y = 1                

    def check_collision(self):
        for body in self._trail:
            if body == self._player:
                if self._tail > INITIAL_TAIL:
                    print(self._tail - INITIAL_TAIL)
                self._tail = INITIAL_TAIL
                self._trail = []
                self._player = pygame.Vector2(MIDDLE, MIDDLE)
                self._speed = pygame.Vector2(0,0)

    def check_pickup(self):
        if self._apple == self._player:
            self._tail += 1
            self._apple.x = random.randint(0, SCREEN_SIZE - 1)
            self._apple.y = random.randint(0, SCREEN_SIZE - 1)

    def update(self):
        self._player.x += self._speed.x
        self._player.y += self._speed.y

        if self._player.x < 0:
            self._player.x = SCREEN_SIZE - 1
        if self._player.y < 0:
            self._player.y = SCREEN_SIZE - 1
        if self._player.x > SCREEN_SIZE - 1:
            self._player.x = 0
        if self._player.y > SCREEN_SIZE - 1:
            self._player.y = 0

        self.check_collision()
        self.check_pickup()

        self._trail.append(pygame.Vector2(self._player.x, self._player.y))
        while len(self._trail) > self._tail:
            self._trail.pop(0)

    def draw(self):
        # clear screen
        screen.fill(BLACK)

        # draw snake
        for body in self._trail:
            body_rect = Rect(body.x * SPRITE_SIZE + 1, body.y * SPRITE_SIZE + 1, SPRITE_SIZE - 1, SPRITE_SIZE - 1)
            pygame.draw.rect(screen, GREEN, body_rect)

        # draw apple
        apple_rect = Rect(self._apple.x * SPRITE_SIZE + 1, self._apple.y * SPRITE_SIZE + 1, SPRITE_SIZE - 1,  SPRITE_SIZE - 1)
        pygame.draw.rect(screen, RED, apple_rect)

    def run(self):
        while True:
            clock.tick(15)
            self.handle_input()
            self.update()
            self.draw()
            pygame.display.flip()

game = SnakeGame()
game.run()