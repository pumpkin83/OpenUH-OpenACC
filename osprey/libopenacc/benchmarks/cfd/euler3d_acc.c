// Copyright 2009, Andrew Corrigan, acorriga@gmu.edu
// This code is from the AIAA-2009-4001 paper


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

typedef struct float3{
	float x;
	float y;
	float z;
}float3;

#ifndef block_length
//	#define block_length 192
int block_length = 192;
#endif

/*
 * Options
 *
 */
//#define GAMMA 1.4
//#define iterations 2000

//#define NDIM 3
//#define NNB 4

//#define RK 3	// 3rd order RK
//#define ff_mach 1.2
//#define deg_angle_of_attack 0.0f

int GAMMA = 1.4;
int iterations = 2000;
int NDIM = 3;
int NNB = 4;
int RK = 3;
int ff_mach = 1.2;
int deg_angle_of_attack = 0.0f;

/*
 * not options
 */
//#define VAR_DENSITY 0
//#define VAR_MOMENTUM  1
//#define VAR_DENSITY_ENERGY (VAR_MOMENTUM+NDIM)
//#define NVAR (VAR_DENSITY_ENERGY+1)

int VAR_DENSITY = 0;
int VAR_MOMENTUM = 1;
int VAR_DENSITY_ENERGY = 4;
int NVAR = 5;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/*
 * Element-based Cell-centered FVM solver functions
 */
//float ff_variable[NVAR];
float ff_variable[5];
float3 ff_flux_contribution_momentum_x;
float3 ff_flux_contribution_momentum_y;
float3 ff_flux_contribution_momentum_z;
float3 ff_flux_contribution_density_energy;

void dump(float* variables, int nel, int nelr);
void initialize_variables(int nelr, float* variables);
inline void compute_flux_contribution(float density, float3* momentum, float density_energy, float pressure, float3* velocity, float3* fc_momentum_x, float3* fc_momentum_y, float3* fc_momentum_z, float3* fc_density_energy);
inline void compute_velocity(float density, float3* momentum, float3* velocity);
inline float compute_speed_sqd(float3* velocity);
inline float compute_pressure(float density, float density_energy, float speed_sqd);
inline float compute_speed_of_sound(float density, float pressure);
void compute_step_factor(int nelr, float* variables, float* areas, float* step_factors);
void compute_flux(int nelr, int* elements_surrounding_elements, float* normals, float* variables, float* fluxes);
void time_step(int j, int nelr, float* old_variables, float* variables, float* step_factors, float* fluxes);

void dump(float* variables, int nel, int nelr)
{
	int i, j;
	FILE *fh1, *fh2, *fh3;

	fh1 = fopen("density", "wt");

	fprintf(fh1, "%d %d\n", nel, nelr);
	for(i=0; i<nel; i++)
		fprintf(fh1, "%f\n", variables[i*NVAR + VAR_DENSITY]);
	fclose(fh1);

	fh2 = fopen("momentum", "wt");
	fprintf(fh2, "%d %d\n", nel, nelr);
	for(i=0; i<nel; i++)
	{
		for(j=0; j!=NDIM;j++)
			fprintf(fh2, "%f ", variables[i*NVAR + (VAR_MOMENTUM+j)]);
		fprintf(fh2, "\n");
	}
	fclose(fh2);
	
	fh3 = fopen("density_energy", "wt");
	fprintf(fh3, "%d %d\n", nel, nelr);
	for(i=0; i<nel; i++)
		fprintf(fh3, "%f\n", variables[i*NVAR + VAR_DENSITY_ENERGY]);
	fclose(fh3);


}



void initialize_variables(int nelr, float* variables)
{
	int i, j;

	for(i = 0; i < nelr; i++)
	{
		for(j = 0; j < NVAR; j++) variables[i*NVAR + j] = ff_variable[j];
	}
}

