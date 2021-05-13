/**
 * @author Morgan Williams-Burrell
 * This file contains part 3 of OS homework 4 on threads
 * This program will take in a number of points, as a command line arg, and calcualate the
 * average Euclidean distance between all points in addition to the centroid.
 * All information will be stored in the answers_t stuct
 * This version is done without threads
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

typedef struct {
    double x,y,z;
} point_t;

typedef struct {
     int num_points;             // number of points
     point_t *points;            // an array of points in 3-space (x,y,z)
     double *distances;          // a num_points^2 array of the distances between points
     double average_distance;    // the average distance between all points
     point_t *centroid;          // the center of all the points
 } answers_t;

/**
 * assigns random values, (x,y,z), to points
 * @param points - an array of empty points
 */
 void init_point(point_t *points){
   points->x = drand48();
   points->y = drand48();
   points->z = drand48();
}

/**
 * Calls 'init_point' on all points in the array
 * @param points - an array of empty points
 * @param num_points - the number of points in the array
 */
void init_all_points(point_t *points, int num_points){
    for (int i = 0; i < num_points; i++){
        init_point(&points[i]);
    }
}

/**
 * creates an array of points and seeds the random number generator
 * @param num_points - the number of points to be created
 * @returns the array of points
 */
point_t *create_points(int num_points){
    point_t *points = (point_t *) calloc(num_points, sizeof(point_t));
    srand48(0);
    init_all_points(points, num_points);
    return points;
}

/**
 * creates a double array to hold all Euclidean distances
 * @param num_points - the number of points
 * @returns the array of distances
 */
double *create_distances(int num_points) {
    double *distances = calloc(pow(num_points,2), sizeof(double *));
    return distances;
}

/**
 * calculates the distance between two(2) points
 * @param a - a point
 * @param b - a point
 * @returns the array of distnaces
 */
double distance(const point_t *a, const point_t *b){
    double dx = a->x - b->x;
    double dy = a->y - b->y;
    double dz = a->z - b->z;
    double distance = sqrt( pow(dx,2) + pow(dy,2) + pow(dz,2) );
    return distance;
}

/**
 * calculates the distance between all points
 * @param points - an array of points
 * @param num_points - the number of points in the array
 * @return an array containing the Euclidean distances for all points
 */
double *compute_all_distances(const point_t *points, int num_points){
    double * distances = create_distances(num_points);
    for (int i = 0; i < num_points; i++){
        for(int j = 0; j < num_points;j++){
            distances[(i*num_points)+j] = distance(&points[i],&points[j]);
        }
    }
    return distances;
}

/**
 * calculates the average of all Euclidean distances
 * @param dists - an array of Euclidean distances
 * @param num_points - the number of points
 * @return the average Euclidean distance
 */
double compute_average(const double *dists, int num_points) {
    double sum, average = 0;
    for(int i = 0; i < pow(num_points,2);i++){
        sum += dists[i];
    }
    average = sum / pow(num_points,2);

    return average;
}

/**
 * finds the centroid on all points
 * @param points - an array of points
 * @param num_points - the number of points in the array
 * @returns a point that has the values of the centroid
 */
point_t *compute_centroid(const point_t *points, int num_points) {
    point_t *centroid = malloc(sizeof(point_t));
    double cx, cy, cz = 0;
    for (int i = 0; i < num_points; i++){
        cx += points[i].x;
        cy += points[i].y;
        cz += points[i].z;
    }

    centroid->x = cx / num_points;
    centroid->y = cy / num_points;
    centroid->z = cz / num_points;

    return centroid;
}

/**
 * 
 * @param num_points
 * @return an answer_t struct that hold all calculated data
 */
answers_t *compute(int num_points){
    answers_t *answers = malloc(sizeof(answers_t));
    answers->num_points = num_points;
    answers->points = create_points(num_points);
    answers->distances = compute_all_distances(answers->points,num_points);
    answers->average_distance = compute_average(answers->distances,num_points);
    answers->centroid = compute_centroid(answers->points,num_points);
    return answers;
}

/**
 * records the start time of the program
 */
void tick(struct timeval *start) {
    gettimeofday(start, NULL);
}

/**
 * records the end time of the program and calculates the total runtime
 * @param start - the time the program started running
 * @return the differnce between the start time and end time
 */
long tock(struct timeval start) {
    struct timeval end;
    gettimeofday(&end, NULL);
    unsigned long endTime = (end.tv_sec*1000000L)+end.tv_usec;
    unsigned long startTime = (start.tv_sec*1000000L)+start.tv_usec;
    return endTime - startTime;
}

int main(int argc, char **argv) {

    if(argc != 2) {
        printf("This program supports one(1) argument, Try again.\n");
        exit(0);
    }

    double num_points = strtod(argv[1],0);
    
    struct timeval start;
    tick(&start);

    answers_t *answers = compute(num_points);

    printf("Centroid: (%.3lf,%.3lf,%.3lf), Average: (%.3lf)\n", answers->centroid->x, answers->centroid->y, answers->centroid->z, answers->average_distance);
    printf("It took %ld usecs\n", tock(start));
}