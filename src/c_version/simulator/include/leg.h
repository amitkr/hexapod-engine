#ifndef LEG_H
#define LEG_H

#include <cstdlib>
#include <vector>
#include <Eigen/Core>
#include <Eigen/LU>
#include <cmath>
#include <memory>

#include "drawing_primitives.h"
#include "kinematic_pair.h"
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

// Forward declare so we can make the regular leg a friend.
template <int n_joints> class LegController;

// Leg representation made up of only revolute joints.
// The "mechanical" representation, taking only joint angles and speeds as input.
template <int n_joints> class Leg : public Drawable {
// Allow the controller class to have direct access to joints.
friend class LegController<n_joints>;
protected:
  RevoluteJoint joints[n_joints];

  // The leg's pose relative to some parent coordinate system.
  // Maybe global, maybe chassis.
  Pose local_pose;

public:
  Leg() {};
  Leg(RevoluteJoint _joints[n_joints], Pose local_pose) : local_pose(local_pose) {
    DrawLock();
    for (int i = 0; i < n_joints; i++) {
      joints[i] = _joints[i];
    }
    DrawUnlock();
  }
  Leg(RevoluteJoint _joints[n_joints]) : local_pose(Pose(0,0,0,0,0,0)) {
    for (int i = 0; i < n_joints; i++) {
      joints[i] = _joints[i];
    }
  }

  void SetState(double angles[n_joints]) {
    DrawLock();
    for (int i = 0; i < n_joints; i++) {
      joints[i].SetTheta(angles[i]);
    }
    DrawUnlock();
  }

  inline const RevoluteJoint& Joint(int i) const { return joints[i]; };

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

  bool IsMoving() const {
    for (int i = 0; i < n_joints; i++) {
      if (joints[i].IsMoving()) {
        return true;
      }
    }
    return false;
  }

  Eigen::Vector4d ToGlobal(Eigen::Vector4d in) {
    Eigen::Matrix4d compound = local_pose.FromFrameMat();
    for (int i = 0; i < n_joints; i++) {
      compound *= joints[i].DHMat();
    }
    return compound * in;
  }

  // If you don't specify a point, it defaults to the origin.
  inline Eigen::Vector4d ToGlobal() {
    return ToGlobal(Eigen::Vector4d(0.0, 0.0, 0.0, 1.0));
  };

  // Returns the origins of each of the segments in the global coordinate system
  void AllOrigins(Eigen::Vector4d points[n_joints + 1]) {
    DrawLock();
    Eigen::Vector4d origin(0.0, 0.0, 0.0, 1.0);

    Eigen::Matrix4d compound(local_pose.FromFrameMat());
    for (int i = 0; i < n_joints; i++) {
      points[i] = compound * origin;
      compound *= joints[i].DHMat();
    }
    points[n_joints] = compound * origin;
    DrawUnlock();
  }

  // Returns the basic structure of each of the joints.
  // Slightly More detailed version of AllOrigins.
  void AllJoints(Eigen::Vector4d points[2*n_joints]) {
    DrawLock();
    Eigen::Vector4d origin(0.0, 0.0, 0.0, 1.0);
    Eigen::Vector4d offset(0.0, 0.0, 0.0, 1.0);
    Eigen::Matrix4d compound(local_pose.FromFrameMat());
    for (int i = 0; i < n_joints; i++) {
      compound *= joints[i].DHMat();
      offset[0] = -joints[i].R();
      points[2*i] = compound * offset;
      points[2*i + 1] = compound * origin;
    }
    DrawUnlock();
  }

  inline const Pose& LegPose() {
    return local_pose;
  }

  // Implement the drawable interface.
  void Draw(Eigen::Matrix4d to_global) {
    int strip_size = 2*n_joints;
    Eigen::Vector4d segs[strip_size];
    double r[strip_size];
    double g[strip_size];
    double b[strip_size];
    AllJoints(segs);

    // Transform the points into the global frame using the provided matrix.
    for (int i = 0; i < strip_size; i++) {
      segs[i] = to_global * segs[i];
      double val = .2 + .8*(double)i/(strip_size - 1);
      r[i] = val;
      b[i] = val;
      g[i] = val;
    }
    LineStrip(strip_size, segs, r, g, b);
  }

  void Draw(Eigen::Matrix4d to_global, double r_in, double g_in, double b_in) {
    int strip_size = 2*n_joints;
    Eigen::Vector4d segs[strip_size];
    double r[strip_size];
    double g[strip_size];
    double b[strip_size];
    AllJoints(segs);

    // Transform the points into the global frame using the provided matrix.
    for (int i = 0; i < strip_size; i++) {
      segs[i] = to_global * segs[i];
      double val = .2 + .8*(double)i/(strip_size - 1);
      r[i] = r_in*val;
      b[i] = g_in*val;
      g[i] = b_in*val;
    }
    LineStrip(strip_size, segs, r, g, b);
  }
};


// Class to have more fine tuned control over the exact paths taken to a destination.
template <int n_joints> class LegController : public Drawable {
protected:
  // A pointer to the leg that is attempted to be controlled.
  // This should be updated(externally) in accordance with the control inputs generated by this controller.
  Leg<n_joints>* model;

  // A reference to an IK solver that works with the leg model above.
  std::unique_ptr<IKSolver> ik_solver;

  // Deadline is the time when the controller is attempting to finish its current movement
  // Current time is the progress since that command was started.
  double deadline = 0;
  double current_time = 0;

  // This is the path the controller should take.
  PathGen* path = NULL;
  bool motion_complete = true;
  bool infeasible= false;

  static constexpr double destination_epsilon_squared = 1e-5*1e-5;

public:
  LegController() {};
  LegController(Leg<n_joints>* _model, std::unique_ptr<IKSolver> ik_solver) : model(_model), ik_solver(std::move(ik_solver)) {}; 

  void SetState(double angles[n_joints]);
  void SetControl(PathGen* path, double deadline);

  double GetJointCommands(Eigen::Vector3d point, double current_deadline, LegCommand<n_joints>* command_to_set);
  double GetJointCommands(Eigen::Vector3d point, double joint_angles[n_joints]);

  // Updates the state of the controller, not the state of the underlying model.
  void UpdateState(double time_elapsed, LegCommand<n_joints>* out_command);
  inline bool InProgress() const { return !IsOverdue() || (!motion_complete && !infeasible); }
  inline bool IsOverdue() const { return (current_time > deadline); };
  Eigen::Vector3d GetEndpoint();

  inline const Leg<n_joints>* Model() const { return model; };

  void Draw(Eigen::Matrix4d to_global);
};

#endif // LEG_H
