import numpy as np
from math import cos, sin


# Represents an arbitrary position and orientation in space

class pose:
    position = (0, 0, 0)
    yaw = None
    pitch = None
    roll = None

    transformMat = None
    inverseTransformMat = None

    def __init__(self, (x, y, z), (yaw, pitch, roll)):
        self.position = (x, y, z)
        self.yaw = yaw
        self.pitch = pitch
        self.roll = roll
        cy = cos(yaw)  # yaw
        sy = sin(yaw)
        cp = cos(pitch)  # pitch
        sp = sin(pitch)
        cr = cos(roll)  # roll
        sr = sin(roll)
        rotMat = np.array([[cy*cp, cy*sp*sr - sy*cr, cy*sp*cr + sy*sr],
                           [sy*cp, sy*sp*sr + cy*cr, sy*sp*cr - cy*sr],
                           [-sp, cp*sr, cp*cr]])
        self.transformMat = np.vstack([np.hstack((rotMat, np.array([self.position]).T)), [0, 0, 0, 1]])
        self.inverseTransformMat = np.linalg.inv(self.transformMat)

    def to_local(self, point):
        if len(point) is 3:
            point = np.append(point, [1])
        return np.dot(self.inverseTransformMat, point)[:3]

    def to_global(self, point):
        if len(point) is 3:
            point = np.append(point, [1])
        return np.dot(self.transformMat, point)[:3]

    def copy(self):
        return pose(self.position, (self.yaw, self.pitch, self.roll))

    def l2_norm_squared(self, pose_in):
        return sum([(self.position[i] - pose_in.position[i])**2 for i in xrange(len(self.position))])
