#ifndef TEST_PARTS_H_
#define TEST_PARTS_H_

#include <memory>

#include "kinematic_pair.h"
#include "leg.h"
#include "ik_solver.h"
#include "chassis.h"

// Test Leg config: Alpha, R, D

inline static void GetTestJoints(RevoluteJoint* joints, bool right) {
  joints[0] = RevoluteJoint(M_PI/2.0, .5, 0);
  if (right) {
    joints[1] = RevoluteJoint(0.0, 1.5, -.2);
    joints[2] = RevoluteJoint(0.0, 2.0, .2);
  } else {
    joints[1] = RevoluteJoint(0.0, 1.5, .2);
    joints[2] = RevoluteJoint(0.0, 2.0, -.2);
  }
}
inline static void GetTestJoints(RevoluteJoint* joints) { return GetTestJoints(joints, true); }

inline static Leg<3> GetTestLeg(bool right) {
  RevoluteJoint joints[3];
  GetTestJoints(joints, right);
  return Leg<3>(joints);
}

inline static Leg<3> GetTestLeg() { return GetTestLeg(true); }

inline static LegController<3> GetTestLegController(Leg<3>* leg) {
  return LegController<3>(leg, std::unique_ptr<IKSolver>(new IK3DoF(leg->Joint(0), leg->Joint(1), leg->Joint(2))));
}


template <int n_legs>
inline static Chassis<n_legs, 3> GetTestChassis() {
  // Legs: Counter Clockwise from front-right
  Leg<3> legs[n_legs];

  // Right legs
  for (int i = 0; i < n_legs/2; i++) {
    legs[i] = GetTestLeg(true);
  }

  // Left legs
  for (int i = n_legs/2; i < n_legs; i++) {
    legs[i] = GetTestLeg(true);
  }

  // Leg poses for a chassis of a perfect polygon.
  Pose poses[n_legs];
  double angle_diff = (2*M_PI)/n_legs;
  for (int i = 0; i < n_legs; i++) {
    // Angle wrt the chassis face
    double curr_angle = angle_diff/2.0 + i*angle_diff;
    if (curr_angle > M_PI && curr_angle <= 2*M_PI) {
      curr_angle = -(2*M_PI - curr_angle);
    }
    poses[i] = Pose(cos(curr_angle), sin(curr_angle), 0.0, curr_angle, 0.0, 0.0);
  }

  /*
  // IK Solvers
  std::unique_ptr<IKSolver> solvers[n_legs];
  for (int i = 0; i < n_legs; i++) {
    solvers[i] = std::unique_ptr(new IK3DoF(legs[i].Joint(0), legs[i].Joint(1), legs[i].Joint(2)));
  }
  */

  return Chassis<n_legs, 3>(legs, poses);
}


#endif // TEST_PARTS_H_
