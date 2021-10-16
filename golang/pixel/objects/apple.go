package objects

import (
	"image/color"
	"snake/constants"
	"snake/interfaces"
)

type Apple struct {
	Position Vector
	Width    int
	Height   int
	Color    color.RGBA
}

func (a *Apple) Move(x, y int) {
	a.Position.X = x
	a.Position.Y = y
}

func (a *Apple) Draw(renderer interfaces.Renderer) {
	x := a.Position.X * constants.SpriteSize
	y := a.Position.Y * constants.SpriteSize
	renderer.RenderRectangle(x, y, x+a.Width, y+a.Height, a.Color)
}
