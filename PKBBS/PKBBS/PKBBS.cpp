#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random/random_device.hpp>
#include <boost/integer/common_factor.hpp>
#include <iostream>
#include <iomanip>
#include <omp.h>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <fstream>

using namespace boost::multiprecision;

cpp_int generateBlumIntegerFragment() {
    boost::random::random_device rd;
    boost::random::independent_bits_engine<boost::random::mt11213b, 256, cpp_int> generator(rd);

    boost::random::random_device testGenerator;

    bool flag = true;
    cpp_int returnVal;

    //#pragma omp parallel shared(flag)
    while(flag) {
        cpp_int val = generator();

        if (val % 4 == 3 && miller_rabin_test(val, 25, testGenerator) && miller_rabin_test((val - 1) / 2, 25, testGenerator)) {
            //#pragma omp critical
            {
                if (flag) {
                    returnVal = val;
                    flag = false;
                }
            }
        }
    }
    return returnVal;
}

std::vector<bool> BBSGenerator(cpp_int blumInt, cpp_int seed, size_t length) {
    std::vector<bool> result;
    result.reserve(length);

    cpp_int val = pow(seed, 2) % blumInt;

    for (size_t i = 0; i < length; ++i) {
        result.push_back(val % 2 ? true : false);
        val = pow(val, 2) % blumInt;
    }

    return result;
}

std::string convertBoolVecToStdStr(const std::vector<bool>& vec) {
    std::string res(vec.size(), '0');

    #pragma omp parallel for schedule(dynamic) shared(res)
    for (int i = 0; i < vec.size(); ++i) {
        res[i] = vec[i] + '0';
    }

    return res;
}

cpp_int randomizeSeedForBBS(const cpp_int blumInt) {
    boost::random::random_device rd;
    boost::random::independent_bits_engine<boost::random::mt11213b, 256, cpp_int> generator(rd);

    cpp_int returnVal;

    do {
        returnVal = generator();
    } while (boost::math::gcd(blumInt, returnVal) - 1);

    return returnVal;
}

int main(int argc, char* argv[])
{
    size_t seqLength = 20000;
    std::string outFilename = "out.txt";
    std::fstream fs;
    std::string result;

    boost::program_options::options_description desc("Paweł Perechuda; 145199\nAllowed options");
    desc.add_options()
        ("help,h", "print help message")
        ("length,l", boost::program_options::value<size_t>(), "length of the produced BBS sequence")
        ("output,o", boost::program_options::value<std::string>(), "output filename")
        ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    if(vm.count("length"))
        seqLength = vm["length"].as<size_t>();
    if(vm.count("output"))
        outFilename = vm["output"].as<std::string>();

    cpp_int p = generateBlumIntegerFragment();
    cpp_int q = generateBlumIntegerFragment();
    cpp_int seed = randomizeSeedForBBS(p * q);


    result = convertBoolVecToStdStr(BBSGenerator(p * q, seed, seqLength));

    fs.open(outFilename, std::ios_base::out);

    fs << "p: " << p << std::endl
        << "q: " << q << std::endl
        << "seed: " << seed << std::endl
        << "======" << std::endl
        << result;
    fs.close();

    //std::cout << "seqLength: " << seqLength << std::endl
    //    << "outFilename: " << outFilename << std::endl;

    //#pragma omp parallel for
    //for (int i = 0; i < 1000; ++i) {
    //    std::cout << "Bloom integer fragment found: " << std::hex << std::showbase << generateBlumIntegerFragment() << std::endl;
    //}

    //std::cout << convertBoolVecToStdStr(BBSGenerator(generateBlumIntegerFragment() * generateBlumIntegerFragment(), 123, 20000));
    return 0;
}
