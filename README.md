# Linear
This is linear, C++ library for linear algebra.

## Before you begin
This library, though objective, is ment to be fast and efficient for multiple usecases.
To accelerate the computation GPU's multithreading was used alond side some other technical nuances.
This means this code's efficiency can vary dependingly on one's hardware.
Should it serve many purposes, it may not be fully optimized towards certain genres.

## Installation
### Get files
Choose from options:
- Download the latest release from [here](url) (currently unavailable),
- Reference library via repository url,
- Fork project and compile it yourself

### Add references
Once you get your files, drop library files in your project directory and add references to them.

## Guide
Now let's go through the contents of the library. This library contains tools for performing complex algebraic operations.
This includes both matrices and vectors for obvious reasons. All are members of the `lnr` namespace, each in their respective header file.

### Matrices
Matrix is a cornerstone for linear algebra, a universal computation unit. It is represented by a template `mat<M, N>` where `M` and `N`
are respectively height and width of the matrix.
We can create a matrix like this:
```C++
#include <mat.h>

lnr::mat<3,2> mat3x2 {1, 2, 3, 4, 5, 6};
```

As the size of a matrix is embedded in the type itself, this grants a type safety for every arithmetic operation.
Almost every validation is done during compile time, so mathematically incorrect operations just won't compile.
```C++
#include <mat.h>

using namespace lnr;

int main() {
  mat<3,2> mat3x2 {1, 2, 3, 4, 5, 6};
  mat<2> mat2x2 {1, 0, 0, 1};

  auto m1 = mat2x2 * mat3x2; // Won't compile!
  auto m2 = mat3x2 * mat2x2; // No error
}
```
> Given one size parameter matrix will be a square.

### Vectors
Vector is nothing but a one-dimensional matrix. It is a functional extension of `mat<N, 1>` which is simply a N-sized column matrix.
However, vector class provides some vector-specific operations along side those inherited from matrix.
A vector can be created like this:
```C++
#include <vec.h>

lnr::vec<3> vec3 {1, 2, 3};
```

Vectors support same operations as matrices, because they are ones indeed. That's why to multiply two vectors first one has to be transposed to match
matrix multiplication restriction and will produce 1x1 matrix, single cell containing dot product. Doing the opposite will return a 2x2 matrix.
```C++
#include <mat.h>

using namespace lnr;

int main() {
  vec<3> v1{1, 2, 3}, v2{-1, 0, 1};
  mat<1> = v1.t() * v2;
  mat<2> = v1 * v2.t();
}
```

### Rational
As an addition, library contains a `rational` numeric type which represents a rational number. It support all arithmetic operations without a loss.
This means it preserves also non-decimal ratios which gives a following result:
```C++
#include <rational.h>

using namespace lnr;

int main() {
  rational a = 1;
  a /= 3;
  a *= 3;
  // a = 1;
}
```
