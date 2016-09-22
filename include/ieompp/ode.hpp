#ifndef IEOMPP_ODE_HPP_
#define IEOMPP_ODE_HPP_

#include <ieompp/types/number.hpp>
#include <ieompp/types/eigen.hpp>

namespace ieompp
{
    template <typename MatrixT, typename VectorT>
    class RungeKutta4
    {
        using Matrix = MatrixT;
        using Vector = VectorT;
        using StepWidth =
            typename types::real_type<typename types::eigen_matrix_traits<Matrix>::Scalar>::type;
        using Complex = typename types::eigen_matrix_traits<Vector>::Scalar;

        static_assert(types::is_complex<Complex>::value, "A complex Vector has to be used!");

      private:
        const std::size_t _N;
        Matrix _matrix;
        Vector _k1, _k2, _k3, _k4;

      public:
        RungeKutta4(std::size_t N) : _N(N), _k1(N), _k2(N), _k3(N), _k4(N) {}
        const Matrix& matrix() const { return _matrix; }
        Matrix& matrix() { return _matrix; }

        void step(Vector& current, const StepWidth dt)
        {
            _k1 = _matrix * current;
            _k2 = _matrix * (current + (dt / 2) * _k1);
            _k3 = _matrix * (current + (dt / 2) * _k2);
            _k4 = _matrix * (current + dt * _k3);
            current += (dt * Complex(0., 1.) / 6.) * (_k1 + 2. * _k2 + 2. * _k3 + _k4);
        }
    };
}

#endif
