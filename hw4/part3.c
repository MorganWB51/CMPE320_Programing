/**
 * @author Morgan Williams-Burrell
 * This file contains part 3 of OS homework 4 on threads
 * This program will take in a number of points, as a command line arg, and calcualate the
 * average Euclidean distance between all points in addition to the centroid.
 * All information will be stored in the answers_t stuct
 * 
 * 
 * 
 * Note to professor: the autograder says the average for 100 points is 0.630, but mine says .638
 * This matchs with the result of my threadless version, so i decided leave it. If this turns out to
 * be incorrect a pointer in the right direction would be appreciated. 
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

 typedef struct {
    const point_t *points;
    int num_points;
    double *dists;
    int start_row;
    int end_row;
} thrd_dist_args_t;


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
 * The function that will run on a thread to comupte distances for a section of points
 */
void *compute_distance_thread(void *rawargs)
{
    thrd_dist_args_t *args = (thrd_dist_args_t *)rawargs;

    for (int r = args->start_row; r < args->end_row; r++)
    {
        for (int c = 0; c < args->num_points; c++) 
        {
            args->dists[r * args->num_points + c] = distance(&args->points[r], &args->points[c]);
        }
    }
}

/**
 * calculates the distance between all points
 * @param points - an array of points
 * @param num_points - the number of points in the array
 * @param num_threads - the number of thread to create
 * @return an array containing the Euclidean distances for all points
 */
double *compute_all_distances(const point_t *points, int num_points, int num_threads){
    double * distances = create_distances(num_points);
    pthread_t threadID[num_threads];
    thrd_dist_args_t work[num_threads];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for(int i = 0; i < num_threads; i++){
      work[i].points = points;
      work[i].num_points = num_points;
      work[i].dists = distances;
      work[i].start_row = i * (num_points/num_threads);
      work[i].end_row = work[i].start_row + (num_points/num_threads);
    }

    if(num_points%num_threads != 0) {
        work[num_threads-1].end_row += num_points%num_threads;
    }
    
    for(int i = 0; i < num_threads; i++) {
        //printf("doing %d - %d\n", work[i].start_row, work[i].end_row);
        pthread_create(&threadID[i], NULL, compute_distance_thread, &work[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threadID[i],NULL);
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
    point_t *centroid = calloc(1,sizeof(point_t));
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
answers_t *compute(int num_points, int num_threads){
    answers_t *answers = calloc(1,sizeof(answers_t));
    answers->num_points = num_points;
    answers->points = create_points(num_points);
    answers->distances = compute_all_distances(answers->points,num_points,num_threads);
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

    if(argc != 3) {
        printf("This program supports two(2) argument, Try again.\n");
        exit(0);
    }

    int num_points = strtod(argv[1],0);
    int num_threads = strtod(argv[2],0);
    
    struct timeval start;
    tick(&start);
    answers_t *answers = compute(num_points, num_threads);

    printf("Centroid: (%.3lf,%.3lf,%.3lf), Average: (%.3lf)\n", answers->centroid->x, answers->centroid->y, answers->centroid->z, answers->average_distance);
    printf("It took %ld usecs using %d threads\n", tock(start), num_threads);

    free(answers->centroid);
    free(answers->distances);
    free(answers->points);
    free(answers);


}