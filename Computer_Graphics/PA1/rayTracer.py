#!/usr/bin/env python3
# -*- coding: utf-8 -*
# sample_python aims to allow seamless integration with lua.
# see examples below

from ctypes.wintypes import RGB
from functools import reduce
import math
import os
import sys
import pdb  # use pdb.set_trace() for debugging
import code # or use code.interact(local=dict(globals(), **locals()))  for debugging.
import xml.etree.ElementTree as ET
import numpy as np
import torch
# from .utils.random import random_in_unit_disk
from PIL import Image
class Color:
    def __init__(self, R, G, B):
        self.color=np.array([R,G,B]).astype(np.float64)

    def gammaCorrect(self, gamma):
        inverseGamma = 1.0 / gamma
        self.color=np.power(self.color, inverseGamma)

    def toUINT8(self):
        return (np.clip(self.color, 0,1)*255).astype(np.uint8)

def normalize(vector):
    return vector / np.linalg.norm(vector)

class Camera(object):
    def __init__(self, viewPoint, viewDir, viewProjNormal, projDistance, viewUp, viewWidth, viewHeight, imgSize):
        self.viewPoint = viewPoint # 카메라 위치벡터(p)
        self.viewDir = viewDir # 카메라의 방향벡터(d)
        
        self.viewProjNormal = viewProjNormal # 카메라의 Normal Vector
        self.projDistance = projDistance # Focal Length

        self.viewUp = viewUp # Normal Vector
        self.viewWidth = viewWidth # 카메라의 시야넓이
        self.viewHeight = viewHeight # 카메라의 시야높이
        self.imgSize = imgSize
        
        d = normalize(self.viewDir)
        a = normalize(self.viewUp)
        self.u = normalize(np.cross(d, a))
        self.v = normalize(np.cross(d, self.u))
        self.w = normalize(np.cross(self.v, self.u)) # w = -d, d의 반대 방향
        
        # p - d * w 로 카메라뷰의 센터를 구해준다
        self.viewCenter = self.viewPoint - (self.projDistance * self.w)
        # self.horizontalVector = np.array([self.viewWidth[0], 0.0, 0.0])
        # self.verticalVector = np.array([0.0, self.viewHeight[0], 0.0])

        self.bottomLeftCorner = self.viewCenter - self.viewWidth/2 * self.u - self.viewHeight/2 * self.v
   
    def getPlanePoint(self, i, j):
        # u = l + self.viewWidth(i + 0.5) / n_x
        # v = b + self.viewHeight(j + 0.5) / n_y
        return self.bottomLeftCorner + (self.viewWidth / self.imgSize[0] * (i + 0.5)) * self.u + (self.viewHeight / self.imgSize[1] * (j + 0.5)) * self.v

    #i, j에서의 viewing ray vector을 return
    def getViewingRay(self, i, j):
        # return self.bottomLeftCorner + (self.horizontalVector * i / (self.imgSize[0] - 1)) + (self.verticalVector * j / (self.imgSize[1] - 1)) - self.viewPoint
        return normalize(self.getPlanePoint(i, j) - self.viewPoint)
    
    # t에서의 pixel 값을 return
    def getPixel(self, ray, t):
        return ray.origin + (ray.direction * t)

class Ray():
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction
    
class Shader():
    def __init__(self, name, type, diffuseColor, specularColor, exponent):
        self.name = name
        self.type = type
        self.diffuseColor = diffuseColor
        self.specularColor = specularColor
        self.exponent = exponent
        
class Surface(object):
    def __init__(self, shader):
        self.shader = shader

    def lambertian(self, pixel, normalVector, light):
        lightRay = normalize(light.position - pixel)
        return max(0, normalVector @ lightRay) * light.intensity
    
    def phong(self, viewingRay, pixel, normalVector, light, exponent):
        lightRay = normalize(light.position - pixel)
        h = normalize(lightRay - viewingRay)
        
        return max(0, pow(normalVector @ h, exponent)) * light.intensity
    
