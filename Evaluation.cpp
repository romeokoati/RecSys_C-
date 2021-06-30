#include "Evaluation.hpp"

using namespace std;


std::vector<std::string> metrics = {"hr","prec","recall","std::map","mrr","f0.5","f1","f2"};
std::vector<int> tops_n = {1,2,3,5,10,15,20,30,40,50,100};
std::vector<std::string> eval_metrics = {
    "hr@1","hr@2","hr@3","hr@5","hr@10","hr@15","hr@20","hr@30","hr@40","hr@50","hr@100",
    "prec@1","prec@2","prec@3","prec@5","prec@10","prec@15","prec@20","prec@30","prec@40","prec@50","prec@100",
    "recall@1","recall@2","recall@3","recall@5","recall@10","recall@15","recall@20","recall@30","recall@40","recall@50","recall@100",
    "std::map@1","std::map@2","std::map@3","std::map@5","std::map@10","std::map@15","std::map@20","std::map@30","std::map@40","std::map@50","std::map@100",
    "mrr@1","mrr@2","mrr@3","mrr@5","mrr@10","mrr@15","mrr@20","mrr@30","mrr@40","mrr@50","mrr@100",
    "f0.5@1","f0.5@2","f0.5@3","f0.5@5","f0.5@10","f0.5@15","f0.5@20","f0.5@30","f0.5@40","f0.5@50","f0.5@100",
    "f1@1","f1@2","f1@3","f1@5","f1@10","f1@15","f1@20","f1@30","f1@40","f1@50","f1@100",
    "f2@1","f2@2","f2@3","f2@5","f2@10","f2@15","f2@20","f2@30","f2@40","f2@50","f2@100"
};

vector<string> Evaluation::evaluation_metric_list () {
    return eval_metrics;
}


Evaluation::Evaluation(map<int, vector<int>> links_to_rec, map<int, vector<int>> rec_links){
    if (links_to_rec.size() < 0)
        cerr << "Evaluation : links_to_rec bad value!";
    if (rec_links.size() < 0)
        cerr << "Evaluation : rec_links bad value!";
    m_links_to_rec = links_to_rec;
    m_rec_links = rec_links;
    //m_rec_links_binary = NULL;
    //m_result_values = map<string, double>;
    //m_result_weights = map<string, double>;
    m_evaluation_metrics = eval_metrics;
}
Evaluation::~Evaluation(){}


vector<string> Evaluation::get_evaluation_metrics(){
    return m_evaluation_metrics;
}
map<string, double> Evaluation::get_result_values(){
    return m_result_values;
}
map<string, double> Evaluation::get_result_weights(){
    return m_result_weights;
}

void Evaluation::compute_evaluation_results(){
    //# transform all recommendation list to binary list
    vector<string> keyDist;
    map<int, vector<int>>::iterator im;

    for(im=m_links_to_rec.begin() ; im!=m_links_to_rec.end() ; im++){
        int idx = (*im).first;
        m_rec_links_binary[idx] = {};
        int taille = (m_rec_links[idx]).size(), i = 0, j = 0;
//        for(i=0; i<taille; i++){
//            vector<int> links;
//            links = m_links_to_rec[idx];
//            int link = m_rec_links[idx])[i];
//            if(find(links.begin(), links.end(), link)){
//                m_rec_links_binary[idx].push_back(1);
//            } else {
//                m_rec_links_binary[idx].push_back(0);
//            }
//        }
    }
    //# return all evaluation
    map<string, map<int, double>> eval_results, eval_result_weights;
    map<string, map<int, double>> result1 = _get_hit_ratio();
    eval_results["hr"] = result1["hr"]; eval_result_weights["hr"] = result1["w"];
    map<string, map<int, double>> result2 = _get_precision();
    eval_results["prec"]= result2["prec"];  eval_result_weights["prec"] = result2["w"];
    map<string, map<int, double>> result3 = _get_recall();
    eval_results["recall"] = result3["recall"];  eval_result_weights["recall"] = result3["w"];
    map<string, map<int, double>> result4 = _get_map();
    eval_results["map"] = result4["map"];  eval_result_weights["map"] = result4["w"];
    map<string, map<int, double>> result5 = _get_mrr();
    eval_results["mrr"] = result5["mrr"];  eval_result_weights["mrr"] = result5["w"];
    map<string, map<int, double>> result6 = _get_fmeasure(0.5);
    eval_results["f0.5"] = result6["f0.5"];  eval_result_weights["f0.5"] = result6["w"];
    map<string, map<int, double>> result7 = _get_fmeasure(1);
    eval_results["f1"] = result7["f1"];  eval_result_weights["f1"] = result7["w"];
    map<string, map<int, double>> result8 = _get_fmeasure(2);
    eval_results["f2"] = result8["f2"];  eval_result_weights["f2"] = result8["w"];

    for(int i=0; i<metrics.size(); i++){
        for(int j=0; j<tops_n.size(); j++){
            m_result_values[metrics[i]+"@"+to_string(tops_n[j])] = eval_results[metrics[i]][tops_n[j]];
            m_result_weights[metrics[i]+"@"+to_string(tops_n[j])] = eval_result_weights[metrics[i]][tops_n[j]];
        }
    }
}

