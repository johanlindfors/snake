package objects

import (
	"image/color"
	"math/rand"
	"os"
	"snake/constants"

	"github.com/hajimehoshi/ebiten/v2"
	"github.com/hajimehoshi/ebiten/v2/inpututil"
)

type SnakeGame struct {
	apple Apple
	snake Snake
	keys  []ebiten.Key
}

func (g *SnakeGame) GenerateApple() {
	for ok := true; ok; ok = g.snake.CheckCollision(g.apple.Position) {
		g.apple.Move(
			rand.Intn(constants.ScreenSize),
			rand.Intn(constants.ScreenSize),
		)
	}
}

func (g *SnakeGame) Update() error {
	g.handleInput()
	g.snake.Update()

	if g.snake.CheckCollision(g.apple.Position) {
		g.snake.tail++
		g.GenerateApple()
	}
	return nil
}

func (g *SnakeGame) handleInput() {
	g.keys = inpututil.AppendPressedKeys(g.keys[:0])
	for _, p := range g.keys {
		if p == ebiten.KeyArrowLeft && g.snake.dx == 0 {
			g.snake.dx = -1
			g.snake.dy = 0
		}
		if p == ebiten.KeyArrowRight && g.snake.dx == 0 {
			g.snake.dx = 1
			g.snake.dy = 0
		}
		if p == ebiten.KeyArrowDown && g.snake.dy == 0 {
			g.snake.dx = 0
			g.snake.dy = 1
		}
		if p == ebiten.KeyArrowUp && g.snake.dy == 0 {
			g.snake.dx = 0
			g.snake.dy = -1
		}
		if p == ebiten.KeyEscape {
			os.Exit(0)
		}
	}
}

func (g *SnakeGame) Draw(screen *ebiten.Image) {
	g.apple.Draw(screen)
	g.snake.Draw(screen)
}

func (g *SnakeGame) Layout(outsideWidth, outsideHeight int) (screenWidth, screenHeight int) {
	return constants.ScreenSize * constants.SpriteSize, constants.ScreenSize * constants.SpriteSize
}

func CreateSnakeGame() *SnakeGame {
	return &SnakeGame{
		Apple{Point{3, 3}, constants.SpriteSize - 1, constants.SpriteSize - 1, color.RGBA{0xff, 0, 0, 0xff}},
		Snake{Point{10, 10}, constants.SpriteSize - 1, constants.SpriteSize - 1, color.RGBA{0, 0xff, 0, 0xff}, constants.InitialTail, 0, 1, []Point{}},
		[]ebiten.Key{},
	}
}
