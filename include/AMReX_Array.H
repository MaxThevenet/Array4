
#ifndef AMREX_ARRAY_H_
#define AMREX_ARRAY_H_

#include <type_traits>
#include <AMReX_GpuQualifiers.H>
#include <AMReX_Extension.H>

namespace amrex {

    struct Dim3 { int x; int y; int z; };

    template <typename T>
    struct Array4
    {
        T* AMREX_RESTRICT p;
        Dim3 begin;
        Dim3 end;

        template <class = typename std::enable_if<std::is_const<T>::value> >
        AMREX_GPU_HOST_DEVICE
        Array4 (Array4<typename std::remove_const<T>::type> const& rhs)
            : p(rhs.p), begin(rhs.begin), end(rhs.end)
            {}

        AMREX_GPU_HOST_DEVICE
        Array4 (Array4<T> const& rhs)
            : p(rhs.p), begin(rhs.begin), end(rhs.end)
            {}

        AMREX_GPU_HOST_DEVICE
        Array4 (T* a_p, Dim3 const& a_begin, Dim3 const& a_end)
            : p(a_p), begin(a_begin), end(a_end)
            {}

        AMREX_GPU_HOST_DEVICE AMREX_FORCE_INLINE
        T& operator() (int i, int j, int k) const {
#if defined(AMREX_DEBUG) || defined(AMREX_BOUND_CHECK)
            AMREX_ASSERT(i>=begin.x && i<end.x && j>=begin.y && j<end.y && k>=begin.z && k<end.z);
#endif
            return p[(i-begin.x+(j-begin.y)*(end.x-begin.x)+(k-begin.z)*(end.x-begin.x)*(end.y-begin.y))];
        }

        AMREX_GPU_HOST_DEVICE AMREX_FORCE_INLINE
        T& operator() (int i, int j, int k, int n) const {
#if defined(AMREX_DEBUG) || defined(AMREX_BOUND_CHECK)
            AMREX_ASSERT(i>=begin.x && i<end.x && j>=begin.y && j<end.y && k>=begin.z && k<end.z);
#endif
            return p[(i-begin.x+(j-begin.y)*(end.x-begin.x)+(k-begin.z)*(end.x-begin.x)*(end.y-begin.y))+static_cast<long>(n)*(((end.x-begin.x)*(end.y-begin.y))*(end.z-begin.z))];
        }
    };

}

#endif

