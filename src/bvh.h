#ifndef BHVNODE_H
#define BHVNODE_H

#include <thrust/sort.h>

#include "hitable/hitable.h"


struct BoxCompare {
    __device__ BoxCompare(int m): mode(m) {}
    __device__ bool operator()(Hitable* a, Hitable* b) const {
        // return true;

        AABB box_left, box_right;
        Hitable* ah = a;
        Hitable* bh = b;
        
        if(!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
                return false;
        }

        float val1, val2; 
        if(mode == 1) {
            val1 = box_left.min().x();
            val2 = box_right.min().x();
        }else if(mode == 2) {
            val1 = box_left.min().y();
            val2 = box_right.min().y();
        }else if(mode == 3) {
            val1 = box_left.min().z();
            val2 = box_right.min().z();
        }

        if(val1 - val2 < 0.0){
            return false;
        } else{
            return true;
        }
    }
    // mode: 1, x; 2, y; 3, z
    int mode;
};


class BVHNode: public Hitable{
public:
    __device__ BVHNode() {}
    __device__ BVHNode(Hitable **l, 
                       int n, 
                       float time0, 
                       float time1, 
                       curandState *state);

    __device__ virtual bool hit(const Ray& r, 
                                float t_min, 
                                float t_max, 
                                HitRecord& rec) const;

    __device__ virtual bool bounding_box(float t0, 
                                         float t1, 
                                         AABB& b) const;
    
    Hitable* left;
    Hitable* right;
    AABB box;
};


__device__ BVHNode::BVHNode(Hitable **l, 
                            int n, 
                            float time0, 
                            float time1,
                            curandState *state){
    int axis = int(3 * curand_uniform(state));
    if (axis == 0){
        thrust::sort(l, l + n, BoxCompare(1));
    }else if (axis == 1){
        thrust::sort(l, l + n, BoxCompare(2));
    }else{
        thrust::sort(l, l + n, BoxCompare(3));
    }

    if (n == 1) {
        left = right = l[0];
    }else if (n == 2) {
        left  = l[0];
        right = l[1];
    }else {
        left  = new BVHNode(      l,     n/2, time0, time1, state);
        right = new BVHNode(l + n/2, n - n/2, time0, time1, state);
    }
    
    AABB box_left, box_right;
    if (!left->bounding_box(time0, time1, box_left) || 
        !right->bounding_box(time0, time1, box_right)){
            return;
            // std::cerr << "no bounding box in BVHNode constructor \n";
        }
    box = surrounding_box(box_left, box_right);

}


__device__ bool BVHNode::bounding_box(float t0, 
                                      float t1, 
                                      AABB& b) const{
    b = box;
    return true;
}


__device__ bool BVHNode::hit(const Ray& r, 
                             float t_min, 
                             float t_max, 
                             HitRecord& rec) const{
    if (box.hit(r, t_min, t_max)) {
        HitRecord left_rec, right_rec;
        bool hit_left  = left->hit(r, t_min, t_max, left_rec);
        bool hit_right = right->hit(r, t_min, t_max, right_rec);
        if (hit_left && hit_right) {
            if (left_rec.t < right_rec.t) {
                rec = left_rec;
            }else{
                rec = right_rec;
            }
            return true;
        }else if (hit_left){
            rec = left_rec;
            return true;
        }else if (hit_right){
            rec = right_rec;
            return true;
        }else{
            return false;
        }
    }
    return false;
}

#endif  /* BHVNODE_H */