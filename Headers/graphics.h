/*
 * File: graphics.h
 */
#ifndef GRAPHICS_H
#define GRAPHICS_H

/*
 * graphics class
 */
class graphics {

};

/*
 * Graphic function declarations
 */
sdl_surface*	load_image(str filename);
void			apply_surface(int x, int y, sdl_surface* src, sdl_surface* dest);

#endif
/* end graphics.h */