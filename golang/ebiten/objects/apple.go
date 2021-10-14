package objects

import (
	"image/color"
	"snake/constants"

	"github.com/hajimehoshi/ebiten/v2"
	"github.com/hajimehoshi/ebiten/v2/ebitenutil"
)

type Apple struct {
	Position Point
	Width    float64
	Height   float64
	Color    color.Color
}

func (a *Apple) Move(x, y int) {
	a.Position.X = x
	a.Position.Y = y
}

func (a *Apple) Draw(screen *ebiten.Image) {
	ebitenutil.DrawRect(
		screen,
		float64(a.Position.X*constants.SpriteSize),
		float64(a.Position.Y*constants.SpriteSize),
		a.Width,
		a.Height,
		a.Color)
}
