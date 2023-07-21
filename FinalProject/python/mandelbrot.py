from PIL import Image, ImageDraw
from math import log, log2

MAX_ITER = 80

def mandelbrot_v1(c):
    z= 0 
    n =0
    while abs(z) <= 2 and n < MAX_ITER:
        z = z*z + c
        n = n+1
    return n

MAX_ITER = 80
def mandelbrot_v2(c):
    z = 0
    n = 0
    while abs(z) <= 2 and n < MAX_ITER:
        z = z**2 + c
        n += 1

    if n == MAX_ITER:
        return MAX_ITER
    
    return n + 1 - log(log2(abs(z)))
#IMAG SIZE 
WIDTH = 600
HEIGHT = 400
# PLOT WINDOW
RE_START = -2
RE_END = 1
IM_START = -1
IM_END = 1

im = Image.new('HSV',(WIDTH,HEIGHT),(0,0,0))
draw = ImageDraw.Draw(im)

for x in range (0,WIDTH):
    for y in range(0,HEIGHT):
        #CONVERT PIXEL COORDINATE TO COMPLEX NUMBER
        c = complex(RE_START + (x / WIDTH) * (RE_END - RE_START),
                    IM_START +  (y / HEIGHT)* (IM_END - IM_START))
        #compute the numbers of interations
        m = mandelbrot_v2(c)
        # the color depends on the number of iteration
        hue = int(255 * m/ MAX_ITER)
        seturation = 255
        value = 255 if m < MAX_ITER else 0 
        #piot the point 
        draw.point([x,y],(hue,seturation,value))
im.convert('RGB').save('output.png','PNG')
