#include <iostream>
#include <fstream>
using namespace std;

#include <ieompp/algebra/operator.hpp>
#include <ieompp/algebra/term.hpp>
#include <ieompp/discretization/linear.hpp>
#include <ieompp/io/file_header.hpp>
#include <ieompp/io/triplet_list.hpp>
#include <ieompp/models/hubbard_explicit/basis.hpp>
#include <ieompp/models/hubbard_explicit/matrix.hpp>
#include <ieompp/platform.hpp>
#include <ieompp/spdlog.hpp>
using namespace ieompp;
namespace spd = spdlog;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char** argv)
{
    const std::string program_name("hubbard_interaction_matrix_1d_real");

    po::options_description description("Calculate the interaction matrix for the 1D Hubbard model on a linear lattice in real space\n\nOptions");
    description.add_options()
        ("help", "print this help message")
        ("version", "print version information")
        ("N", po::value<size_t>()->default_value(16), "number of lattice sites")
        ("U", po::value<double>()->default_value(1.), "interaction strength")
        ("out", po::value<string>()->default_value("matrix_interaction_1d_real.txt"), "output file")
        ("log", po::value<string>()->default_value("matrix_interaction_1d_real.log"), "log file");

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
    const auto U        = vm["U"].as<double>();
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
    main_logger->info(" N   = {}", N);
    main_logger->info(" U   = {}", U);
    main_logger->info(" out = {}", out_path);
    main_logger->info(" log = {}", log_path);

    // setting up a lattice
    hubbard_logger->info("setting up a lattice");
    discretization::LinearDiscretization<double, size_t> lattice(N, 1.);
    log(hubbard_logger, get_description(lattice));

    using Operator = algebra::Operator<size_t, bool>;
    using Term     = algebra::Term<double, Operator>;

    // init operator basis
    hubbard_logger->info("Setting up operator basis");
    hubbard::real_space::Basis3Operator<Term> basis(lattice);
    log(hubbard_logger, get_description(basis));

    types::TripletList<double> elements(basis.size(), basis.size());
    hubbard_logger->info("Computing matrix elements in a triplet list");
    hubbard::real_space::init_interaction_matrix(elements, basis, U);
    hubbard_logger->info("  {} out of {} matrix elements are non-zero", elements.size(),
                         elements.rows() * elements.cols());
    hubbard_logger->info("Sorting matrix elements for col-major format");
    elements.sort();

    io_logger->info("Open output file \"{}\"", out_path);
    ofstream file(out_path.c_str());

    io::write_header(file, {get_instance_description(Platform()), get_description(elements)});

    io_logger->info("Write triplet list with {} elements", elements.size());
    file << '\n' << basis.size() << 'x' << basis.size() << '\n';
    io::write_triplet_list(file, elements);

    io_logger->info("Close output file \"{}\"", out_path);
    file.close();

    return 0;
}
