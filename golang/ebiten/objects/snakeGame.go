package objects

import (
	"image/color"
	"math/rand"
	"snake/constants"

	"github.com/hajimehoshi/ebiten/v2"
)

func GenerateApple(snake Snake, apple *Apple) {
	for ok := true; ok; ok = snake.CheckCollision(apple.Position) {
		apple.Move(
			rand.Intn(constants.ScreenSize),
			rand.Intn(constants.ScreenSize),
		)
	}
}

type SnakeGame struct {
	apple Apple
	snake Snake
}

func (g *SnakeGame) Update() error {
	g.snake.HandleInput()
	g.snake.Update()

	if g.snake.CheckCollision(g.apple.Position) {
		g.snake.tail++
		GenerateApple(g.snake, &g.apple)
	}
	return nil
}

func (g *SnakeGame) Draw(screen *ebiten.Image) {
	g.apple.Draw(screen)
	g.snake.Draw(screen)
}

func (g *SnakeGame) Layout(outsideWidth, outsideHeight int) (screenWidth, screenHeight int) {
	return constants.ScreenSize * constants.SpriteSize,
		constants.ScreenSize * constants.SpriteSize
}

func CreateSnakeGame() *SnakeGame {
	position := Vector{constants.ScreenSize / 2, constants.ScreenSize / 2} // start position

	apple := Apple{
		position,
		constants.SpriteSize - 1,     // width
		constants.SpriteSize - 1,     // height
		color.RGBA{0xff, 0, 0, 0xff}} // red

	snake := Snake{
		position,                     // start position
		Vector{0, 0},                 // velocity
		constants.SpriteSize - 1,     // width
		constants.SpriteSize - 1,     // height
		color.RGBA{0, 0xff, 0, 0xff}, // green
		constants.InitialTail,
		[]Vector{position},
		[]ebiten.Key{}}

	GenerateApple(snake, &apple)

	return &SnakeGame{apple, snake}
}
