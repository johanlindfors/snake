package interfaces

import "image/color"

type Renderer interface {
	RenderRectangle(x1, y1, x2, y2 int, color color.RGBA)
}
