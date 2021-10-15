package main

import (
	"log"
	"math/rand"
	"time"

	"snake/constants"
	"snake/objects"

	"github.com/hajimehoshi/ebiten/v2"
)

func main() {
	rand.Seed(time.Now().UTC().UnixNano())

	ebiten.SetWindowSize(constants.ScreenSize*constants.SpriteSize, constants.ScreenSize*constants.SpriteSize)
	ebiten.SetWindowTitle("Snake!")
	ebiten.SetMaxTPS(constants.Fps)

	if err := ebiten.RunGame(objects.CreateSnakeGame()); err != nil {
		log.Fatal(err)
	}
}
