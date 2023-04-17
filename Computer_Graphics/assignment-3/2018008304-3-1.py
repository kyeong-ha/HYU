import glfw
from OpenGL.GL import *
import numpy as np

th = np.radians(10)
R = np.array([[np.cos(th), -np.sin(th), 0.],
              [np.sin(th), np.cos(th), 0.],
              [0., 0., 1.]])

T = np.array([[1.,0.,0.], 
              [0.,1.,0.],
              [0.,0.,1.]])

def render(T):
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()
    
    # draw cooridnate
    glBegin(GL_LINES)
    glColor3ub(255, 0, 0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([1.,0.]))
    glColor3ub(0, 255, 0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([0.,1.]))
    glEnd()
    
    # draw triangle
    glBegin(GL_TRIANGLES)
    glColor3ub(255, 255, 255)
    glVertex2fv( (T @ np.array([.0,.5,1.]))[:-1] )
    glVertex2fv( (T @ np.array([.0,.0,1.]))[:-1] )
    glVertex2fv( (T @ np.array([.5,.0,1.]))[:-1] )
    glEnd()

def key_callback(window, key, scancode, action, mods): 
    global type
    global T
    if action == glfw.PRESS or action==glfw.REPEAT:
        if key == glfw.KEY_Q: 
            T[0,2] -= 0.1
        elif key == glfw.KEY_E:
            T[0,2] += 0.1
            glTranslatef(.1, 0, 0)
            
        elif key == glfw.KEY_A:
            R = np.array([[np.cos(th), -np.sin(th), 0.],
                          [np.sin(th), np.cos(th), 0.],
                          [0., 0., 1.]])
            T = T @ R
            
        elif key == glfw.KEY_D:
            R = np.array([[np.cos(th), np.sin(th), 0.],
                          [-np.sin(th), np.cos(th), 0.],
                          [0., 0., 1.]])
            T = T @ R
            
        elif key == glfw.KEY_1:
            T = np.array([[1.,0.,0.], 
                          [0.,1.,0.],
                          [0.,0.,1.]])
            
        elif key == glfw.KEY_W: 
            T[0,0] *= 0.9
            
        elif key == glfw.KEY_S: 
            R = np.array([[np.cos(th), -np.sin(th), 0.],
                [np.sin(th), np.cos(th), 0.],
                [0., 0., 1.]])
            T = R @ T
        
def main():
    if not glfw.init():
        return
    
    window = glfw.create_window(480,480,"2018008304-3-1", None, None) 
    if not window:
        glfw.terminate()
        return

    glfw.set_key_callback(window, key_callback) 
    glfw.make_context_current(window)

    while not glfw.window_should_close(window): 
        glfw.poll_events()
        
        render(T)
        glfw.swap_buffers(window)
        
    glfw.terminate()

if __name__ == "__main__": 
    main()