#include <AMReX_Array.H>
#include <vector>
#include <iostream>
#include <algorithm>

int main (int argc, char* argv[])
{
    const int n = 64;
    std::vector<double> vx(n*n*n, 1.0);
    std::vector<double> vy(n*n*n, 2.0);
    const double a = 3.0;

    amrex::Dim3 begin{10,11,12};
    amrex::Dim3 end{10+n,11+n,12+n};

    amrex::Array4<double> x(vx.data(), begin, end);
    amrex::Array4<double> y(vy.data(), begin, end);
    // x and y are like Fortran array with bounds (10:10+64-1,11:11+64-1,12:12+64-1)
    for         (int k = begin.z; k < end.z; ++k) {
        for     (int j = begin.y; j < end.y; ++j) {
            // Inform the compiler that the next loop has no assumed vector dependencies
            AMREX_PRAGMA_SIMD
            for (int i = begin.x; i < end.x; ++i) {
                y(i,j,k) += a*x(i,j,k);
            }
        }
    }

    // check the min and max of the results
    std::cout << *std::min_element(vy.begin(), vy.end()) << "\n";
    std::cout << *std::max_element(vy.begin(), vy.end()) << "\n";
}
