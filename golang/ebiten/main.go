package main

import (
	"image/color"
	"log"
	"math/rand"
	"os"
	"time"

	"snake/constants"
	"snake/interfaces"
	"snake/objects"

	"github.com/hajimehoshi/ebiten/v2"
	"github.com/hajimehoshi/ebiten/v2/ebitenutil"
	"github.com/hajimehoshi/ebiten/v2/inpututil"
)

type EbitenWrapper struct {
	game   *objects.Game
	screen *ebiten.Image
	keys   []ebiten.Key
}

func (ew *EbitenWrapper) RenderRectangle(x1, y1, x2, y2 int, color color.RGBA) {
	ebitenutil.DrawRect(
		ew.screen,
		float64(x1),
		float64(y1),
		float64(x2-x1),
		float64(y2-y1),
		color)
}

func (ew *EbitenWrapper) HandleInput() interfaces.Direction {
	result := interfaces.None

	ew.keys = inpututil.AppendPressedKeys(ew.keys[:0])
	for _, p := range ew.keys {
		switch p {
		case ebiten.KeyArrowLeft:
			result = interfaces.Left
		case ebiten.KeyArrowRight:
			result = interfaces.Right
		case ebiten.KeyArrowDown:
			result = interfaces.Down
		case ebiten.KeyArrowUp:
			result = interfaces.Up
		case ebiten.KeyEscape:
			os.Exit(0)
		}
	}
	return result
}

func (ew *EbitenWrapper) Update() error {
	ew.game.Update(ew)
	return nil
}

func (ew *EbitenWrapper) Draw(screen *ebiten.Image) {
	ew.screen = screen
	ew.game.Draw(ew)
}

func (ew *EbitenWrapper) Layout(outsideWidth, outsideHeight int) (screenWidth, screenHeight int) {
	return constants.ScreenSize * constants.SpriteSize,
		constants.ScreenSize * constants.SpriteSize
}

func main() {
	rand.Seed(time.Now().UTC().UnixNano())

	ebiten.SetWindowSize(constants.ScreenSize*constants.SpriteSize, constants.ScreenSize*constants.SpriteSize)
	ebiten.SetWindowTitle("Snake!")
	ebiten.SetMaxTPS(constants.Fps)

	wrapper := EbitenWrapper{
		game: objects.CreateGame()}

	if err := ebiten.RunGame(&wrapper); err != nil {
		log.Fatal(err)
	}
}
