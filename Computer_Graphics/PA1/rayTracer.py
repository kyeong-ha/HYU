#!/usr/bin/env python3
# -*- coding: utf-8 -*
# sample_python aims to allow seamless integration with lua.
# see examples below

from ctypes.wintypes import RGB
from functools import reduce
import os
import sys
import pdb  # use pdb.set_trace() for debugging
import code # or use code.interact(local=dict(globals(), **locals()))  for debugging.
import xml.etree.ElementTree as ET
import numpy as np
import torch
from .utils.random import random_in_unit_disk
from PIL import Image
class Color:
    def __init__(self, R, G, B):
        self.color=np.array([R,G,B]).astype(np.float64)

    def gammaCorrect(self, gamma):
        inverseGamma = 1.0 / gamma
        self.color=np.power(self.color, inverseGamma)

    def toUINT8(self):
        return (np.clip(self.color, 0,1)*255).astype(np.uint8)

class Camera():
    def __init__(self, viewPoint, viewDir, viewProjNormal, projDistance, viewUp, viewWidth, viewHeight):
        self.viewPoint = viewPoint
        self.viewDir = viewDir
        
        self.viewProjNormal = viewProjNormal
        self.projDistance = projDistance

        self.viewUp = viewUp
        self.viewWidth = viewWidth
        self.viewHeight = viewHeight

    # def getRay(self):
class Shader():
    def __init__(self, name, type, diffuseColor, specularColor=[0,0,0], exponent=0):
        self.name = name
        self.type = type
        self.diffuseColor = diffuseColor
        self.specularColor = specularColor
        self.exponent = exponent
        
class Sphere():
    def __init__(self, shader, radius, center):
        self.shader = shader
        self.radius = radius
        self.center = center
    
class Light():
    def __init__(self, position, intensity):
        self.position = position
        self.color = intensity

def main():
    tree = ET.parse(sys.argv[1])
    root = tree.getroot()

    # set default values
    viewDir=np.array([0,0,-1]).astype(np.float64)
    viewUp=np.array([0,1,0]).astype(np.float64)
    viewProjNormal=-1*viewDir  # you can safely assume this. (no examples will use shifted perspective camera)
    viewWidth=1.0
    viewHeight=1.0
    projDistance=1.0
    intensity=np.array([1,1,1]).astype(np.float64)  # how bright the light is.
    
    shader = []
    surface = []
    print(np.cross(viewDir, viewUp))

    imgSize=np.array(root.findtext('image').split()).astype(np.int32)
    
    for c in root.findall('camera'):
        viewPoint=np.array(c.findtext('viewPoint').split()).astype(np.float64)
        viewDir = np.array(c.findtext('viewDir').split()).astype(np.float64)
        viewProjNormal = np.array(c.findtext('projNormal').split()).astype(np.float64)
        viewUp = np.array(c.findtext('viewUp').split()).astype(np.float64)
        if (c.findtext('projDistance')):
            projDistance = np.array(c.findtext('projDistance').split()).astype(np.float64)
        viewWidth = np.array(c.findtext('viewWidth').split()).astype(np.float64)
        viewHeight = np.array(c.findtext('viewHeight').split()).astype(np.float64)
        camera = Camera(viewPoint, viewDir, viewProjNormal, projDistance, viewUp, viewWidth, viewHeight)
        # print('viewpoint', viewPoint)
        
    for c in root.findall('shader'):
        type_c = c.get('type')
        name_c = c.get('name')
        if type_c == 'Phong':
            diffuseColor_c=np.array(c.findtext('diffuseColor').split()).astype(np.float64)
            shader.append(Shader(name_c, type_c, diffuseColor_c))
        elif type_c == 'Lambertian':
            diffuseColor_c=np.array(c.findtext('diffuseColor').split()).astype(np.float64)
            specularColor_c = np.array(c.findtext('specularColor').split()).astype(np.float64)
            exponent_c = np.array(c.findtext('exponent').split()).astype(np.float64)
            shader.append(Shader(name_c, type_c, diffuseColor_c, specularColor_c, exponent_c))
        # print('name', c.get('name'))
        # print('diffuseColor', diffuseColor_c)
        
    for c in root.findall('light'):
        intensity_c = np.array(c.findtext('intensity').split()).astype(np.float64)
        position_c = np.array(c.findtext('position').split()).astype(np.float64)
        
        light = (Light(position_c, intensity_c))
    
    for c in root.findall('surface'):
        type_c = c.get('type')
        if type_c == 'Sphere':
            center_c = np.array(c.findtext('center').split()).astype(np.float64)
            radius_c = np.array(c.findtext('radius')).astype(np.float64)
            ref = ''
            for child in c:
                if child.tag == 'shader':
                    ref = child.get('ref')
                    for d in shader:
                        if d.name == ref:
                            surface.append(Sphere(d, radius_c, center_c))
                            break
    #code.interact(local=dict(globals(), **locals()))  

    # Create an empty image
    channels=3
    img = np.zeros((imgSize[1], imgSize[0], channels), dtype=np.uint8)
    img[:,:]=0

    for x in np.arange(imgSize[0]):
        for y in np.arange(imgSize[1]):
            ray = start + u_unit * x * view.x + view.y * y * v_unit
            tmp = raytrace(list, ray, view.viewPoint)
            img[y][x] = shade(tmp[0], ray, view, list, tmp[1], light)

    rawimg = Image.fromarray(img, 'RGB')
    rawimg.save(sys.argv[1] + '.png')


    # rawimg = Image.fromarray(img, 'RGB')
    # #rawimg.save('out.png')
    # rawimg.save(sys.argv[1]+'.png')
    
if __name__=="__main__":
    main()
