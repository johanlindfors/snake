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
FRAMES_PER_SECOND = 15

def handle_input(speed):
    for event in pygame.event.get():
        if event.type == pygame.QUIT: 
            sys.exit()
        elif event.type == KEYDOWN:
            if speed.x == 0:
                if event.key == K_LEFT:
                    speed.x = -1
                    speed.y = 0
                elif event.key == K_RIGHT:
                    speed.x = 1
                    speed.y = 0
            if speed.y == 0:
                if event.key == K_UP:
                    speed.x = 0
                    speed.y = -1
                elif event.key == K_DOWN:
                    speed.x = 0
                    speed.y = 1
            if event.key == K_ESCAPE:
                sys.exit()              

def check_collision(obj, trail):
    for body in trail:
        if body == obj:
            return True
    return False

def generate_apple(apple, trail):
    while True:
        apple.x = random.randint(0, SCREEN_SIZE - 1)
        apple.y = random.randint(0, SCREEN_SIZE - 1)
        if check_collision(apple, trail) == False:
            break

def check_pickup(apple, player, trail, tail):
    if apple == player:
        tail += 1
        generate_apple(apple, trail)
    return tail

def update(player, apple, trail, tail, speed):
    player.x = (player.x + speed.x) % SCREEN_SIZE
    player.y = (player.y + speed.y) % SCREEN_SIZE

    if check_collision(player, trail):
        if tail > INITIAL_TAIL:
            print(tail - INITIAL_TAIL)
        tail = INITIAL_TAIL
        player.x = MIDDLE
        player.y = MIDDLE
        speed.x = 0
        speed.y = 0

    tail = check_pickup(apple, player, trail, tail)

    trail.append(pygame.Vector2(player.x, player.y))
    while len(trail) > tail:
        trail.pop(0)
    return tail

def draw(screen, player, apple, trail):
    # clear screen
    screen.fill(BLACK)

    # draw snake
    for body in trail:
        body_rect = Rect(body.x * SPRITE_SIZE + 1, body.y * SPRITE_SIZE + 1, SPRITE_SIZE - 1, SPRITE_SIZE - 1)
        pygame.draw.rect(screen, GREEN, body_rect)

    # draw apple
    apple_rect = Rect(apple.x * SPRITE_SIZE + 1, apple.y * SPRITE_SIZE + 1, SPRITE_SIZE - 1,  SPRITE_SIZE - 1)
    pygame.draw.rect(screen, RED, apple_rect)

def run():
    width = SCREEN_SIZE * SPRITE_SIZE
    height = SCREEN_SIZE * SPRITE_SIZE
    size = width, height 

    screen = pygame.display.set_mode(size)
    clock = pygame.time.Clock()

    player = pygame.Vector2(MIDDLE, MIDDLE)
    apple = pygame.Vector2(THIRD, THIRD)
    speed = pygame.Vector2()
    trail = []
    tail = INITIAL_TAIL

    pygame.init()
    while True:
        clock.tick(FRAMES_PER_SECOND)
        handle_input(speed)
        tail = update(player, apple, trail, tail, speed)
        draw(screen, player, apple, trail)
        pygame.display.flip()

run()