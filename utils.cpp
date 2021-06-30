#include "utils.hpp"



//char buf[4096]; // never know how much is needed

std::string workdir ; //= (std::string) cwd(buf, sizeof buf);
std::string out = workdir + "/out";

/*
  if (argc > 1) {
    cout  << "CWD: " << cwd(buf, sizeof buf) << endl;

    // Change working directory and test for success
    if (0 == cd(argv[1])) {
      cout << "CWD changed to: " << cwd(buf, sizeof buf) << endl;
    }

std::string ma_variable = "mon_repertoire_avec_variable";
std::string chemin = "c:\chemin\" +ma_variable;
// ou
std::string chemin = std::string("c:\chemin" ) + ma_variable;
mkdir(chemin.c_str());
*/
Out::Out (std::string file, std::string newdir, std::string topen)
{
    m_filename = "";
    if (newdir != "")
    {
        m_filename = outDir + newdir;
        //mkdir(m_filename.c_ctr());
        m_filename = m_filename + file + ".txt";
    }
    else
        m_filename = outDir + file + ".txt";
    //m_filetmp = ofstream fichier(m_filename);

}

Out::~Out (){}
//ofstream safeOpen (std::string path, std::string topen="w") {
//    //mkdir(path.c_str());
//    return ofstream fichier(path.c_str());
//}

void Out::write (std::string text) {
//    ofstream monFlux(m_filetmp.c_str(), ios::app);
//    monFlux << text.c_str() << endl;
}
void Out::writewt(std::string text){ // write with time
//    std::string time;
//    time_t curr_time;
//	tm * curr_tm;
//	char date_std::string[100];
//	char time_std::string[100];
//
//	time(&curr_time);
//	curr_tm = localtime(&curr_time);
//
//	strftime(date_std::string, 50, "%d/%m/%Y", curr_tm);
//	strftime(time_std::string, 50, " %T", curr_tm);
//
//    ofstream monFlux(m_filetmp.c_str(), ios::app);
//    monFlux << date_std::string << time_std::string << " ; " << text.c_str() << endl;
}
void Out::close(){}
void Out::copyFile(std::string srcdir, std::string dstdir, std::string filename){
//    std::string outsrcdir = Out.outDir + srcdir.c_str();
//    std::string outsrcfile = outsrcdir + m_filename.c_str();
//    std::string outdstdir = Out.outDir + dstdir.c_str();
//    mkdir(outdstdir);
//    ifstream  src(outsrcfile, ios::binary);
//    ofstream  dst(outdstdir, ios::binary);
//
//    dst << src.rdbuf();
}
DataDistCdfCcdf Out::dataDistCdfCcdf(std::vector<double> tableau, std::map<double, double> dist, std::map<double, double> cdf,
                                        std::map<double, double> ccdf){
    sort(tableau.begin(), tableau.end());
    double minv = tableau[0], maxv = tableau[tableau.size()-1];
    std::vector<double> valDist;
    std::map<double, double>::iterator im ;
    int i = 0, nb = 0, taille = tableau.size();

    for(i=0; i<taille; i++){
        ++dist[tableau[i]];
    }
    for(im=dist.begin() ; im!=dist.end() ; im++)
        valDist.push_back((double)(*im).first);

    sort(valDist.begin(), valDist.end());

    /* #CDF*/
    int len_valDist = valDist.size(), key;
    for(i=0; i<len_valDist; i++){
        key = valDist[i];
        nb += dist[key];
        cdf[key] = (100.0 * nb)/taille;
    }
    for(im=dist.begin(); im!=dist.end(); im++)
        (*im).second = (100.0 * (double)((*im).first))/taille;

    /* #CCDF*/
    for(i=0; i<len_valDist; i++){
        key = valDist[i];
        ccdf[key] = (100.0 * nb)/taille;
        nb -= dist[key];
    }
    DataDistCdfCcdf dataDistCdfCcdf;
//    dataDistCdfCcdf.cdf = cdf;
//    dataDistCdfCcdf.ccdf = ccdf;
//    dataDistCdfCcdf.dist = dist;
//    dataDistCdfCcdf.maxv = maxv;
//    dataDistCdfCcdf.minv = minv;
    return dataDistCdfCcdf;

}
DistAndCcdf Out::distAndCcdf(std::vector<double> tableau, std::map<double, double> dist, std::map<double, double> ccdf){
    std::sort(tableau.begin(), tableau.end());
    double minv = tableau[0], maxv = tableau[tableau.size()-1];
    std::vector<double> valDist;
    std::map<double, double>::iterator im;
    int i = 0, nb = 0, taille = tableau.size();

    for(i=0; i<taille; i++)
        ++dist[tableau[i]];
    for(im=dist.begin() ; im!=dist.end() ; im++)
        valDist.push_back((double)(*im).first);

    std::sort(valDist.begin(), valDist.end());
    int len_valDist = valDist.size(), key;
    nb = len_valDist;

    /* #CCDF*/
    for(i=0; i<len_valDist; i++){
        key = valDist[i];
        ccdf[key] = (100.0 * nb)/taille;
        nb -= dist[key];
    }
    DistAndCcdf distAndCcdf;
    distAndCcdf.ccdf = ccdf;
    distAndCcdf.dist = dist;
    distAndCcdf.maxv = maxv;
    distAndCcdf.minv = minv;
    return distAndCcdf;
}


double tfunction_identity(double weight_init, double Dt, double nt, double ta){
    return weight_init;
}

double tfunction_half_life(double weight_init, double Dt, double nt, double ta){
    if(nt > 0)
        return (weight_init * exp(-(log(2) * Dt * 1.0/nt)));
    return (weight_init * exp(-1 * Dt * log(2)));
}

double tfunction_logistic(double weight_init, double Dt, double nt, double ta=5){
    double K = (nt > 0) ? (ta*1.0)/nt : ta;
    return (1.0 - (1.0/ (1.0 + exp(-1.0 * K * (Dt - nt)))));
}

double tfunction_constant_decay(double weight_init, double Dt, double nt, double ta){
    double tw =  (nt > 0 && Dt <= nt) ? (1 - ((Dt * 1.0)/(nt * 1.0))) : 0.0;
    return tw;
}

double tfunction_short_term(double weight_init, double Dt, double nt, double ta){
    double tw = (Dt < nt) ? 1.0 : 0.0;
    return tw;
}

std::vector<std::vector<double>> read_data(std::string path) {
	std::ifstream in(path);
    std::vector<std::vector<double>> fields;
    std::vector<double>temp;
    if (in) {
        std::string line;
        while (getline(in, line)) {
            std::stringstream sep(line);
            std::string field;
            temp = std::vector<double>();
            fields.push_back(std::vector<double>());
            while (getline(sep, field, ';')) {
                temp.push_back(stod(field));
            }

            fields.back().push_back(temp[4]);
            fields.back().push_back(temp[0]);
            fields.back().push_back(temp[1]);
            fields.back().push_back(temp[3]);
            fields.back().push_back(temp[2]);
        }
    }
	return fields;
}

// Function to return the Jaccard distance
double jaccard_similarity_score(std::vector<double> v1, std::vector<double> v2){
    double size_v1 = v1.size(), size_v2 = v2.size();
    std::vector<int> intersect;
    // Find the intersection of the two sets
    for(int i=0; i<size_v1; i++)
        for(int j=0; j<size_v2; j++)
            if(v1[i]==v2[j])
                intersect.push_back(v1[i]);
    double size_in = intersect.size();
    // Calculate the Jaccard index using the formula
    double jaccardIndex = size_in / (size_v1 + size_v2 - size_in);
    return (1 - jaccardIndex);
}



