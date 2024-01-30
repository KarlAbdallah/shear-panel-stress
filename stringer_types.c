/*
 * AER8875 - Projet Integrateur IV en Aerospatial
 * Safety Margin Calculator Tool - An Implementation For Python
 * Created by K. Abdallah
 * Revision History:
 *  Version Date          Author         Comment
    1.0     02/25/2023    K. Abdallah    Creation
    2.0     02/26/2023    K. Abdallah    Refactoring + Header file creation
*/

/*
 * File:    stringer_types.c
 * -------------------------
 * Used as a dictionary for stringer shapes
*/


#include <stdlib.h>


stringer* new_stringer(int stringer_type, double in_skin_thickness, double in_skin_width,
    double in_bw, double in_tw, double in_bf, double in_tf, double in_bp, double in_tp) {

    // Allocate memory for stringer type
    stringer* st = (stringer*) malloc(sizeof(stringer));
    
    // Check if memory as been corrrectly allocated
    if (st == nullptr) return nullptr;

    // Load in correct stringer type
    /* z shaped stringer */
    if (stringer_type == 1) {
        // Update stringer
        st->type = 'z';
        st->num_of_elem = 4;
        update_stringer_z(st, in_skin_thickness, in_skin_width, in_bw, in_tw, in_bf, in_tf,
            in_bp, in_tp);
    }
    else {
        return nullptr;
    }

    return st;
}

void del_stringer(stringer* st) {
    // Check if stringer object is null
    if (st == nullptr) return;
    else {
        free(st->b);
        free(st->h);
        free(st->d);
        free(st->BC);
        free(st->bi);
        free(st->ti);
        free(st);
        return;
    }
}

void update_stringer_z(stringer* st, double in_skin_thickness, double in_skin_width,
    double in_bw, double in_tw, double in_bf, double in_tf, double in_bp, double in_tp) {
    // Check if stringer object is null
    if (st == nullptr) return;

    // Allocate memory for b,h,d pointers of stringer
    st->b = (double*) calloc(st->num_of_elem, sizeof(double));
    st->h = (double*) calloc(st->num_of_elem, sizeof(double));
    st->d = (double*) calloc(st->num_of_elem, sizeof(double));
    st->BC = (double*) calloc(st->num_of_elem-1, sizeof(double));
    st->bi = (double*) calloc(st->num_of_elem-1, sizeof(double));
    st->ti = (double*) calloc(st->num_of_elem-1, sizeof(double));

    // TODO: Check if allocated memory is null

    // Populating vector b
    st->b[0] = in_bf;
    st->b[1] = in_tw;
    st->b[2] = in_bf;
    st->b[3] = in_bp;

    // Populating vector h
    st->h[0] = in_tf;
    st->h[1] = in_bw;
    st->h[2] = in_tf;
    st->h[3] = in_tp;

    // Populating vector d
    for (int i = 0; i < st->num_of_elem; i++) {
        st->d[i] = st->h[i] / 2;
        for (int j = i + 1; j < st->num_of_elem; j++) {
            st->d[i] += st->h[j];
        }
        st->d[i] += in_skin_thickness;
    }
    
    // Calculate section properties
    double* A = stringer_area_vector(st->num_of_elem, st->b, st->h);
    double* I = stringer_inertia_vector(st->num_of_elem, st->b, st->h);
    double Ay = first_moment_of_area(st->num_of_elem, A, st->d);
    st->A_st = stringer_area(st->num_of_elem, A);
    st->I_st = stringer_inertia(st->num_of_elem, I, st->b, st->h, st->d);
    st->Y_bar = stringer_centroid(Ay, st->A_st);
    st->rho_st = stringer_gyration_radius(st->I_st, st->A_st);
    st->A_pa = panel_area(in_skin_thickness, in_skin_width);

    free(A);
    free(I);

    // Populating vector bi
    st->bi[0] = st->b[0];
    st->bi[1] = st->h[1];
    st->bi[2] = st->b[2];

    // Populating vector ti
    st->ti[0] = st->h[0];
    st->ti[1] = st->b[1];
    st->ti[2] = st->h[2];

    // Populating vector BC
    st->BC[0] = 1;
    st->BC[1] = 0;
    st->BC[2] = 1;

    return;
}