inline void compute_flux_contribution(float density, float3* momentum, float density_energy, float pressure, float3* velocity, float3* fc_momentum_x, float3* fc_momentum_y, float3* fc_momentum_z, float3* fc_density_energy)
{
	fc_momentum_x->x = (velocity->x)*(momentum->x) + pressure;
	fc_momentum_x->y = (velocity->x)*(momentum->y);
	fc_momentum_x->z = (velocity->x)*(momentum->z);

	fc_momentum_y->x = fc_momentum_x->y;
	fc_momentum_y->y = (velocity->y)*(momentum->y) + pressure;
	fc_momentum_y->z = (velocity->y)*(momentum->z);

	fc_momentum_z->x = fc_momentum_x->z;
	fc_momentum_z->y = fc_momentum_y->z;
	fc_momentum_z->z = (velocity->z)*(momentum->z) + pressure;

	float de_p = density_energy+pressure;
	fc_density_energy->x = (velocity->x)*de_p;
	fc_density_energy->y = (velocity->y)*de_p;
	fc_density_energy->z = (velocity->z)*de_p;
}

inline void compute_velocity(float density, float3* momentum, float3* velocity)
{
	velocity->x = (momentum->x) / density;
	velocity->y = (momentum->y) / density;
	velocity->z = (momentum->z) / density;
}

inline float compute_speed_sqd(float3* velocity)
{
	return (velocity->x)*(velocity->x) + (velocity->y)*(velocity->y) + (velocity->z)*(velocity->z);
}

inline float compute_pressure(float density, float density_energy, float speed_sqd)
{
	return ((float)GAMMA-(float)1.0f)*(density_energy - (float)0.5f*density*speed_sqd);
}

inline float compute_speed_of_sound(float density, float pressure)
{
	return sqrtf((float)GAMMA*pressure/density);
}



void compute_step_factor(int nelr, float* variables, float* areas, float* step_factors)
{
	int i;

 #pragma acc data present(variables[0:nelr*NVAR], areas[0:nelr]) deviceptr(step_factors)
 {

	//#pragma acc parallel num_gangs(nelr/block_length) num_workers(block_length/32) vector_length(32)
	#pragma acc kernels loop gang(nelr/block_length) vector(block_length) 
	for(i = 0; i < nelr; i++)
	{
		float density = variables[NVAR*i + VAR_DENSITY];
	
		float3 momentum;
		momentum.x = variables[NVAR*i + (VAR_MOMENTUM+0)];
		momentum.y = variables[NVAR*i + (VAR_MOMENTUM+1)];
		momentum.z = variables[NVAR*i + (VAR_MOMENTUM+2)];

		float density_energy = variables[NVAR*i + VAR_DENSITY_ENERGY];
		float3 velocity;
	
		//compute_velocity(density, &momentum, &velocity);
		velocity.x = (momentum.x) / density;
		velocity.y = (momentum.y) / density;
		velocity.z = (momentum.z) / density;

		float speed_sqd, pressure, speed_of_sound;
	//	float speed_sqd      = compute_speed_sqd(&velocity);
		speed_sqd = (velocity.x)*(velocity.x) + (velocity.y)*(velocity.y) + (velocity.z)*(velocity.z);
	//	float pressure  = compute_pressure(density, density_energy, speed_sqd);
		pressure =  ((float)GAMMA-(float)1.0f)*(density_energy - (float)0.5f*density*speed_sqd);
	//	float speed_of_sound = compute_speed_of_sound(density, pressure);
		speed_of_sound = sqrtf((float)GAMMA*pressure/density);

		// dt = float(0.5f) * std::sqrt(areas[i]) /  (||v|| + c).... but when we do time stepping, this later would need to be divided by the area, so we just do it all at once
		step_factors[i] = (float)0.5f / (sqrtf(areas[i]) * (sqrtf(speed_sqd) + speed_of_sound));
		
	}
	
 }

}


