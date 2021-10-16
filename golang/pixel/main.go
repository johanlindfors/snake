package main

import (
	"image/color"
	"math/rand"
	"time"

	"snake/constants"
	"snake/interfaces"
	"snake/objects"

	"github.com/faiface/pixel"
	"github.com/faiface/pixel/imdraw"
	"github.com/faiface/pixel/pixelgl"
	"golang.org/x/image/colornames"
)

type PixelWrapper struct {
	imd *imdraw.IMDraw
	win *pixelgl.Window
}

func (p *PixelWrapper) RenderRectangle(x1, y1, x2, y2 int, color color.RGBA) {
	p.imd.Color = color
	p.imd.Push(pixel.V(float64(x1), float64(y1)))
	p.imd.Push(pixel.V(float64(x2), float64(y2)))
	p.imd.Rectangle(0)
}

func (p *PixelWrapper) HandleInput() interfaces.Direction {
	if p.win.Pressed(pixelgl.KeyLeft) {
		return interfaces.Left
	}
	if p.win.Pressed(pixelgl.KeyRight) {
		return interfaces.Right
	}
	if p.win.Pressed(pixelgl.KeyDown) {
		return interfaces.Up
	}
	if p.win.Pressed(pixelgl.KeyUp) {
		return interfaces.Down
	}
	return interfaces.None
}

func run() {
	cfg := pixelgl.WindowConfig{
		Title:  "Snake",
		Bounds: pixel.R(0, 0, constants.ScreenSize*constants.SpriteSize, constants.ScreenSize*constants.ScreenSize),
	}
	win, err := pixelgl.NewWindow(cfg)
	if err != nil {
		panic(err)
	}

	game := objects.CreateGame()
	frameTick := time.NewTicker(time.Second / time.Duration(constants.Fps))

	imd := imdraw.New(nil)
	wrapper := PixelWrapper{imd, win}

	for !win.Closed() {

		game.Update(&wrapper)
		imd.Clear()

		game.Draw(&wrapper)

		win.Clear(colornames.Black)
		imd.Draw(win)
		win.Update()

		if frameTick != nil {
			<-frameTick.C
		}
	}
}

func main() {
	rand.Seed(time.Now().UTC().UnixNano())
	pixelgl.Run(run)
	// ebiten.SetMaxTPS(constants.Fps)
}
