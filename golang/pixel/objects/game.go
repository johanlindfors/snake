package objects

import (
	"image/color"
	"math/rand"
	"snake/constants"
	"snake/interfaces"

	"golang.org/x/image/colornames"
)

func GenerateApple(snake Snake, apple *Apple) {
	for snake.CheckCollision(apple.Position) {
		apple.Move(
			rand.Intn(constants.ScreenSize),
			rand.Intn(constants.ScreenSize),
		)
	}
}

type Game struct {
	apple Apple
	snake Snake
}

func (g *Game) Update(inputHandler interfaces.InputHandler) error {
	g.snake.Update(inputHandler)

	if g.snake.CheckCollision(g.apple.Position) {
		g.snake.tail++
		GenerateApple(g.snake, &g.apple)
	}
	return nil
}

func (g *Game) Draw(renderer interfaces.Renderer) {
	g.apple.Draw(renderer)
	g.snake.Draw(renderer)
}

func CreateGame() *Game {
	position := Vector{constants.ScreenSize / 2, constants.ScreenSize / 2} // start position

	apple := Apple{
		position,
		constants.SpriteSize - 1, // width
		constants.SpriteSize - 1, // height
		colornames.Red}

	snake := Snake{
		position,                     // start position
		Vector{0, 0},                 // velocity
		constants.SpriteSize - 1,     // width
		constants.SpriteSize - 1,     // height
		color.RGBA{0, 0xff, 0, 0xff}, // green
		constants.InitialTail,
		[]Vector{position}}

	GenerateApple(snake, &apple)

	return &Game{apple, snake}
}
