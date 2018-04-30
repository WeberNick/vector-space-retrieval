#include "evaluation.hh"

const CB* Evaluation::_cb = nullptr;

void Evaluation::init(const CB& aControlBlock)
{
    if(!_cb)
    {
        _cb = &aControlBlock;
    }
}

Evaluation::Evaluation(const std::string& aQueryName, const IR_MODE aMode) : 
    _queryName(aQueryName), _mode(modeToString(aMode)), _measureInstance(nullptr), _time(0), _evalPath(std::string(_cb->evalPath()).append("eval/")), _evalStream() 
{
        fs::create_directory(_evalPath);
        _evalPath.append(std::string(_queryName).append(".txt"));
}


void Evaluation::start()
{
    const std::string lTraceMsg = std::string("Evaluating performance of mode '") + _mode + std::string("' for query '") + _queryName + std::string("'");
    TRACE(lTraceMsg);
    _measureInstance = new Measure;
    _measureInstance->start();
}

double Evaluation::stop()
{
    _measureInstance->stop();
    _time = _measureInstance->mTotalTime();
    delete _measureInstance;
    _measureInstance = nullptr;
    const std::string lTraceMsg = std::string("Time to process query '") + _queryName + std::string("' in mode '") + _mode + std::string("' : ") + std::to_string(_time);
    TRACE(lTraceMsg);
    _evalStream.open(_evalPath.c_str(), std::ofstream::out | std::ofstream::app);
    _evalStream << _mode << ' ' << std::setprecision(3) << std::fixed << std::endl;
    _evalStream.close();
    return _time;
}

void Evaluation::evalIR()
{
    //todo: compare query results with gold standard
}

void Evaluation::plot(const std::string& aQueryName, const std::string aPlotFormat)
{
    const std::string lPathToEval = std::string(_cb->evalPath() ).append("eval/");
    const std::string lPathToQueryFile = std::string(lPathToEval).append(aQueryName);
    const std::string lInputData = std::string(lPathToQueryFile).append(".txt");
    const std::string lOutputPath = std::string(lPathToQueryFile).append(aPlotFormat);
    if(fs::exists(lInputData))
    {
		Gnuplot gp;
		gp << "set terminal " << aPlotFormat << "\n";
		gp << "set output '" << lOutputPath << "'\n";
		gp << "set grid\n";
		gp << "set autoscale\n";
		gp << "set ylabel 'Time to process query' font ', 8'\n";
		gp << "set xtics font ',8'\n";
		gp << "set title 'Processing Time of Query " << aQueryName << " for different IR modes' font ',12'\n";
		gp << "set boxwidth 0.7 relative\n";
		gp << "set style data histograms\n";
		gp << "set style histogram cluster\n";
		gp << "set style fill solid 1.0 border lt -1\n";
		gp << "set auto x\n";
		gp << "set auto y\n";
		gp << "set key top left font ', 6'\n";

		const std::string command = "plot \n"; //todo, fix command with correct path to inpit file(s) and set labels
		gp << command;
    }
}

void Evaluation::reuse(const std::string& aQueryName, const IR_MODE aMode)
{
    _queryName = aQueryName;
    _mode = modeToString(aMode);
    _evalPath = std::string(_cb->evalPath()).append("eval/").append(std::string(_queryName).append(".txt"));
}