void compute_flux(int nelr, int* elements_surrounding_elements, float* normals, float* variables, float* fluxes)
{
	int i;
	const float smoothing_coefficient = 0.2f;
		
 #pragma acc data present(elements_surrounding_elements[0:nelr*NNB],\
 					 normals[0:nelr*NDIM*NNB],\
					 variables[0:nelr*NVAR],\
					 fluxes[0:nelr*NVAR])
 {
    #pragma acc kernels loop gang(nelr/block_length) vector(block_length)
	for(i = 0; i < nelr; i++)
	{
		int j, nb;
		float normal_len;
		float factor;

		float3 normal;

		float density_i = variables[NVAR*i + VAR_DENSITY];
		float3 momentum_i;
		momentum_i.x = variables[NVAR*i + (VAR_MOMENTUM+0)];
		momentum_i.y = variables[NVAR*i + (VAR_MOMENTUM+1)];
		momentum_i.z = variables[NVAR*i + (VAR_MOMENTUM+2)];

		float density_energy_i = variables[NVAR*i + VAR_DENSITY_ENERGY];

		float3 velocity_i;
		//compute_velocity(density_i, &momentum_i, &velocity_i);
		velocity_i.x = (momentum_i.x) / density_i;
		velocity_i.y = (momentum_i.y) / density_i;
		velocity_i.z = (momentum_i.z) / density_i;
		
		float speed_sqd_i                          = compute_speed_sqd(&velocity_i);
		float speed_i                              = sqrtf(speed_sqd_i);
		float pressure_i                           = compute_pressure(density_i, density_energy_i, speed_sqd_i);
		float speed_of_sound_i                     = compute_speed_of_sound(density_i, pressure_i);
	
		float3 flux_contribution_i_momentum_x, flux_contribution_i_momentum_y, flux_contribution_i_momentum_z;
		float3 flux_contribution_i_density_energy;

		//compute_flux_contribution(density_i, &momentum_i, density_energy_i, pressure_i, &velocity_i, &flux_contribution_i_momentum_x, &flux_contribution_i_momentum_y, &flux_contribution_i_momentum_z, &flux_contribution_i_density_energy);
		flux_contribution_i_momentum_x.x = (velocity_i.x)*(momentum_i.x) + pressure_i;
		flux_contribution_i_momentum_x.y = (velocity_i.x)*(momentum_i.y);
		flux_contribution_i_momentum_x.z = (velocity_i.x)*(momentum_i.z);

		flux_contribution_i_momentum_y.x = flux_contribution_i_momentum_x.y;
		flux_contribution_i_momentum_y.y = (velocity_i.y)*(momentum_i.y) + pressure_i;
		flux_contribution_i_momentum_y.z = (velocity_i.y)*(momentum_i.z);

		flux_contribution_i_momentum_z.x = flux_contribution_i_momentum_x.z;
		flux_contribution_i_momentum_z.y = flux_contribution_i_momentum_y.z;
		flux_contribution_i_momentum_z.z = (velocity_i.z)*(momentum_i.z) + pressure_i;

		float de_p = density_energy_i+pressure_i;
		flux_contribution_i_density_energy.x = (velocity_i.x)*de_p;
		flux_contribution_i_density_energy.y = (velocity_i.y)*de_p;
		flux_contribution_i_density_energy.z = (velocity_i.z)*de_p;

		float flux_i_density = 0.0f;
		float3 flux_i_momentum;
		flux_i_momentum.x = 0.0f;
		flux_i_momentum.y = 0.0f;
		flux_i_momentum.z = 0.0f;
		float flux_i_density_energy = 0.0f;


		float3 velocity_nb;
		float3 momentum_nb;
		float density_nb, density_energy_nb;
		float3 flux_contribution_nb_momentum_x, flux_contribution_nb_momentum_y, flux_contribution_nb_momentum_z;
		float3 flux_contribution_nb_density_energy;
		float speed_sqd_nb, speed_of_sound_nb, pressure_nb;
	
		for(j = 0; j < NNB; j++)
		{
			nb = elements_surrounding_elements[i*NNB + j];
			normal.x = normals[(i*NNB + j)*NDIM + 0];
			normal.y = normals[(i*NNB + j)*NDIM + 1];
			normal.z = normals[(i*NNB + j)*NDIM + 2];
			normal_len = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);

			if(nb >= 0) 	// a legitimate neighbor
			{
				density_nb =        variables[nb*NVAR + VAR_DENSITY];
				momentum_nb.x =     variables[nb*NVAR + (VAR_MOMENTUM+0)];
				momentum_nb.y =     variables[nb*NVAR + (VAR_MOMENTUM+1)];
				momentum_nb.z =     variables[nb*NVAR + (VAR_MOMENTUM+2)];
				density_energy_nb = variables[nb*NVAR + VAR_DENSITY_ENERGY];
													compute_velocity(density_nb, &momentum_nb, &velocity_nb);
				speed_sqd_nb                      = compute_speed_sqd(&velocity_nb);
				pressure_nb                       = compute_pressure(density_nb, density_energy_nb, speed_sqd_nb);
				speed_of_sound_nb                 = compute_speed_of_sound(density_nb, pressure_nb);
										//			compute_flux_contribution(density_nb, &momentum_nb, density_energy_nb, pressure_nb, &velocity_nb, &flux_contribution_nb_momentum_x, &flux_contribution_nb_momentum_y, &flux_contribution_nb_momentum_z, &flux_contribution_nb_density_energy);
				flux_contribution_nb_momentum_x.x = (velocity_nb.x)*(momentum_nb.x) + pressure_nb;
				flux_contribution_nb_momentum_x.y = (velocity_nb.x)*(momentum_nb.y);
				flux_contribution_nb_momentum_x.z = (velocity_nb.x)*(momentum_nb.z);

				flux_contribution_nb_momentum_y.x = flux_contribution_nb_momentum_x.y;
				flux_contribution_nb_momentum_y.y = (velocity_nb.y)*(momentum_nb.y) + pressure_nb;
				flux_contribution_nb_momentum_y.z = (velocity_nb.y)*(momentum_nb.z);

				flux_contribution_nb_momentum_z.x = flux_contribution_nb_momentum_x.z;
				flux_contribution_nb_momentum_z.y = flux_contribution_nb_momentum_y.z;
				flux_contribution_nb_momentum_z.z = (velocity_nb.z)*(momentum_nb.z) + pressure_nb;

				de_p = density_energy_nb+pressure_nb;
				flux_contribution_nb_density_energy.x = (velocity_nb.x)*de_p;
				flux_contribution_nb_density_energy.y = (velocity_nb.y)*de_p;
				flux_contribution_nb_density_energy.z = (velocity_nb.z)*de_p;

				// artificial viscosity
				factor = -normal_len*smoothing_coefficient*(float)0.5f*(speed_i + sqrtf(speed_sqd_nb) + speed_of_sound_i + speed_of_sound_nb);
				flux_i_density += factor*(density_i-density_nb);
				flux_i_density_energy += factor*(density_energy_i-density_energy_nb);
				flux_i_momentum.x += factor*(momentum_i.x-momentum_nb.x);
				flux_i_momentum.y += factor*(momentum_i.y-momentum_nb.y);
				flux_i_momentum.z += factor*(momentum_i.z-momentum_nb.z);

				// accumulate cell-centered fluxes
				factor = (float)0.5f*normal.x;
				flux_i_density += factor*(momentum_nb.x+momentum_i.x);
				flux_i_density_energy += factor*(flux_contribution_nb_density_energy.x+flux_contribution_i_density_energy.x);
				flux_i_momentum.x += factor*(flux_contribution_nb_momentum_x.x+flux_contribution_i_momentum_x.x);
				flux_i_momentum.y += factor*(flux_contribution_nb_momentum_y.x+flux_contribution_i_momentum_y.x);
				flux_i_momentum.z += factor*(flux_contribution_nb_momentum_z.x+flux_contribution_i_momentum_z.x);

				factor = (float)0.5f*normal.y;
				flux_i_density += factor*(momentum_nb.y+momentum_i.y);
				flux_i_density_energy += factor*(flux_contribution_nb_density_energy.y+flux_contribution_i_density_energy.y);
				flux_i_momentum.x += factor*(flux_contribution_nb_momentum_x.y+flux_contribution_i_momentum_x.y);
				flux_i_momentum.y += factor*(flux_contribution_nb_momentum_y.y+flux_contribution_i_momentum_y.y);
				flux_i_momentum.z += factor*(flux_contribution_nb_momentum_z.y+flux_contribution_i_momentum_z.y);

				factor = (float)0.5f*normal.z;
				flux_i_density += factor*(momentum_nb.z+momentum_i.z);
				flux_i_density_energy += factor*(flux_contribution_nb_density_energy.z+flux_contribution_i_density_energy.z);
				flux_i_momentum.x += factor*(flux_contribution_nb_momentum_x.z+flux_contribution_i_momentum_x.z);
				flux_i_momentum.y += factor*(flux_contribution_nb_momentum_y.z+flux_contribution_i_momentum_y.z);
				flux_i_momentum.z += factor*(flux_contribution_nb_momentum_z.z+flux_contribution_i_momentum_z.z);
			}
			else if(nb == -1)	// a wing boundary
			{
				flux_i_momentum.x += normal.x*pressure_i;
				flux_i_momentum.y += normal.y*pressure_i;
				flux_i_momentum.z += normal.z*pressure_i;
			}
			else if(nb == -2) // a far field boundary
			{
				factor = (float)0.5f*normal.x;
				flux_i_density += factor*(ff_variable[VAR_MOMENTUM+0]+momentum_i.x);
				flux_i_density_energy += factor*(ff_flux_contribution_density_energy.x+flux_contribution_i_density_energy.x);
				flux_i_momentum.x += factor*(ff_flux_contribution_momentum_x.x + flux_contribution_i_momentum_x.x);
				flux_i_momentum.y += factor*(ff_flux_contribution_momentum_y.x + flux_contribution_i_momentum_y.x);
				flux_i_momentum.z += factor*(ff_flux_contribution_momentum_z.x + flux_contribution_i_momentum_z.x);

				factor = (float)0.5f*normal.y;
				flux_i_density += factor*(ff_variable[VAR_MOMENTUM+1]+momentum_i.y);
				flux_i_density_energy += factor*(ff_flux_contribution_density_energy.y+flux_contribution_i_density_energy.y);
				flux_i_momentum.x += factor*(ff_flux_contribution_momentum_x.y + flux_contribution_i_momentum_x.y);
				flux_i_momentum.y += factor*(ff_flux_contribution_momentum_y.y + flux_contribution_i_momentum_y.y);
				flux_i_momentum.z += factor*(ff_flux_contribution_momentum_z.y + flux_contribution_i_momentum_z.y);

				factor = (float)0.5f*normal.z;
				flux_i_density += factor*(ff_variable[VAR_MOMENTUM+2]+momentum_i.z);
				flux_i_density_energy += factor*(ff_flux_contribution_density_energy.z+flux_contribution_i_density_energy.z);
				flux_i_momentum.x += factor*(ff_flux_contribution_momentum_x.z + flux_contribution_i_momentum_x.z);
				flux_i_momentum.y += factor*(ff_flux_contribution_momentum_y.z + flux_contribution_i_momentum_y.z);
				flux_i_momentum.z += factor*(ff_flux_contribution_momentum_z.z + flux_contribution_i_momentum_z.z);

			}
		}

		fluxes[i*NVAR + VAR_DENSITY] = flux_i_density;
		fluxes[i*NVAR + (VAR_MOMENTUM+0)] = flux_i_momentum.x;
		fluxes[i*NVAR + (VAR_MOMENTUM+1)] = flux_i_momentum.y;
		fluxes[i*NVAR + (VAR_MOMENTUM+2)] = flux_i_momentum.z;
		fluxes[i*NVAR + VAR_DENSITY_ENERGY] = flux_i_density_energy;

	}
  }/*end acc data*/
		
}

