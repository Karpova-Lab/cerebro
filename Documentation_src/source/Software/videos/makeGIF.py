from moviepy.editor import *
from sys import argv

script,movieFile = argv

clip = (VideoFileClip(movieFile)).fadeout(.5,final_color=[255,255,255])
clip.write_gif(movieFile[:-3]+"gif",fps = 20)
