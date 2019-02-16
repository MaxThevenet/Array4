This is adapted from AMReX's Array4 class.  It provides non-owning
3D/4D view to a contiguous chunk of memory.  Non-owning means it's the
user's responsibility to allocate/deallocate memory.  View here
doesn't imply read-only.

It is usually constructed with

    template <typename T> Array4(T* p, Dim3 begin, Dim3 end);
    
Here, `p` is a pointer to CPU or CUDA memory.  `Dim3` is a struct with
three integer members, `x`, `y`, and `z`.  `Dim3 begin` and `end`
specify the inclusive beginning and exclusive end of the 3D index
bounds, respectively.  The class mimics Fortran array with bounds
`(begin.x:end.x-1,begin.y:end.y-1,begin.z:end.z-1)`.  We can access
the data via `operator()` (e.g., `a(i,j,k)`).  Note that `Array4` has
column-major order although it's a C++ class.  We usually want the
innermost loop to be over the first index.  The class also supports an
optional 4th dimension via `(i,j,k,n)`, where the last index `n` is
always zero based.

If `const *` is used to construct an `Array4`, the access will be
read-only.  If `*` without `const` is used for construction, the
access is both readable and writable.  The class also provide copy
constructors so that an object like `Array4<double*>` can be passed to
a function with argument `Array4<double const*> const&`.

When compiled with either `AMREX_DEBUG` or `AMREX_BOUND_CHECK`
defined, the class provides bound checking.

The class is CUDA-aware.  The constructors and `operator()` have both
host and device versions.

Also included in this repo is the `AMREX_PRAGMA_SIMD` macro that can
be used to inform the compiler that there are no assumed vector
dependencies, like `#pragma ivdep` supported by some compilers.
Another useful macro is `AMREX_FORCE_INLINE` for inlining.
