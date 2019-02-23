
#ifndef AMREX_ARRAY_H_
#define AMREX_ARRAY_H_

#include <cstdio>
#include <cassert>
#include <type_traits>
#include <AMReX_GpuQualifiers.H>
#include <AMReX_Extension.H>

namespace amrex {

    struct Dim3 { int x; int y; int z; };

    template <typename T>
    struct Array4
    {
        T* AMREX_RESTRICT p;
        long const jstride;
        long const kstride;
        long const nstride;
        Dim3 begin;
        Dim3 end;

        template <class = typename std::enable_if<std::is_const<T>::value> >
        AMREX_GPU_HOST_DEVICE
        Array4 (Array4<typename std::remove_const<T>::type> const& rhs)
            : p(rhs.p),
              jstride(rhs.jstride),
              kstride(rhs.kstride),
              nstride(rhs.nstride),
              begin(rhs.begin),
              end(rhs.end)
            {}

        AMREX_GPU_HOST_DEVICE
        Array4 (Array4<T> const& rhs)
            : p(rhs.p),
              jstride(rhs.jstride),
              kstride(rhs.kstride),
              nstride(rhs.nstride),
              begin(rhs.begin),
              end(rhs.end)
            {}

        AMREX_GPU_HOST_DEVICE
        Array4 (T* a_p, Dim3 const& a_begin, Dim3 const& a_end)
            : p(a_p),
              jstride(a_end.x-a_begin.x),
              kstride(jstride*(a_end.y-a_begin.y)),
              nstride(kstride*(a_end.z-a_begin.z)),
              begin(a_begin),
              end(a_end)
            {}

        AMREX_GPU_HOST_DEVICE AMREX_FORCE_INLINE
        T& operator() (int i, int j, int k) const {
#if defined(AMREX_DEBUG) || defined(AMREX_BOUND_CHECK)
            index_assert(i, j, k, __FILE__, __LINE__);
#endif
            return p[(i-begin.x)+(j-begin.y)*jstride+(k-begin.z)*kstride];
        }

        AMREX_GPU_HOST_DEVICE AMREX_FORCE_INLINE
        T& operator() (int i, int j, int k, int n) const {
#if defined(AMREX_DEBUG) || defined(AMREX_BOUND_CHECK)
            index_assert(i, j, k, __FILE__, __LINE__);
#endif
            return p[(i-begin.x)+(j-begin.y)*jstride+(k-begin.z)*kstride+n*nstride];
        }

#if defined(AMREX_DEBUG) || defined(AMREX_BOUND_CHECK)
        AMREX_GPU_HOST_DEVICE inline
        void index_assert (int i, int j, int k, const char* file, int line) const
        {
            if (i<begin.x || i>=end.x || j<begin.y || j>=end.y || k<begin.z || k>=end.z) {
                std::printf("File \"%s\", line %d: i = %d, j = %d, k = %d is out of bound (%d:%d,%d:%d,%d:%d)\n",
                            file, line, i, j, k, begin.x, end.x-1, begin.y, end.y-1, begin.z, end.z-1);
#if !defined(__APPLE__) || !defined(__CUDA_ARCH__)
                assert(0);
#endif
            }
        }
#endif
    };

}

#endif

