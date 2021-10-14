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
		if p == ebiten.KeyArrowLeft && g.snake.velocity.X == 0 {
			g.snake.velocity.X = -1
			g.snake.velocity.Y = 0
		}
		if p == ebiten.KeyArrowRight && g.snake.velocity.X == 0 {
			g.snake.velocity.X = 1
			g.snake.velocity.Y = 0
		}
		if p == ebiten.KeyArrowDown && g.snake.velocity.Y == 0 {
			g.snake.velocity.X = 0
			g.snake.velocity.Y = 1
		}
		if p == ebiten.KeyArrowUp && g.snake.velocity.Y == 0 {
			g.snake.velocity.X = 0
			g.snake.velocity.Y = -1
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
		Apple{Vector{3, 3}, constants.SpriteSize - 1, constants.SpriteSize - 1, color.RGBA{0xff, 0, 0, 0xff}},
		Snake{Vector{10, 10}, Vector{0, 0}, constants.SpriteSize - 1, constants.SpriteSize - 1, color.RGBA{0, 0xff, 0, 0xff}, constants.InitialTail, []Vector{}},
		[]ebiten.Key{},
	}
}