void time_step(int j, int nelr, float* old_variables, float* variables, float* step_factors, float* fluxes)
{
	int i;

#pragma acc data present(fluxes[0:nelr*NVAR]) deviceptr(step_factors, old_variables) 
 {
    #pragma acc kernels loop gang(nelr/block_length) vector(block_length)
	for(i = 0; i < nelr; i++)
	{
		float factor = step_factors[i]/(float)(RK+1-j);

		variables[NVAR*i + VAR_DENSITY] = old_variables[NVAR*i + VAR_DENSITY] + factor*fluxes[NVAR*i + VAR_DENSITY];
		variables[NVAR*i + VAR_DENSITY_ENERGY] = old_variables[NVAR*i + VAR_DENSITY_ENERGY] + factor*fluxes[NVAR*i + VAR_DENSITY_ENERGY];
		variables[NVAR*i + (VAR_MOMENTUM+0)] = old_variables[NVAR*i + (VAR_MOMENTUM+0)] + factor*fluxes[NVAR*i + (VAR_MOMENTUM+0)];
		variables[NVAR*i + (VAR_MOMENTUM+1)] = old_variables[NVAR*i + (VAR_MOMENTUM+1)] + factor*fluxes[NVAR*i + (VAR_MOMENTUM+1)];
		variables[NVAR*i + (VAR_MOMENTUM+2)] = old_variables[NVAR*i + (VAR_MOMENTUM+2)] + factor*fluxes[NVAR*i + (VAR_MOMENTUM+2)];
	}
 }
}
/*
 * Main function
 */
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("specify data file name\n");
		return 0;
	}
	const char* data_file_name = argv[1];

	// set far field conditions
		const float angle_of_attack = (float)(3.1415926535897931 / 180.0f) * (float)deg_angle_of_attack;

		ff_variable[VAR_DENSITY] = (float)1.4;

		float ff_pressure = (float)1.0f;
		float ff_speed_of_sound = sqrtf(GAMMA*ff_pressure / ff_variable[VAR_DENSITY]);
		float ff_speed = (float)ff_mach*ff_speed_of_sound;

		float3 ff_velocity;
		ff_velocity.x = ff_speed*(float)(cos((float)angle_of_attack));
		ff_velocity.y = ff_speed*(float)(sin((float)angle_of_attack));
		ff_velocity.z = 0.0f;

		ff_variable[VAR_MOMENTUM+0] = ff_variable[VAR_DENSITY] * ff_velocity.x;
		ff_variable[VAR_MOMENTUM+1] = ff_variable[VAR_DENSITY] * ff_velocity.y;
		ff_variable[VAR_MOMENTUM+2] = ff_variable[VAR_DENSITY] * ff_velocity.z;

		ff_variable[VAR_DENSITY_ENERGY] = ff_variable[VAR_DENSITY]*((float)0.5f*(ff_speed*ff_speed)) + (ff_pressure / (float)(GAMMA-1.0f));

		float3 ff_momentum;
		ff_momentum.x = *(ff_variable+VAR_MOMENTUM+0);
		ff_momentum.y = *(ff_variable+VAR_MOMENTUM+1);
		ff_momentum.z = *(ff_variable+VAR_MOMENTUM+2);


		compute_flux_contribution(ff_variable[VAR_DENSITY], &ff_momentum, ff_variable[VAR_DENSITY_ENERGY], ff_pressure, &ff_velocity, &ff_flux_contribution_momentum_x, &ff_flux_contribution_momentum_y, &ff_flux_contribution_momentum_z, &ff_flux_contribution_density_energy);

	int nel;
	int nelr;


	// read in domain geometry
	FILE *fin;
	char line[256];
	char *pch;
	float* areas;
	int* elements_surrounding_elements;
	float* normals;
	int i, j, k;

	fin = fopen(data_file_name, "rt");
	fgets(line, 256, fin);
	nel = atoi(line);
	nelr = block_length*((nel / block_length )+ MIN(1, nel % block_length));

	areas = (float*)malloc(nelr*sizeof(float));
	elements_surrounding_elements = (int*)malloc(nelr*NNB*sizeof(int));
	normals = (float*)malloc(NDIM*NNB*nelr*sizeof(float));

	// read in data
	for(i=0 ; i<nel; i++)
	{
		fgets(line, 256, fin);
		pch = strtok(line, " \t");
		areas[i] = atof(pch);
		

		for(j=0; j<NNB; j++)
		{
			pch = strtok(NULL, " \t");
			elements_surrounding_elements[i*NNB + j] = atoi(pch);

			if(elements_surrounding_elements[i*NNB+j] < 0) elements_surrounding_elements[i*NNB+j] = -1;
				elements_surrounding_elements[i*NNB + j]--; //it's coming in with Fortran numbering

			for(k=0; k<NDIM; k++)
			{
				pch = strtok(NULL, " \t");
				normals[(i*NNB + j)*NDIM + k] = atof(pch);

				normals[(i*NNB + j)*NDIM + k] = -normals[(i*NNB + j)*NDIM + k];
			}
		}
	
	}

		// fill in remaining data
		int last = nel-1;
		for(i = nel; i < nelr; i++)
		{
			areas[i] = areas[last];
			for(j = 0; j < NNB; j++)
			{
				// duplicate the last element
				elements_surrounding_elements[i*NNB + j] = elements_surrounding_elements[last*NNB + j];
				for(k = 0; k < NDIM; k++) normals[(i*NNB + j)*NDIM + k] = normals[(last*NNB + j)*NDIM + k];
			}
		}

	fclose(fin);

	// Create arrays and set initial conditions
	float* variables = (float*)malloc(nelr*NVAR*sizeof(float));
	initialize_variables(nelr, variables);

	float *old_variables;
	float *fluxes = (float*)malloc(nelr*NVAR*sizeof(float));
	float *step_factors;
	old_variables = (float*)acc_malloc(nelr*NVAR*sizeof(float));
	step_factors = (float*)acc_malloc(nelr*sizeof(float));
