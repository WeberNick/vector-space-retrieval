#include "args.hh"

void construct_arg_desc(argdesc_vt& x) {
    //typedef argdesc_t<Args, char> carg_t;
    //typedef argdesc_t<Args, int> iarg_t;
    //typedef argdesc_t<Args, uint> uarg_t;
    //typedef argdesc_t<Args, float> farg_t;
    //typedef argdesc_t<Args, double> darg_t;
    typedef argdesc_t<Args, bool> barg_t;
    typedef argdesc_t<Args, std::string> sarg_t;

    x.push_back(new barg_t("--help", false, &Args::help, "print this message"));
    x.push_back(new barg_t("--trace", false, &Args::trace, "sets the flag to activate tracing"));
    x.push_back(new barg_t("--measure", false, &Args::measure, "sets the flag to measure performance"));
    x.push_back(new barg_t("--print", false, &Args::print, "sets the flag to print messages to standard output (error messages, tracing, ...)"));
    x.push_back(new sarg_t("--path", "../data/", &Args::path, "path to the data directory. relative path from folder 'src' or absolute path possible"));
}

Args::Args() : _help(false), _trace(false), _measure(false), _print(false), _path("./data/") {}

Args::~Args() {}
