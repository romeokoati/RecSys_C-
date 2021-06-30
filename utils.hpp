#ifndef DEF_UTILS
#define DEF_UTILS


#include <bits/stdc++.h>            // For jaccard score computation.
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <map>


typedef struct DistAndCcdf {
    double minv;
    double maxv;
	std::map<double, double> dist;
	std::map<double, double> ccdf;
} DistAndCcdf;

typedef struct DataDistCdfCcdf {

    double minv;
    double maxv;
	std::map<double, double> dist;
	std::map<double, double> cdf;
	std::map<double, double> ccdf;
} DataDistCdfCcdf;


class Out {
    public:
    Out(std::string file, std::string newdir="", std::string topen="w");
    void write(std::string text);
    void writewt(std::string text);
    void close();
    void copyFile(std::string srcdir, std::string dstdir, std::string filename);
    DataDistCdfCcdf dataDistCdfCcdf(std::vector<double> tableau, std::map<double, double> dist, std::map<double, double> cdf,
                                        std::map<double, double> ccdf);
    DistAndCcdf distAndCcdf(std::vector<double> tableau, std::map<double, double> dist, std::map<double, double> ccdf);
    ~Out();

    //private:
    std::string m_filename;
    std::string m_filetmp;
    std::string outDir;
};


//std::ifstream safeOpen(std::string path, std::string topen="w");
//void mkdir(std::string path);

std::vector<std::vector<double>> read_data(std::string path);

double jaccard_similarity_score(std::vector<double> v1, std::vector<double> v2);


/**
###############################################################################
###############################################################################
#
# TIME-WEIGHT FUNCTIONS
#*/

double tfunction_identity(double weight_init, double Dt, double nt, double ta);

double tfunction_half_life(double weight_init, double Dt, double nt, double ta);

double tfunction_logistic(double weight_init, double Dt, double nt, double ta);

double tfunction_constant_decay(double weight_init, double Dt, double nt, double ta);

double tfunction_short_term(double weight_init, double Dt, double nt, double ta);

//time_weight_functions = {tfunction_identity, tfunction_half_life, tfunction_logistic, tfunction_constant_decay, tfunction_short_term};


#endif // DEF_UTILS
