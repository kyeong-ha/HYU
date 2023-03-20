import glfw
from OpenGL.GL import *
import numpy as np

type = GL_LINE_LOOP # Defalut is number '4'
def render(): 
    glClear(GL_COLOR_BUFFER_BIT) 
    glLoadIdentity() 
    
    glBegin(type)

    rad = np.radians(30)
    for i in range(12): # regular 12-sided polygon
        glVertex2fv(np.array([np.cos(rad*i), np.sin(rad*i)]))
    
    glEnd()

def key_callback(window, key, scancode, action, mods): 
    global type
    if action == glfw.PRESS: 
        if key == glfw.KEY_1: type = GL_POINTS
        elif key == glfw.KEY_2: type = GL_LINES
        elif key == glfw.KEY_3: type = GL_LINE_STRIP
        elif key == glfw.KEY_4: type = GL_LINE_LOOP
        elif key == glfw.KEY_5: type = GL_TRIANGLES
        elif key == glfw.KEY_6: type = GL_TRIANGLE_STRIP
        elif key == glfw.KEY_7: type = GL_TRIANGLE_FAN
        elif key == glfw.KEY_8: type = GL_QUADS
        elif key == glfw.KEY_9: type = GL_QUAD_STRIP
        elif key == glfw.KEY_0: type = GL_POLYGON
        
def main():
    if not glfw.init():
        return
    
    window = glfw.create_window(480,480,"2018008304-2-2", None, None) 
    if not window:
        glfw.terminate()
        return

    glfw.set_key_callback(window, key_callback) 
    glfw.make_context_current(window)

    while not glfw.window_should_close(window): 
        glfw.poll_events()
        render()
        glfw.swap_buffers(window)
        
    glfw.terminate()

if __name__ == "__main__": 
    main()