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
        const StepWidth _dt;
        Matrix _matrix;
        Vector _k1, _k2, _k3, _k4;

      public:
        RungeKutta4(std::size_t dimension, const StepWidth& step_width)
            : _N(dimension), _dt(step_width), _k1(dimension), _k2(dimension), _k3(dimension),
              _k4(dimension)
        {
        }

        const StepWidth& step_width() const { return _dt; }
        std::size_t dimension() const { return _N; }
        const Matrix& matrix() const { return _matrix; }
        Matrix& matrix() { return _matrix; }

        void step(Vector& current)
        {
            _k1 = _matrix * current;
            _k2 = _matrix * (current + (_dt / 2) * _k1);
            _k3 = _matrix * (current + (_dt / 2) * _k2);
            _k4 = _matrix * (current + _dt * _k3);
            current += (_dt * Complex(0., 1.) / 6.) * (_k1 + 2. * _k2 + 2. * _k3 + _k4);
        }
    };

    namespace io
    {
        template <typename FileInfoHeader, typename Matrix, typename Vector>
        FileInfoHeader& add_file_info(FileInfoHeader& fih, const RungeKutta4<Matrix, Vector>& rk4)
        {
            fih.add_entry("Integrator");
            fih.add_entry("  type", "RungeKutta4 (fixed step)");
            fih.add_entry("  dimension", rk4.dimension());
            fih.add_entry("  step width", rk4.step_width());
            return fih;
        }
    }
}

#endif
