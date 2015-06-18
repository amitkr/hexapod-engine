#ifndef LEG_H
#define LEG_H

#include <cstdlib>
#include <vector>
#include <Eigen/Core>
#include <Eigen/LU>
#include <cmath>

#include "kinematic_pair.h"
#include "drawing_primitives.h"
#include "ik_solver.h"
#include "interpolators.h"



template <int n_joints> struct LegCommand {
  RevoluteJointCommand joint_commands[n_joints];

  LegCommand() {};
  LegCommand(RevoluteJointCommand _joint_commands[n_joints]) {
    for (int i = 0; i < n_joints; i++) {
      joint_commands[i] = _joint_commands[i];
    }
  };
};

// Leg representation made up of only revolute joints.
// The "mechanical" representation, taking only joint angles and speeds as input.
template <int n_joints> class Leg : public Drawable {
protected:
  RevoluteJoint joints[n_joints];

public:

  Leg(RevoluteJoint _joints[n_joints]) {
    DrawLock();
    for (int i = 0; i < n_joints; i++) {
      joints[i] = _joints[i];
    }
    DrawUnlock();
  }

  void SetState(double angles[n_joints]) {
    DrawLock();
    for (int i = 0; i < n_joints; i++) {
      joints[i].SetTheta(angles[i]);
    }
    DrawUnlock();
  }

  inline const RevoluteJoint* Joint(int i) const { return &joints[i]; };

  // Play the leg through in time toward its commanded destination.
  void UpdateState(double time_elapsed) {
    DrawLock();
    for (int i = 0; i < n_joints; i++) {
      joints[i].UpdateState(time_elapsed);
    }
    DrawUnlock();
  }

  // Propagate a set of commands to each of the joints.
  void SetCommand(LegCommand<n_joints> command) {
    for (int i = 0; i < n_joints; i++) {
      joints[i].SetCommand(command.joint_commands[i]);
    }
  }

  bool IsMoving() {
    for (int i = 0; i < n_joints; i++) {
      if (joints[i].IsMoving()) {
        return true;
      }
    }
    return false;
  }

  Eigen::Vector4d ToGlobal(Eigen::Vector4d in) {
    Eigen::Matrix4d compound = joints[0].DHMat();
    for (int i = 1; i < n_joints; i++) {
      compound *= joints[i].DHMat();
    }
    return compound * in;
  }

  // If you don't specify a point, it defaults to the origin.
  inline Eigen::Vector4d ToGlobal() {
    return ToGlobal(Eigen::Vector4d(0.0, 0.0, 0.0, 1.0));
  }; 

  Eigen::Vector3d ToGlobal3() {
    return ((Eigen::Vector4d)ToGlobal(Eigen::Vector4d(0.0, 0.0, 0.0, 1.0))).block<3,1>(0, 0);
  }; 

  // Returns the origins of each of the segments in the global coordinate system
  void AllOrigins(Eigen::Vector4d points[n_joints + 1]) {
    DrawLock();
    Eigen::Vector4d origin(0.0, 0.0, 0.0, 1.0);
    points[0] = origin;

    Eigen::Matrix4d compound(joints[0].DHMat());
    for (int i = 1; i < n_joints; i++) {
      points[i] = compound * origin;
      compound *= joints[i].DHMat();
    }
    points[n_joints] = compound * origin;
    DrawUnlock();
  }

  // Returns the basic structure of each of the joints.
  // SLightly More detailed version of AllOrigins.
  void AllJoints(Eigen::Vector4d points[2*n_joints]) {
    DrawLock();
    Eigen::Vector4d origin(0.0, 0.0, 0.0, 1.0);
    Eigen::Vector4d offset(0.0, 0.0, 0.0, 1.0);
    Eigen::Matrix4d compound(Eigen::Matrix4d::Identity(4, 4));

    for (int i = 0; i < n_joints; i++) {
      compound *= joints[i].DHMat();
      offset[0] = -joints[i].R();
      points[2*i] = compound * offset;
      points[2*i + 1] = compound * origin;
    }
    DrawUnlock();
  }

  // Implement the drawable interface.
  // 
  void Draw(Eigen::Matrix4d to_global) {
    int strip_size = 2*n_joints;
    Eigen::Vector4d segs[strip_size];
    AllJoints(segs);

    // Transform the points into the global frame using the provided matrix.
    for (int i = 0; i < strip_size; i++) {
      segs[i] = to_global * segs[i];
    }
    LineStrip(strip_size, segs, 1.0, 0.0, 0.0);
  }
};


// Class to have more fine tuned control over the exact paths taken to a destination.
template <int n_joints> class LegController : public Leg<n_joints> {
protected:
  IKSolver* ik_solver;
  Eigen::Vector3d dest;
  double deadline, current_time;
  PathGen* path;

public:
  LegController(RevoluteJoint _joints[n_joints], IKSolver* ik_solver) : Leg<n_joints>(_joints), ik_solver(ik_solver) {}; 

  void SetControl(PathGen* path, double deadline);
  int GetJointCommands(Eigen::Vector3d point, double current_deadline, LegCommand<n_joints>* command_to_set);
  int GetJointCommands(Eigen::Vector3d point, double joint_angles[n_joints]);
  void UpdateState(double time_elapsed);

  // Override the Leg's draw method
  void Draw(Eigen::Matrix4d to_global);
};

#endif // LEG_H