//	#pragma acc declare deviceptr(old_variables)

	// these need to be computed the first time in order to compute time step
	printf("Starting...\n");
	
	struct timeval tim;
	double start, end;
	gettimeofday(&tim, NULL);
	start = tim.tv_sec + (tim.tv_usec/1000000.0);
	
	// Begin iterations
#pragma acc data copyin(ff_variable[0:NVAR],\
						areas[0:nelr],\
						elements_surrounding_elements[0:nelr*NNB],\
						normals[0:nelr*NDIM*NNB],\
						fluxes[0:nelr*NVAR])\
				  deviceptr(old_variables, step_factors)\
				  copy(variables[0:nelr*NVAR])
{
	for(i = 0; i < iterations; i++)
	{
		//memcpy(old_variables, variables, nelr*NVAR*sizeof(float));
		#pragma acc kernels deviceptr(old_variables) 
		{
		#pragma acc loop gang(nelr*NVAR/960) vector(960)
		for(j=0; j<nelr*NVAR; j++)
			old_variables[j] = variables[j];
		}

		// for the first iteration we compute the time step
		compute_step_factor(nelr, variables, areas, step_factors);

		for(j = 0; j < RK; j++)
		{
			compute_flux(nelr, elements_surrounding_elements, normals, variables, fluxes);
			time_step(j, nelr, old_variables, variables, step_factors, fluxes);
		}
	}
}

	gettimeofday(&tim, NULL);
	end = tim.tv_sec + (tim.tv_usec/1000000.0);
	printf("%.4f seconds per iterations\n", (end-start)/iterations);
	printf("%.4f seconds, %d iterations\n", (end-start), iterations);


	printf("Saving solution...\n");
	dump(variables, nel, nelr);
	printf("Saved solution...\n");


	printf("Cleaning up...\n");

	free(areas);
	free(elements_surrounding_elements);
	free(normals);

	free(variables);
	acc_free(old_variables);
	acc_free(step_factors);
	free(fluxes);


	printf("Done...\n");

	return 0;
}
