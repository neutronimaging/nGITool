# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import matplotlib.pyplot as plt
import numpy as np

plt.figure(figsize=[6,6])
xx, yy = np.meshgrid(np.arange(-512,511,1),np.arange(-512,511,1))
w=50*np.pi
d=0.5
R1=w*np.sqrt(np.square((xx/512)-d)+np.square((yy/512)-d))
R2=w*np.sqrt(np.square((xx/512)+d)+np.square((yy/512)-d))
R3=w*np.sqrt(np.square((xx/512))+np.square((yy/512)+d))
z=np.cos(R1+0.25*np.pi)+np.cos(R2)+np.cos(R3)
plt.imshow(z, cmap='viridis')
plt.axis('off')
plt.gca().set_position([0, 0, 1, 1])
plt.savefig("test.svg")