map<string, map<int, double>> Evaluation::_get_hit_ratio(){
    map<int, double> hri, wi;
    int nb_u = 1.0 * m_rec_links_binary.size();
    for(int i=0; i<tops_n.size(); i++){
        hri[tops_n[i]] = 0.0;
        wi[tops_n[i]] = nb_u;
    }
    if(nb_u > 0){
        for(map<int, vector<int>>::iterator im=m_rec_links_binary.begin(); im!=m_rec_links_binary.end(); im++){
            int idx = (*im).first;
            for(int j=0; j<tops_n.size(); j++){
                double sum = 0.0;
                for(int top=0; tops_n[top]<tops_n[j]; top++){
                    sum += m_rec_links_binary[idx][tops_n[top]];
                }
                if(sum >= 1)
                    hri[tops_n[j]] += 1;
            }
        }
        for(int i=0; i<tops_n.size(); i++){
            hri[tops_n[i]] = (hri[tops_n[i]] * 1.0)/nb_u;
        }
    }
    map<string, map<int, double>> result;
    result["w"] = wi;
    result["hr"] = hri;
    return result;
}
map<string, map<int, double>> Evaluation::_get_precision(){
    map<int, double> preci, wi, deno_preci, nume_preci;
    for(int i=0; i<tops_n.size(); i++){
        preci[tops_n[i]] = 0.0;
        deno_preci[tops_n[i]] = 0.0;
        nume_preci[tops_n[i]] = 0.0;
        wi[tops_n[i]] = 0.0;
    }
    for(map<int, vector<int>>::iterator im=m_rec_links_binary.begin(); im!=m_rec_links_binary.end(); im++){
        int idx = (*im).first;
        for(int j=0; j<tops_n.size(); j++){
            //# denominator [number of recommendations]
            vector<int> tmp;
            for(int i=0; i<j; i++)
                tmp.push_back(m_rec_links[idx][tops_n[i]]);
            deno_preci[tops_n[j]] += tmp.size();

            //# numerator [number of good recommendations]
            double sum = 0.0;
            for(int top=0; tops_n[top]<tops_n[j]; top++){
                sum += m_rec_links_binary[idx][tops_n[top]];
            }
            nume_preci[tops_n[j]] = sum;
        }
    }
    for(int i=0; i<tops_n.size(); i++){
        preci[tops_n[i]] = (deno_preci[tops_n[i]] > 0) ? (1.0 * nume_preci[tops_n[i]])/(1.0 * deno_preci[tops_n[i]]) : 0.0;
        wi[tops_n[i]] = deno_preci[tops_n[i]];
    }
    map<string, map<int, double>> result;
    result["w"] = wi;
    result["preci"] = preci;
    return result;
}

map<string, map<int, double>> Evaluation::_get_recall(){
    map<int, double> recalli, wi, deno_recalli, nume_recalli;
    for(int i=0; i<tops_n.size(); i++){
        recalli[tops_n[i]] = 0.0;
        deno_recalli[tops_n[i]] = 0.0;
        nume_recalli[tops_n[i]] = 0.0;
        wi[tops_n[i]] = 0.0;
    }
    for(map<int, vector<int>>::iterator im=m_rec_links_binary.begin(); im!=m_rec_links_binary.end(); im++){
        int idx = (*im).first;
        for(int j=0; j<tops_n.size(); j++){
            //# denominator [number of links observed]
            int u_nb_links_to_rec = m_links_to_rec[idx].size();
            deno_recalli[tops_n[j]] += u_nb_links_to_rec;

            //# numerator [number of good recommendations]
            double sum = 0.0;
            for(int top=0; tops_n[top]<tops_n[j]; top++){
                sum += m_rec_links_binary[idx][tops_n[top]];
            }
            nume_recalli[tops_n[j]] = sum;
        }
    }
    for(int i=0; i<tops_n.size(); i++){
        //# compute the recall metric
        recalli[tops_n[i]] = (deno_recalli[tops_n[i]] > 0) ? (1.0 * nume_recalli[tops_n[i]])/(1.0 * deno_recalli[tops_n[i]]) : 0.0;
        wi[tops_n[i]] = deno_recalli[tops_n[i]];
    }
    map<string, map<int, double>> result;
    result["w"] = wi;
    result["recall"] = recalli;
    return result;
}

