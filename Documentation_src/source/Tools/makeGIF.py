from moviepy.editor import *

clip = (VideoFileClip("edit label2.mov")).fadeout(1,final_color=[255,255,255])
clip.write_gif("edit label2.gif",fps = 20)
