package objects

import (
	"image/color"
	"snake/constants"
	"snake/interfaces"
)

type Snake struct {
	position Vector
	velocity Vector
	width    int
	height   int
	color    color.RGBA
	tail     int
	trail    []Vector
}

func (s *Snake) CheckCollision(position Vector) bool {
	for _, p := range s.trail {
		if p.X == position.X && p.Y == position.Y {
			return true
		}
	}
	return false
}

func (s *Snake) handleInput(inputHandler interfaces.InputHandler) {
	directionPressed := inputHandler.HandleInput()
	if directionPressed == interfaces.Left && s.velocity.X == 0 {
		s.velocity.X = -1
		s.velocity.Y = 0
	}
	if directionPressed == interfaces.Right && s.velocity.X == 0 {
		s.velocity.X = 1
		s.velocity.Y = 0
	}
	if directionPressed == interfaces.Down && s.velocity.Y == 0 {
		s.velocity.X = 0
		s.velocity.Y = 1
	}
	if directionPressed == interfaces.Up && s.velocity.Y == 0 {
		s.velocity.X = 0
		s.velocity.Y = -1
	}
}

func (s *Snake) Draw(renderer interfaces.Renderer) {
	for _, p := range s.trail {
		x := p.X * constants.SpriteSize
		y := p.Y * constants.SpriteSize
		renderer.RenderRectangle(x, y, x+s.width, y+s.height, s.color)
	}
}

func (s *Snake) move() {
	s.position.X = (s.position.X + s.velocity.X + constants.ScreenSize) % constants.ScreenSize
	s.position.Y = (s.position.Y + s.velocity.Y + constants.ScreenSize) % constants.ScreenSize
}

func (s *Snake) reset() {
	s.position = Vector{constants.ScreenSize / 2, constants.ScreenSize / 2}
	s.velocity = Vector{0, 0}
	s.tail = constants.InitialTail
}

func (s *Snake) trimTrail() {
	s.trail = append(s.trail, s.position)
	if len(s.trail) > s.tail {
		ret := make([]Vector, 0)
		s.trail = append(ret, s.trail[1:]...)
	}
}

func (s *Snake) Update(inputHandler interfaces.InputHandler) {
	s.handleInput(inputHandler)
	s.move()
	if s.CheckCollision(s.position) {
		s.reset()
	}
	s.trimTrail()
}
