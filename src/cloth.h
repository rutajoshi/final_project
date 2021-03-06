#ifndef CLOTH_H
#define CLOTH_H

#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "CGL/CGL.h"
#include "CGL/misc.h"
#include "clothMesh.h"
#include "collision/collisionObject.h"
#include "spring.h"

using namespace CGL;
using namespace std;

enum e_orientation { HORIZONTAL = 0, VERTICAL = 1 };

struct ClothParameters {
  ClothParameters() {}
  ClothParameters(bool enable_structural_constraints,
                  bool enable_shearing_constraints,
                  bool enable_bending_constraints, double damping,
                  double density, double ks)
      : enable_structural_constraints(enable_structural_constraints),
        enable_shearing_constraints(enable_shearing_constraints),
        enable_bending_constraints(enable_bending_constraints),
        damping(damping), density(density), ks(ks) {}
  ~ClothParameters() {}

  // Global simulation parameters

  bool enable_structural_constraints;
  bool enable_shearing_constraints;
  bool enable_bending_constraints;

  double damping;

  // Mass-spring parameters
  double density;
  double ks;
};

struct Cloth {
  Cloth() {}
  Cloth(double width, double height, double depth, int num_width_points,
        int num_height_points, int num_depth_points, float thickness);
  ~Cloth();

  void buildGrid();

  void simulate(double frames_per_sec, double simulation_steps, ClothParameters *cp,
                vector<Vector3D> external_accelerations,
                vector<CollisionObject *> *collision_objects);

  void reset();
  void buildClothMesh();

  void build_spatial_map();
  void self_collide(PointMass &pm, double simulation_steps);
  float hash_position(Vector3D pos);
  Vector3D calculate_delta_p(PointMass &pm_i);
  double kernel_poly6(Vector3D pos_dif, double radius);
  void lambda_i(PointMass &pm);
  double delta_constraint_pk(PointMass &pm_i, PointMass &pm_k);
  double spiky_kernel(Vector3D pos_diff, double h);
  Vector3D spiky_kernel_grad(Vector3D pos_dif, double radius);
  double calculate_density_neighbors(PointMass &pm);

  Vector3D location_vector(PointMass &pm_i);
  Vector3D force_vorticity_i(PointMass &pm_i);
  void viscosity_constraint(PointMass &pm_i);
  Vector3D vorticity_wi(PointMass &pm_i);
  double viscosity_kernel(Vector3D pos_dif, double h);
  double calc_h();
  Vector3D calc_delta_q();
  vector<PointMass *> *get_neighbors(PointMass &pm);

  // Cloth properties
  double width;
  double height;
  double depth;
  int num_width_points;
  int num_height_points;
  int num_depth_points;
  double thickness;
  e_orientation orientation;
  double epsilon;

  // Cloth components
  vector<PointMass> point_masses;
  vector<vector<int>> pinned;
  vector<Spring> springs;
  ClothMesh *clothMesh;

  // Spatial hashing
  unordered_map<float, vector<PointMass *> *> map;
};

#endif /* CLOTH_H */