map<string, map<int, double>> Evaluation::_get_map(){
    double nb_u = 1.0 * (m_rec_links_binary.size());
    map<int, double> mapi, wi, nume_mapi;
    for(int i=0; i<tops_n.size(); i++){
        mapi[tops_n[i]] = 0.0;
        nume_mapi[tops_n[i]] = 0.0;
        wi[tops_n[i]] = nb_u;
    }
    if(nb_u > 0){
        for(map<int, vector<int>>::iterator im=m_rec_links_binary.begin(); im!=m_rec_links_binary.end(); im++){
            int idx = (*im).first;
            for(int j=0; j<tops_n.size(); j++){
                vector<int> tmp;
                for(int i=0; i<j; i++)
                    tmp.push_back(m_rec_links_binary[idx][tops_n[i]]);
                nume_mapi[tops_n[j]] += _get_average_precision(tmp);
            }
        }
        for(int i=0; i<tops_n.size(); i++){
            //# compute the map metric
            mapi[tops_n[i]] = (1.0 * nume_mapi[tops_n[i]])/(1.0 * nb_u);
        }
    }
    map<string, map<int, double>> result;
    result["w"] = wi;
    result["map"] = mapi;
    return result;
}

double Evaluation::_get_average_precision(vector<int> user_rec_links_binary){
    double average_precision = 0.0;
    vector<int> indexes_good_rec;
    for(int i=0; i<user_rec_links_binary.size(); i++){
        if(user_rec_links_binary[i] == 1){
            indexes_good_rec.push_back(i);
        }
    }
    for(int i=0; i<indexes_good_rec.size(); i++){
        average_precision += (i+1)/(1.0 * (indexes_good_rec[i]+1));
    }
    average_precision = indexes_good_rec.size() > 0 ? (1.0 * average_precision)/(1.0 * indexes_good_rec.size()) : 0.0;
    return average_precision;
}

map<string, map<int, double>> Evaluation::_get_mrr(){
    double nb_u = 1.0 * (m_rec_links_binary.size());
    map<int, double> mrri, wi, nume_mrri;
    for(int i=0; i<tops_n.size(); i++){
        mrri[tops_n[i]] = 0.0;
        nume_mrri[tops_n[i]] = 0.0;
        wi[tops_n[i]] = nb_u;
    }
    if(nb_u > 0){
        for(map<int, vector<int>>::iterator im=m_rec_links_binary.begin(); im!=m_rec_links_binary.end(); im++){
            int idx = (*im).first;
            for(int j=0; j<tops_n.size(); j++){
                vector<int> tmp;
                for(int i=0; i<j; i++)
                    tmp.push_back(m_rec_links_binary[idx][tops_n[i]]);
                nume_mrri[tops_n[j]] += _get_reciprocal_rank(tmp);
            }
        }
        for(int i=0; i<tops_n.size(); i++){
            //# compute the mrr metric
            mrri[tops_n[i]] = nume_mrri[tops_n[i]]/(1.0 * nb_u);
        }
    }
    map<string, map<int, double>> result;
    result["w"] = wi;
    result["mrr"] = mrri;
    return result;
}

double Evaluation::_get_reciprocal_rank(vector<int> user_rec_links_binary){
    double reciprocal_rank = 0.0;
    auto it = find(user_rec_links_binary.begin(), user_rec_links_binary.end(), 1);
    if(it != user_rec_links_binary.end()){
        int index = it - user_rec_links_binary.begin();
        reciprocal_rank = 1.0/(1.0 * (index+1));
    }
    return reciprocal_rank;
}

map<string, map<int, double>> Evaluation::_get_fmeasure(double b){
    map<int, double> fmi, wi, true_positivei, false_negativei, false_positivei;
    for(int i=0; i<tops_n.size(); i++){
        fmi[tops_n[i]] = 0.0;
        true_positivei[tops_n[i]] = 0.0;
        false_negativei[tops_n[i]] = 0.0;
        false_positivei[tops_n[i]] = 0.0;
        wi[tops_n[i]] = 0.0;
    }
    for(map<int, vector<int>>::iterator im=m_rec_links_binary.begin(); im!=m_rec_links_binary.end(); im++){
        int idx = (*im).first;
        for(int j=0; j<tops_n.size(); j++){
            //# True positive [number of good recommendations]
            double sum = 0.0;
            for(int top=0; tops_n[top]<tops_n[j]; top++){
                sum += m_rec_links_binary[idx][tops_n[top]];
            }
            true_positivei[tops_n[j]] += sum;

            //# False negative [number of links observed but which are not predicted]
            false_negativei[tops_n[j]] += (m_links_to_rec[idx].size() - true_positivei[tops_n[j]]);

            //# False positive [predicted True but which are False]
            false_positivei[tops_n[j]] += (tops_n[j] - true_positivei[tops_n[j]]);
        }
    }
    for(int i=0; i<tops_n.size(); i++){
        //# numerator of F-measure
        fmi[tops_n[i]] = (1 + b*b) * true_positivei[tops_n[i]];
        //# denominator of F-measure
        wi[tops_n[i]] = ((1 + b*b) * true_positivei[tops_n[i]]) + (b*b * false_negativei[tops_n[i]]) + false_positivei[tops_n[i]];
        fmi[tops_n[i]] = (wi[tops_n[i]] > 0) ? (1.0 * fmi[tops_n[i]])/(1.0 * wi[tops_n[i]]) : 0.0;
    }
    map<string, map<int, double>> result;
    result["w"] = wi;
    result["fm"] = fmi;
    return result;
}

