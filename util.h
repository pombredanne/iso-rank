//
//  util.h
//  Sparse_Matrix
//
//  Created by Ali Hajimirza on 6/14/13.
//  Copyright (c) 2013 Ali Hajimirza. All rights reserved.
//

#ifndef _util_h
#define _util_h

#include "vertex.h"
#include "SparseMatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stack>


using namespace std;


/*
 * returns the sum of the values in array arr
 * @pram: pointer to an array of floats
 * @pram: size of the array
 */
template <typename DT>
DT sum_array(DT* arr,int arr_size){
    DT sum=0;
    
    for(int j=0;j<arr_size;j++){
        sum+=arr[j];
    }
    
    return sum;
}


/*
 * returns the minimum value in the array arr
 * @pram: pointer to array of floats
 * @pram: size of the array
 */
template <typename DT>
DT min(DT* arr, int arr_size){
    DT min_so_far=arr[0];
    
    for(int i=1;i<arr_size;i++){
        if(arr[i]<min_so_far){
            min_so_far=arr[i];
        }
    }
    
    return min_so_far;
}


/*
 * returns the maximum value in the array arr
 * @pram: pointer to array of floats
 * @pram: size of the array
 */
template <typename DT>
DT max(DT* arr, int arr_size){
    DT max_so_far=arr[0];
    
    for(int i=1;i<arr_size;i++){
        if(arr[i]>max_so_far){
            max_so_far=arr[i];
        }
    }
    
    return max_so_far;
}


/*
 * returns the mean of all the floats in an array
 * @pram: pointer to array of floats
 * @pram: size of array
 */
template <typename DT>
DT mean(DT *arr,int arr_size){
    return sum_array(arr,arr_size)/arr_size;
}

/*
 * returns the standard deviation of the values in arr
 * @pram: pointer to array of floats
 * @pram: size of the array
 */
template <typename DT>
DT std_dev(DT *arr, int arr_size){
    DT arr_mean=mean(arr,arr_size);
    
    DT variance=0;
    for(int index=0;index<arr_size;index++){
        DT hold =arr_mean-arr[index];
        variance+= hold*hold;
    }
    variance=variance/arr_size;
    return sqrtf(variance);
    
}

/*
 * takes an array of vertex objects vertices and an integer
 * signifying a component of the graph. The function returns
 * an integer array where vertices that don't belong to the
 * component are masked out.
 */
int* component_mask(vertex* vertices, int component, int number_nodes){
    int* comp_mask=(int *)malloc(sizeof(int) * number_nodes);
    
    for(int i=0;i<number_nodes;i++){
        comp_mask[i]=0;
    }
    
    for(int i=0;i<number_nodes;i++){
        vertex* curr_vertex= &vertices[i];
        if((*curr_vertex).get_low_link()==component){
            comp_mask[i]= 1;
        }
    }
    
    return comp_mask;
}



/*
 * multiples a vector by a scalar
 * @pram: pointer to the array returned
 * @pram: pointer to array used to represent the vector
 * @pram: the number by which the vector gets scaled
 */
template <typename DT>
DT* scalar_multiplication(DT *old_row,int size, DT scaling_factor){
    
    DT* ret_row=(DT *)malloc(sizeof(DT)*size);
    
#pragma omp for
    for(int i=0;i<size;i++){
        ret_row[i]=scaling_factor*old_row[i];
    }
    
    return ret_row;
}



#endif
