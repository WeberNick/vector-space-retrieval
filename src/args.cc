#include "args.hh"

void construct_arg_desc(argdesc_vt& x) {
    // using carg_t = argdesc_t<Args, char>;
    // using iarg_t = argdesc_t<Args, int>;
    using uarg_t = argdesc_t<Args, uint>;
    // using farg_t = argdesc_t<Args, float>;
    // using darg_t = argdesc_t<Args, double>;
    using barg_t = argdesc_t<Args, bool>;
    using sarg_t = argdesc_t<Args, std::string>;

    x.push_back(new barg_t("--help", false, &Args::help, "print this message"));
    x.push_back(new barg_t("--trace", false, &Args::trace, "sets the flag to activate tracing"));
    x.push_back(new barg_t("--measure", false, &Args::measure, "sets the flag to measure performance"));
    x.push_back(new barg_t("--print", false, &Args::print, "sets the flag to print messages to standard output (error messages, tracing, ...)"));

    x.push_back(new sarg_t("--path", "./data/", &Args::path, "path to the data directory. relative path from repo root directory or absolute path possible"));

    x.push_back(new uarg_t("--results", 10, &Args::results, "the maximum number of results a query shall return"));
    x.push_back(new uarg_t("--tiers", 0, &Args::tiers, "the number of tiers used for the tiered index. TODO: assign appropriate default value"));
    x.push_back(new uarg_t("--dimensions", 0, &Args::dimensions, "the number of dimensions used for the random projection. TODO: assign appropriate default value"));
}

Args::Args() : _help(false), _trace(false), _measure(false), _print(false), _path("./data/"), _results(10), _tiers(0), _dimensions(0) {}

Args::~Args() {}
