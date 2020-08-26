/*
 * Experiment.cpp
 *
 *  Created on: 16 maj 2016
 *      Author: oramus
 */

#include<stdlib.h>
#include<iostream>
#include <omp.h>
#include <string.h>

#include "Experiment.h"
#include "Distribution.h"

#define DEBUG_ON_

using namespace std;


bool *used;
long *local_histogram;
long local_values;
double local_sum;

struct drand48_data drand_seed;



#pragma omp threadprivate(used, local_histogram, local_values, local_sum, drand_seed)

Experiment::Experiment(int balls, int drawsNumber) {
	this->balls = balls;
	this->drawsNumber = drawsNumber;

	hmax = 0; // wyznaczamy maksymalna sume
	hmin = 0; // i najmniejsza sume

	for (int i = 0; i < drawsNumber; i++) {
		hmax += balls - i;
		hmin += i + 1; // 1 + 2 + 3 + ... liczba losowan
	}

	cout << "Histogram min: " << hmin << " max: " << hmax << endl;

	histogram = new long[hmax + 1];
    #pragma omp parallel
    {
	    used = new bool[balls];
        srand48_r((long)used, &drand_seed);
        local_histogram = new long[hmax + 1];

        memset(local_histogram, 0, sizeof(long)*(hmax+1));
        memset(used, 0, sizeof(bool) * balls);

        local_sum = 0;
        local_values = 0.0;
    }

	for (long i = 0; i < hmax + 1; i++)
		histogram[i] = 0;
}

void Experiment::clearUsed() {
#ifdef DEBUG_ON
    
    cout << "address " << used << endl;
#endif
	for (int i = 0; i < balls; i++)
		used[i] = false;
}

long Experiment::singleExperimentResult() {
	long sum = 0;
	int ball;
	double p;
    double drand_return = 0.0;

	clearUsed();

	for (int i = 0; i < drawsNumber;) {
        drand48_r(&drand_seed, &drand_return);
		ball = 1 + (int) (((double) balls * drand_return) ); // rand losuje od 0 do RAND_MAX wlacznie

#ifdef DEBUG_ON
			cout << "ball :" << ball << endl;
#endif
		if (used[ball - 1])
			continue;

		p = Distribution::getProbability(i + 1, ball); // pobieramy prawdopodobienstwo wylosowania tej kuli
        drand48_r(&drand_seed, &drand_return);
#ifdef DEBUG_ON
			cout << "prob " << drand_return << endl;
#endif

		if (drand_return < p) // akceptacja wyboru kuli z zadanym prawdopodobienstwem
		{
#ifdef DEBUG_ON
			cout << "Dodano kule o numerze " << ball << endl;
#endif
			used[ball - 1] = true;
			sum += ball; // kule maja numery od 1 do balls wlacznie
			i++;
		}
	}

///	cout << "Suma = " << sum << endl;

	return sum;
}

Result * Experiment::calc(long experiments) {

    long l = 0;

	long maxID = 0;
	long minID = 0;
	long maxN = 0;
	long minN = experiments;
	double sum = 0.0;
	long values = 0;

    long idx = 0;


    memset(histogram, 0, sizeof(long) * (hmax + 1));
    #pragma omp parallel firstprivate(l, idx)
    {
	    for (l = 0; l < experiments; l++) {

		    local_histogram[singleExperimentResult()]++;

        }

	    for (idx = hmin; idx <= hmax; idx++) {
		    local_sum += idx * local_histogram[idx];
		    local_values += local_histogram[idx];
	   }

        #pragma omp atomic
            sum += local_sum;
        #pragma omp atomic
            values += local_values;

        local_sum = 0;
        local_values = 0;

        #pragma omp barrier

        #pragma omp critical
        {

            for( idx = hmin; idx <= hmax; idx++) {

                histogram[idx] += local_histogram[idx];
            }

        }

        #pragma omp barrier

        #pragma omp master
        {

            for (idx = hmin; idx <= hmax; idx++) {

                if(maxN < histogram[idx]) {
                    maxN = histogram[idx];
                    maxID = idx;
                }

            }
        }

    }

// indeks to wartosc, histogram -> liczba wystapien
	return new Result(maxID, maxN, sum / values, values);
}

Experiment::~Experiment() {
	delete[] histogram;
    #pragma omp parallel
    {
        delete[] local_histogram;
	    delete[] used;
    }
}

