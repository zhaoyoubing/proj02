#ifndef __CLOTH_SIM_MESH__
#define __CLOTH_SIM_MESH__

#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"

#include "PlaneMesh.h"

class ClothSim
{
public:
	ClothSim(std::shared_ptr<PlaneMesh> pMesh);
	//ClothSim(const int rx, const int rz, const int w, const int l);
	~ClothSim();
	
    // void draw(GLFWwindow *window);
	void tick(float dt, GLFWwindow *window);

private:

    std::shared_ptr<PlaneMesh> mesh;

	void accumulateForces(GLFWwindow *window);
	void verletIntegration(float dt, int n_iterations);
	void forwardEulerIntegration(float dt);
	//void backwardEulerIntegration(float dt);

	struct Directions {
		const int WEST = 0;
		const int NORTHWEST = 1;
		const int NORTH = 2;
		const int NORTHEAST = 3;
		const int EAST = 4;
		const int SOUTHEAST = 5;
		const int SOUTH = 6;
		const int SOUTHWEST = 7;
	};

	Directions DIRS;

	int getId(int direction, int id);
	glm::vec3 getSpringForce(int direction, int id);
	
	void init();
	void reInit();


	// Properties
	std::vector<glm::vec3> forces;
	std::vector<glm::vec3> velocities;

	std::vector<glm::vec3> init_positions;
	std::vector<glm::vec3> prev_positions;
	std::vector<glm::vec3> prev_velocities;
	std::vector<glm::vec3> prev_accelerations;
	
	bool playSimulation = false;
	float restLengthX, restLengthZ, restLengthXZ = 0;
	float spring_factor = 800.9f;
	float damping_factor = .9960;
	float wind_factor = 0.0f;
	float air_resistance = .08f;
	glm::vec3 gravity = glm::vec3(0, -9.82f, 0);

	int width, length;
	int res_x, res_z;
};

#endif