class Sphere(Surface):
    def __init__(self, shader, radius, center):
        self.shader = shader
        self.radius = radius
        self.center = center
        
    def getIntersectSphere(self, origin, ray):
        x = 2 * np.dot(ray, origin - self.center)
        y = np.linalg.norm(origin - self.center) ** 2 - self.radius ** 2
        delta = (x ** 2) - (4 * y)
        
        if delta > 0:
            t1 = (-x + np.sqrt(delta)) / 2
            t2 = (-x - np.sqrt(delta)) / 2
            if t1 > 0 and t2 > 0:
                return min(t1, t2)
        return None


    def getNearestSphere(self, origin, ray, surface):
        distances = [obj.getIntersectSphere(origin, ray) for obj in surface]
        nearestSphere = None
        minDistance = np.inf
        
        for index, distance in enumerate(distances):
            if distance and distance < minDistance:
                minDistance = distance
                nearestSphere = surface[index]
                
        return nearestSphere, minDistance
    
class Light():
    def __init__(self, position, intensity):
        self.position = position
        self.intensity = intensity

def main():
    tree = ET.parse(sys.argv[1])
    root = tree.getroot()

    # set default values
    viewDir=np.array([0,0,-1]).astype(np.float64)
    viewUp=np.array([0,1,0]).astype(np.float64)
    viewProjNormal=-1*viewDir 
    viewWidth=1.0
    viewHeight=1.0
    projDistance=1.0
    intensity=np.array([1,1,1]).astype(np.float64)  # how bright the light is.
    
    shader = []
    surface = []
    
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
        camera = Camera(viewPoint, viewDir, viewProjNormal, projDistance, viewUp, viewWidth, viewHeight, imgSize)
        
    for c in root.findall('shader'):
        type_c = c.get('type')
        diffuseColor_c=np.array(c.findtext('diffuseColor').split()).astype(np.float64)
        specularColor_c=np.array([.0, .0, .0])
        exponent_c = 0.0
        if (type_c == 'Phong'):
            specularColor_c=np.array(c.findtext('specularColor').split()).astype(np.float64)
            exponent_c=(np.float64)(c.findtext('exponent'))
        shader.append(Shader(c.get('name'), type_c, diffuseColor_c, specularColor_c, exponent_c))
        
    for c in root.findall('light'):
        intensity = np.array(c.findtext('intensity').split()).astype(np.float64)
        position_c = np.array(c.findtext('position').split()).astype(np.float64)
        
        light = (Light(position_c, intensity))
    
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

    for y in np.arange(imgSize[1]):
        for x in np.arange(imgSize[0]):
            pixel = 0.0
            ray = camera.getViewingRay(x, y) # 좌표 x,y 에서의 Ray Vector을 반환
            startSurface = None
            
            for s in surface:
                hitSurface, pixel = s.getNearestSphere(camera.viewPoint, ray, surface)
                if hitSurface is not None:
                    startSurface = hitSurface
                    
            if startSurface is not None:
                hitPoint = camera.viewPoint + pixel * ray
                normalVector = normalize(hitPoint - startSurface.center)
                
                for s in shader:
                    # print(s.name, startSurface.shader.name)    
                    if s.name == startSurface.shader.name:
                        color = np.array([0.0, 0.0, 0.0]) # init
                        
                        shadowSurface = None
                        shadowRay = (light.position - hitPoint) / normalize(light.position - hitPoint)
                        
                        for ss in surface:
                            shadowSurface, _ = ss.getNearestSphere(camera.viewPoint, shadowRay, surface)    

                        if shadowSurface is None:
                            if s.type == 'Phong':
                                color += s.specularColor * startSurface.phong(ray, hitPoint, normalVector, light, s.exponent)
                            color += s.diffuseColor * startSurface.lambertian(hitPoint, normalVector, light)
                        else: 
                            print('unknown shader type')  
                        
                        img[y][x] += Color(color[0], color[1], color[2]).toUINT8()
                        # print('img[',y,'][',x,'] = ', img[y][x])
          
    rawimg = Image.fromarray(img, 'RGB')
    rawimg.save(sys.argv[1] + '.png')

if __name__=="__main__":
    main()
