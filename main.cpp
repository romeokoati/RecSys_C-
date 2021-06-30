#include "Expgen.hpp"
#include "utils.hpp"
#include<algorithm>

using namespace std;


bool sortcol( const vector<double>& v1,const vector<double>& v2 )
{
    return v1[0] < v2[0];
}


int main()
{

    string input_dataset_file = "ciao-data.txt";
    string input_trust_network = "ciao-trust-network.txt";

    //# ---->  evaluation metrics

    //# you can add any metric in the form prec@N (Precision), recall@N, map@N, hr@N (Hit Ratio)
    //# with N in {1, 2, 3, 5, 10, 15, 20, 30, 40, 50, 100}
    vector<string> eval_metric = {"prec@10", "recall@10", "map@10", "hr@10"};

    //# ---->  parameters of experiment

    //# the time is divided in time slices of equal length
    int number_of_time_slices = 8; //# this value >= 2

    List_param* rs1_param = new List_param("BIP", "CIU", "EDF", "IT", 0, 0.0, 180, 0, 0.9, 0.9);
    List_param* rs2_param = new List_param("STG", "CIU", "LDF", "IT", 540, 0.1, 365, 10, 0.7, 0.9);


    //    rs1_param->graph = "BIP";
    //    rs1_param->content = "CIU";
    //    rs1_param->time = "EDF";
    //    rs1_param->trust = "IT";
    //    rs1_param->delta = 0;
    //    rs1_param->beta = 0;
    //    rs1_param->t0 = 180;
    //    rs1_param->k = 0;
    //    rs1_param->gamma = 0.9;
    //    rs1_param->alpha = 0.9;
    //
    //    rs2_param->graph = "STG";
    //    rs2_param->content = "CIU";
    //    rs2_param->time = "LDF";
    //    rs2_param->trust = "IT";
    //    rs2_param->delta = 540;
    //    rs2_param->beta = 0.1;
    //    rs2_param->t0 = 365;
    //    rs2_param->k = 10;
    //    rs2_param->gamma = 0.7;
    //    rs2_param->alpha = 0.9;

    vector<List_param*> rs_list; //# list of configurated recsys
    rs_list.push_back(rs1_param);
    rs_list.push_back(rs2_param);

    //# ---->  Read data

    vector<vector<double>> linkstream(read_data(input_dataset_file));
    vector<vector<double>> trust_network(read_data(input_trust_network));


    sort(linkstream.begin(), linkstream.end(),sortcol);



    //linkstream = read_data(input_dataset_file);
    //trust_network = read_data(input_trust_network);


    main_run(rs_list, linkstream, trust_network, eval_metric, number_of_time_slices);









    //	Graph* g = new Graph(Graph::DefaultNodes::SINGLE, 20, 40);
    //	g->display();
    //
    //	g->add_node("F", "u");
    //	g->add_edge(g->getNode("F"), g->getNode("A"), 100, 1, 2);
    //	g->add_edge(g->getNode("A"), g->getNode("F"), 20, 1, 3);
    //	g->display();
    //
    //	SparseMatrix m3(10, 10);
    //    m3.set_(0, 0, 0.1);
    //    m3.set_(0, 4, 0.4);
    //    m3.set_(0, 8, 0.9);
    //    m3.set_(2, 0, 2.0);
    //    m3.set_(2, 1, 2.1);
    //    m3.set_(2, 4, 2.4);
    //    m3.set_(2, 8, 2.9);
    //    m3.set_(3, 6, 3.6);
    //    m3.set_(5, 2, 7.9);
    //    m3.set_(6, 6, 9.9);
    //    m3.set_(7, 0, 7.7);
    //    m3.set_(8, 3, 4.4);
    //    m3.set_(8, 9, 8.8);
    //    m3.set_(9, 1, 6.6);
    //    m3.set_(9, 7, 8.8);
    //
    //    SparseMatrix m4(10, 10);
    //    m4.set_(0, 0, 1.1);
    //    m4.set_(0, 9, 11);
    //    m4.set_(1, 3, 2.2);
    //    m4.set_(2, 6, 3.3);
    //    m4.set_(3, 3, 8.8);
    //    m4.set_(4, 0, 10);
    //    m4.set_(4, 5, 12.4);
    //    m4.set_(5, 8, 4.4);
    //    m4.set_(6, 5, 9.9);
    //    m4.set_(7, 2, 7.7);
    //    m4.set_(8, 7, 5.5);
    //    m4.set_(9, 5, 6.6);
    //
    //    m3.print(false);
    //    m4.print(false);
    //
    //    SparseMatrix m3m4 = m3 * m4;
    //
    //    m3m4.print(false);

    //g->removeEdge(g->getNode("A"), g->getNode("B"));
    //g->display();

    //g->removeNode("A");
    //g->display();

    return 0;
}













/*#include "RecSys.h"

using namespace std;


int main() {
    vector<string> data;
    ifstream fichier("ciao-data.txt");
    if(fichier) {
        //L'ouverture s'est bien passée, on peut donc lire
        string ligne; //Une variable pour stocker les lignes lues
        while(getline(fichier, ligne)) {
            data.push_back(ligne);

        }
    } else {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
    }

    cout << data[1] << endl;

    return 0;
}
*/
