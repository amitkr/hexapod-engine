#ifndef KINEMATIC_CHAIN_H
#define KINEMATIC_CHAIN_H

#include <cstdlib>
#include <vector>
#include <Eigen/Core>
#include <Eigen/LU>
#include <cmath>

class KinematicPair {
protected:
  double alpha, r, d, theta;
  bool params_changed;
  Eigen::Matrix4d dh_mat;
  Eigen::Matrix4d inv_dh_mat;
  void GenerateDHMatrices();

public:
  KinematicPair(double _alpha, double _r, double _d, double _theta) : alpha(_alpha), r(_r), d(_d), theta(_theta), params_changed(true) {};

  // Accessors
  inline double Alpha() const { return alpha; };
  inline double R() const { return r; };
  inline double D() const { return d; };
  inline double Theta() const { return theta; };
  inline bool ParamsChanged() const { return params_changed; };
  inline const Eigen::Matrix4d& DHMat() {
    if (params_changed) {
      GenerateDHMatrices();
      params_changed = false;
    }
    return dh_mat;
  };
  inline const Eigen::Matrix<double, 4, 4>& InvDHMat() {
    if (params_changed) {
      GenerateDHMatrices();
      params_changed = false;
    }
    return inv_dh_mat;
  };
};

class RevoluteJoint : public KinematicPair {
  double min_theta, max_theta;

public:
  RevoluteJoint(double _min_theta, double _max_theta, double _alpha, double _r, double _d) : KinematicPair(_alpha, _r, _d, (_min_theta + _max_theta)/2.0) {};
  RevoluteJoint(double _alpha, double _r, double _d) : RevoluteJoint(-M_PI, M_PI, _alpha, _r, _d) {};
  void SetTheta(double theta);
};

class Leg {
private:

public:
  RevoluteJoint coxa, femur, tibia;
  Leg(RevoluteJoint coxa, RevoluteJoint femur, RevoluteJoint tibia) : coxa(coxa), femur(femur), tibia(tibia) {};

  void SetState(double coxa_angle, double femur_angle, double tibia_angle);

  Eigen::Vector4d ToGlobal(Eigen::Vector4d in); 

  // If you don't specify a point, it defaults to the origin.
  inline Eigen::Vector4d ToGlobal() {
    return ToGlobal(Eigen::Vector4d(0.0, 0.0, 0.0, 1.0));
  }; 

  // Must have number of segments + 1 points
  void AllSegments(Eigen::Vector4d points[4]);
};


/*
struct LegState {
  double theta_coxa, theta_femur, theta_tibia;
  LegState(double _theta_coxa, double _theta_femur, double _theta_tibia);
}

class HexLeg {
public:
  double (*x_to_leg_coxa)(double base_x, double base_y, double theta_coxa);
  double (*y_to_leg_coxa)(double theta_coxa);
  double (*z_to_leg_coxa)(double theta_coxa);
  double (*x_from_leg_coxa)(double theta_coxa);
  double (*y_from_leg_coxa)(double theta_coxa);
  double (*z_from_leg_coxa)(double theta_coxa);

  HexLeg() = delete;
  HexLeg(double (*_x_to_leg_coxa)(double, double, double));
};
*/

#endif // KINEMATIC_CHAIN_H
