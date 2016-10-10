#include <iostream>
using namespace std;

#include <ieompp/models/hubbard_explicit/basis.hpp>
#include <ieompp/types/eigen/multiply_assign.hpp>

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/models/hubbard_explicit/matrix_elements.hpp>
#include <ieompp/ode/rk4.hpp>
#include <ieompp/platform.hpp>
#include <ieompp/spdlog.hpp>
#include <ieompp/types/eigen/function_matrix_interop.hpp>
#include <ieompp/types/function_matrix.hpp>
using namespace ieompp;
namespace spd = spdlog;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    const std::string program_name("hubbard_1d_real");

    po::options_description description("Calculate the dynamics of the 1d Hubbard model\n\nOptions");
    description.add_options()
        ("help", "print this help message")
        ("version", "print version information")
        ("N", po::value<size_t>()->default_value(16), "number of lattice sites")
        ("J", po::value<double>()->default_value(1.), "hopping prefactor")
        ("U", po::value<double>()->default_value(1.), "interaction strength")
        ("dt", po::value<double>()->default_value(0.01), "step width for integrator")
        ("t_end", po::value<double>()->default_value(10.), "end time for integration")
        ("out", po::value<string>()->default_value("kinetic_interaction_1d_real.txt"), "output file")
        ("log", po::value<string>()->default_value("kinetic_interaction_1d_real.log"), "log file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, description), vm);
    po::notify(vm);

    if(vm.count("help")) {
        cout << description << '\n';
        return 1;
    }
    if(vm.count("version")) {
        cout << program_name << "\n\n";
        cout << get_type_description<Platform>() << '\n';
        return 1;
    }

    const auto N        = vm["N"].as<size_t>();
    const auto J        = vm["J"].as<double>();
    const auto U        = vm["U"].as<double>();
    const auto dt       = vm["dt"].as<double>();
    const auto t_end    = vm["t_end"].as<double>();
    const auto out_path = vm["out"].as<string>();
    const auto log_path = vm["log"].as<string>();

    // setting up logging facilities
    vector<spd::sink_ptr> logging_sinks;
    logging_sinks.push_back(make_shared<spd::sinks::stderr_sink_st>());
    logging_sinks.push_back(make_shared<spd::sinks::simple_file_sink_st>(log_path, true));
    auto main_logger = std::make_shared<spd::logger>("main", logging_sinks.begin(), logging_sinks.end());
    auto io_logger = std::make_shared<spd::logger>("io", logging_sinks.begin(), logging_sinks.end());
    auto hubbard_logger = std::make_shared<spd::logger>("hubbard", logging_sinks.begin(), logging_sinks.end());

    main_logger->info("CLI options:");
    main_logger->info(" N     = {}", N);
    main_logger->info(" J     = {}", J);
    main_logger->info(" U     = {}", U);
    main_logger->info(" dt    = {}", dt);
    main_logger->info(" t_end = {}", t_end);
    main_logger->info(" out   = {}", out_path);
    main_logger->info(" log   = {}", log_path);

    // setting up a lattice
    hubbard_logger->info("setting up a lattice");
    discretization::LinearDiscretization<double, uint64_t> lattice(N, 1.);
    log(hubbard_logger, get_description(lattice));

    using Operator = algebra::Operator<uint64_t, bool>;
    using Term     = algebra::Term<double, Operator>;

    // init operator basis
    hubbard_logger->info("Setting up operator basis");
    hubbard::real_space::Basis3Operator<Term> basis(lattice);
    log(hubbard_logger, get_description(basis));

    using namespace std::complex_literals;
    auto matrix_function = [&basis, &lattice, &J, &U](uint64_t i, uint64_t j) {
        return (matrix_elements_kinetic(i, j, basis, lattice, J)
                + matrix_elements_interaction(i, j, basis, U))
               * 1.i;
    };
    types::FunctionMatrix<std::complex<double>, uint64_t> M(basis.size(), basis.size(),
                                                            matrix_function);
    static_assert(std::is_same<decltype(M)::Index, uint64_t>::value, "bla");

    Eigen::VectorXcd h(basis.size());
    h.setZero();
    h(0) = 1.;

    ode::RK4<double> integrator(basis.size(), dt);
    double t = 0.;
    while(t < t_end) {
        main_logger->info("Performing step at t={}", t);
        integrator.step(M, h);
        main_logger->info("Complete step {} -> {}", t, t + integrator.step_size());
        t += integrator.step_size();
        main_logger->info("\th_0({}) = {} + i {}", t, h(0).real(), h(0).imag());
        main_logger->info("\tstep_size = {}", integrator.step_size());
    }
}
