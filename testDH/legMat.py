import numpy as np
from math import pi
from numpy import cos
from numpy import sin

t1, t2, t3 = (pi/2, pi/2, 0)
cosT1, cosT2, cosT3 = cos(t1), cos(t2), cos(t3)
sinT1, sinT2, sinT3 = sin(t1), sin(t2), sin(t3)

a1, a2, a3 = (pi/2, 0, 0)
cosA1, cosA2, cosA3 = cos(a1), cos(a2), cos(a3)
sinA1, sinA2, sinA3 = sin(a1), sin(a2), sin(a3)

r1, r2, r3 = (1, 1.5, 2)
d1, d2, d3 = (0, 0, 0)

transformMat = np.array([[cosT3*(cosT1*cosT2 - cosA1*sinT1*sinT2) - sinT3*(cosA2*cosT1*sinT2 - sinA1*sinA2*sinT1 + cosA1*cosA2*cosT2*sinT1), sinA3*(cosA2*sinA1*sinT1 + sinA2*cosT1*sinT2 + cosA1*sinA2*cosT2*sinT1) - cosA3*cosT3*(cosA2*cosT1*sinT2 - sinA1*sinA2*sinT1 + cosA1*cosA2*cosT2*sinT1) - cosA3*sinT3*(cosT1*cosT2 - cosA1*sinT1*sinT2), cosA3*(cosA2*sinA1*sinT1 + sinA2*cosT1*sinT2 + cosA1*sinA2*cosT2*sinT1) + sinA3*cosT3*(cosA2*cosT1*sinT2 - sinA1*sinA2*sinT1 + cosA1*cosA2*cosT2*sinT1) + sinA3*sinT3*(cosT1*cosT2 - cosA1*sinT1*sinT2), d3*(cosA2*sinA1*sinT1 + sinA2*cosT1*sinT2 + cosA1*sinA2*cosT2*sinT1) + r1*cosT1 + d2*sinA1*sinT1 + r2*cosT1*cosT2 + r3*cosT3*(cosT1*cosT2 - cosA1*sinT1*sinT2) - r3*sinT3*(cosA2*cosT1*sinT2 - sinA1*sinA2*sinT1 + cosA1*cosA2*cosT2*sinT1) - r2*cosA1*sinT1*sinT2],
  [ cosT3*(cosT2*sinT1 + cosA1*cosT1*sinT2) - sinT3*(sinA1*sinA2*cosT1 + cosA2*sinT1*sinT2 - cosA1*cosA2*cosT1*cosT2), - sinA3*(cosA2*sinA1*cosT1 - sinA2*sinT1*sinT2 + cosA1*sinA2*cosT1*cosT2) - cosA3*cosT3*(sinA1*sinA2*cosT1 + cosA2*sinT1*sinT2 - cosA1*cosA2*cosT1*cosT2) - cosA3*sinT3*(cosT2*sinT1 + cosA1*cosT1*sinT2), sinA3*cosT3*(sinA1*sinA2*cosT1 + cosA2*sinT1*sinT2 - cosA1*cosA2*cosT1*cosT2) - cosA3*(cosA2*sinA1*cosT1 - sinA2*sinT1*sinT2 + cosA1*sinA2*cosT1*cosT2) + sinA3*sinT3*(cosT2*sinT1 + cosA1*cosT1*sinT2), r1*sinT1 - d3*(cosA2*sinA1*cosT1 - sinA2*sinT1*sinT2 + cosA1*sinA2*cosT1*cosT2) - d2*sinA1*cosT1 + r2*cosT2*sinT1 - r3*sinT3*(sinA1*sinA2*cosT1 + cosA2*sinT1*sinT2 - cosA1*cosA2*cosT1*cosT2) + r3*cosT3*(cosT2*sinT1 + cosA1*cosT1*sinT2) + r2*cosA1*cosT1*sinT2],
  [sinT3*(cosA1*sinA2 + cosA2*sinA1*cosT2) + sinA1*cosT3*sinT2, sinA3*(cosA1*cosA2 - sinA1*sinA2*cosT2) + cosA3*cosT3*(cosA1*sinA2 + cosA2*sinA1*cosT2) - cosA3*sinA1*sinT2*sinT3, cosA3*(cosA1*cosA2 - sinA1*sinA2*cosT2) - sinA3*cosT3*(cosA1*sinA2 + cosA2*sinA1*cosT2) + sinA1*sinA3*sinT2*sinT3, d1 + d2*cosA1 + d3*(cosA1*cosA2 - sinA1*sinA2*cosT2) + r3*sinT3*(cosA1*sinA2 + cosA2*sinA1*cosT2) + r2*sinA1*sinT2 + r3*sinA1*cosT3*sinT2],
  [0,0,0,1]])

endEffector = np.array([0,0,0,1])
print np.dot(transformMat, endEffector)
