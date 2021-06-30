#include "RecSys.hpp"
#include "utils.hpp"


using namespace std;

Recsysgen::Recsysgen (int tbegin, vector<int> recsys_id, string name) {
    m_tbegin = tbegin;
    m_recsys_id = recsys_id;
    m_name = name;
}
Recsysgen::~Recsysgen(){

}

GraphRecsys::GraphRecsys(int tbegin, vector<int> recsys_id, string name, int graph_type, double alpha,
                         double time, double nt, double ta, int content, double delta,
                         double beta, double kp, int k) : Recsysgen(tbegin, recsys_id, name) {
    //# additional attributes for all graph-based recsys
    m_graph = new Graph(Graph::DefaultNodes::SINGLE, 20, 40);
    m_graph_type = graph_type;              //# 0 for Bipartite, 1 for STG, 2 for LSG
    m_alpha = alpha;                    //# for the pagerank
    //# additional attributes for time weight
    m_time = time; //#if len(time_weight_functions) > time else 0
    m_nt = (nt >= 1.0)? nt : 1;
    //m_tfunction = time_weight_functions[time];
    m_ta = (ta == NULL)? 0 : ta;
    //# additional parameters for content-based graphs
    m_content = content;
    //# additional parameters for session-based graphs
    m_delta = (delta >= 1.0)? delta : 1;
    m_beta = beta;
    //# additional parameters for continuous graph
    //# additional parameterpagerank_scipys for User Common Behavior ( Interest )
    m_k = k;
    m_kp = kp;
    //# users global information
}

GraphRecsys::~GraphRecsys(){}

string GraphRecsys::__str__(){
    string ucbtype = "";
    if(m_k==0)
        ucbtype="NA";
    else if(m_k>0)
        ucbtype="TopK";
    else
        ucbtype="All";
    string ss(m_recsys_id.begin(), m_recsys_id.end());
    string str = "id: " + ss + ", name: " + m_name + ", gtype: "+ to_string(m_graph_type) + ", ctype: " +
                    to_string(m_content) + ", twtype: " + to_string(m_time) + ", ucbtype: " + ucbtype + ", delta: " +
                    to_string(m_delta) + ", k: " + to_string(m_k) + ", beta: " + to_string(m_beta) + ", alpha: " +
                    to_string(m_alpha) + ", nt: " + to_string(m_nt) + ", ta: " + to_string(m_ta);
    return str;
}

void GraphRecsys::update_recsys(vector<vector<double>> substream, vector<vector<double>> cumulate_substream, Global_info global_info){
    //# users global information
    m_user_trust = global_info.user_trust_map;
    //m_user_trust.id_user = global_info.user_trust_map.id_user;
    //m_user_trust.id_trusted_users = global_info.user_trust_map.id_trusted_users;
    m_user_rating_mean = global_info.user_rating_mean;
    m_user_pearson_similarity = global_info.user_jaccard_similarity;

    //# rating info [max, medpagerank_scipyian, min]
    m_rating_max = global_info.rating_info[0];
    m_rating_median = global_info.rating_info[1];
    m_rating_min = global_info.rating_info[2];

    //# user_list_id item_list_id****
    m_user_list_id = global_info.user_list_id;
    m_item_list_id = global_info.item_list_id;
    m_id_user_list = global_info.id_user_list;
    m_id_item_list = global_info.id_item_list;

    //# rating matrix
    m_rating_matrix = global_info.rating_matrix;
    /**#######################################################################
    #
    # BIPARTITE GRAPH
    #
    #######################################################################
    #*/
    if(m_graph_type == 0){
        /**#
        ###################################################################
        # NO CONTENT*/
        if(m_content < 0){
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0)
                    ADD_TO_GRAPH_NO_CONTENT;
            }
        }
        /**#
        ###################################################################
        # CONTENT WITH NCI ONLY*/
        else if(m_content == 1){
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    ADD_TO_GRAPH_NO_CONTENT;
                    ADD_TO_GRAPH_NCI;
                }
            }
        }
        /**
        ###################################################################
        # CONTENT WITH NCU ONLY*/
        else if(m_content == 2){
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    ADD_TO_GRAPH_NO_CONTENT;
                    ADD_TO_GRAPH_NCU;
                }
            }
        }
        /**#
        ###################################################################
        # CONTENT WITH NCI AND NCU*/
        else {
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    ADD_TO_GRAPH_NO_CONTENT;
                    ADD_TO_GRAPH_NCI;
                    ADD_TO_GRAPH_NCU;
                }
            }
        }
    }
    /**#
    #######################################################################
    #
    # SESSION BASED TEMPORAL GRAPH
    #
    #######################################################################
    #*/
    else if(m_graph_type == 1){
        if(m_delta < 1)
            m_delta = 1;
        /**#
        ###################################################################
        # NO CONTENT*/
        if(m_content < 0){
            //std::vector<string> sstr_set, ustr_set, istr_set;
            //std::vector<int> T_set;
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    int T = 1 + int((t - m_tbegin)/m_delta);
                    //T_set.insert(T);
                    string ustr = "u" + u;
                    //string istr = "i" + i;
                    string sstr = "s(" + u + "," + to_string(T) + ")";
                    //sstr_set.insert(sstr);
                    //ustr_set.insert(ustr);
                    //istr_set.insert(istr);
                    //m_user_last_sessions[] = ustr;
                    m_user_last_sessions[ustr] = sstr;
                    ADD_TO_GRAPH_NO_CONTENT;
                    ADD_TO_GRAPH_SSTR_ISTR;
                }
            }
        }
        /**#
        ###################################################################
        # CONTENT WITH NCI ONLY*/
        else if(m_content == 1){
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    int T = 1 + (int)((t - m_tbegin)/m_delta);
                    string sstr = "s(" + u + "," + to_string(T) + ")";
                    //m_user_last_sessions[j][0] = ustr;
                    m_user_last_sessions[ustr] = sstr;
                    ADD_TO_GRAPH_NO_CONTENT;
                    ADD_TO_GRAPH_NCI;
                }
            }
        }
        /**
        ###################################################################
        # CONTENT WITH NCU ONLY*/
        else if(m_content == 2){
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    int T = 1 + (int)((t - m_tbegin)/m_delta);
                    string sstr = "s(" + u + "," + to_string(T) + ")";
                    //m_user_last_sessions[j][0] = ustr;
                    m_user_last_sessions[ustr] = sstr;
                    ADD_TO_GRAPH_NO_CONTENT;
                    ADD_TO_GRAPH_NCU;
                }
            }
        }
        /**#
        ###################################################################
        # CONTENT WITH NCI AND NCU*/
        else {
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    int T = 1 + (int)((t - m_tbegin)/m_delta);
                    string sstr = "s(" + u + "," + to_string(T) + ")";
                    //m_user_last_sessions[j][0] = ustr;
                    m_user_last_sessions[ustr] = sstr;
                    ADD_TO_GRAPH_NO_CONTENT;
                    ADD_TO_GRAPH_NCI;
                    ADD_TO_GRAPH_NCU;
                }
            }
        }
    }
    /**#
    #######################################################################
    #
    # LINKSTREAM GRAPH
    #
    #######################################################################
    #**/
    else if(m_graph_type == 2){
        /**#
        ###################################################################
        # NO CONTENT
        **/
        if(m_content < 0){
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    string ut = "u(" + u + "," + to_string(t) + ")";
                    string it = "i(" + i + "," + to_string(t) + ")";

                    ADD_TO_GRAPH_LSG_NO_CONTENT;
                }
            }
        }
        /**#
        ###################################################################
        # CONTENT WITH NCI ONLY*/
        else if(m_content == 1){
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    string ut = "u(" + u + "," + to_string(t) + ")";
                    string it = "i(" + i + "," + to_string(t) + ")";
                    string ct = "c(" + c + "," + to_string(t) + ")";

                    ADD_TO_GRAPH_LSG_NO_CONTENT;
                    ADD_TO_GRAPH_LSG_NCI;
                }
            }
        }
        /**
        ###################################################################
        # CONTENT WITH NCU ONLY*/
        else if(m_content == 2){
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    string ut = "u(" + u + "," + to_string(t) + ")";
                    string it = "i(" + i + "," + to_string(t) + ")";
                    string ct = "c(" + c + "," + to_string(t) + ")";

                    ADD_TO_GRAPH_LSG_NO_CONTENT;
                    ADD_TO_GRAPH_LSG_NCU;
                }
            }
        }
        /**#
        ###################################################################
        # CONTENT WITH NCI AND NCU*/
        else {
            for(int j=0; j<substream.size(); j++) {
                LINK_WEIGHT;
                if(ui_w >= 0){
                    string ut = "u(" + u + "," + to_string(t) + ")";
                    string it = "i(" + i + "," + to_string(t) + ")";
                    string ct = "c(" + c + "," + to_string(t) + ")";

                    ADD_TO_GRAPH_LSG_NO_CONTENT;
                    m_graph->add_edge(m_graph->getNode(ct), m_graph->getNode(ut), 1, 1, t);
                    m_graph->add_edge(m_graph->getNode(ut), m_graph->getNode(ct), 1, 1, t);
                    ADD_TO_GRAPH_LSG_NCI;

                }
            }
        }

    } else {
        printf("ERROR.......");
    }
    /**#
    #######################################################################
    # TIME WEIGHT **/
    int tnow = int(substream[substream.size()-1][0]);
    _time_weight(tnow);

}

map<int, vector<int>> GraphRecsys::get_recommended_list(vector<int> users_to_rec, map<int, vector<int>> user_item_list, vector<int> all_items)
{
    //#print self.k
    map<int, vector<int>> rec;
    map<string, int>::iterator it;
    if(users_to_rec.size() <= 0)
    {
        return rec;

    }


    vector<string> all_items_istr;
    vector<Node*> nodes = m_graph->nodes;
    map<string, double> d, dangling;
    vector<int> new_items_indices;
    vector<string> new_items;
    vector<string> new_items_final;
    map<int, double> new_items_rank;
    vector<int> u_trusted;

    if(m_graph_type == 0)
    {
        if(m_kp == 0)
        {
            for(map<int, int>::iterator it = m_item_list_id.begin(); it!=m_item_list_id.end(); ++it)
            {
                all_items_istr.push_back("i" + to_string(it->first));
            }


            // Initialize all nodes to 0
            for (int i=0; i<nodes.size(); i++)
            {
                d[nodes[i]->name] = 0;
            }

            for (int j=0; j<users_to_rec.size(); j++)
            {
                string ustr = "u" + to_string(users_to_rec[j]);
                d[ustr] = 1;
                map<string, double> rank_ = pagerank_scipy(m_graph, d, dangling, 10, m_alpha, 1e-15);
                d[ustr] = 0;
                REC_LIST_END;


            }

        }
        else if(m_kp == 1)
        {
                REC_LIST_BEGIN;
                //# configuration of the personalized vector
                d[ustr] = 1;

                // Attention à cette condition dans le cas ou elle est vérifié plusieurs fois
                REC_LIST_TRUST;
                for (int i=0; i<u_trusted.size(); i++)
                {
                    string uistr = "u"+to_string(u_trusted[i]);
                    d[uistr] = m_k * 1;
                    d[ustr] += (1 - m_k) * 1;
                }
                REC_LIST_SUM_D;
                //# computation of pagerank
                map<string, double> rank_ = pagerank_scipy(m_graph, d, dangling, 10, m_alpha, 1e-15);

                //# reset the personalized vector to 0
                d[ustr] = 0;
                for (int i=0; i<u_trusted.size(); i++)
                {
                    string uistr = "u"+to_string(u_trusted[i]);
                    d[uistr] =0;
                }

                REC_LIST_END;
        }
        else
        {
                REC_LIST_BEGIN;
                //# configuration of the personalized vector
                d[ustr] = 1;
                for(map<int, vector<int>>::iterator it = user_item_list.begin(); it!=user_item_list.end(); ++it)
                {
                    u_trusted.push_back(it->first);
                }



                for (int i=0; i<u_trusted.size(); i++)
                {
                    string uistr = "u"+to_string(u_trusted[i]);
                    d[uistr] = m_k * 1 * m_user_pearson_similarity[m_user_list_id[users_to_rec[j]]][m_user_list_id[u_trusted[i]]];
                    d[ustr] += (1 - m_k) * 1;
                }
                REC_LIST_SUM_D;

                // # computation of pagerank
                map<string, double> rank_ = pagerank_scipy(m_graph, d, dangling, 10, m_alpha, 1e-15);

                //# reset the personalized vector to 0
                d[ustr] = 0;
                for (int i=0; i<u_trusted.size(); i++)
                {
                    string uistr = "u"+to_string(u_trusted[i]);
                    d[uistr] =0;
                }

                REC_LIST_END;
         }
    }
    else if(m_graph_type == 1)
    {
        if(m_kp == 0)
        {
                REC_LIST_BEGIN;
                string sstr = m_user_last_sessions[ustr];
                d[ustr] = m_beta;
                d[sstr] = 1.0 - m_beta;
                map<string, double> rank_ = pagerank_scipy(m_graph, d, dangling, 10, m_alpha, 1e-15);
                d[ustr] = 0.0;
                d[sstr] = 0.0;
                REC_LIST_END;
        }
        else if(m_kp == 1)
        {
            REC_LIST_BEGIN;

                string sstr = m_user_last_sessions[ustr];

                //# configuration of the personalized vector
                d[ustr] = m_beta;
                d[sstr] = (1 - m_beta);

                REC_LIST_TRUST;
                for (int i=0; i<u_trusted.size(); i++){
                    string uistr = "u"+to_string(u_trusted[i]);
                    int ui_trust_weight = m_k * 1;
                    d[uistr] = m_beta * ui_trust_weight * m_k;
                    d[ustr] += m_beta * (1 - m_k);
                    for(map<string, string>::iterator it = m_user_last_sessions.begin(); it!=m_user_last_sessions.end(); ++it){
                        if(uistr == m_user_last_sessions[it->first]){
                            string suistr = m_user_last_sessions[ustr];
                            d[suistr] = (1 - m_beta) * ui_trust_weight * m_k;
                            d[sstr] += (1 - m_beta) * (1 - m_k);
                        }
                    }
                }
                REC_LIST_SUM_D;

                //# computation of pagerank
                map<string, double> rank_ = pagerank_scipy(m_graph, d, dangling, 10, m_alpha, 1e-15);

                //# reset the personalized vector to 0
                REC_LIST_RESET_PERS_VECTOR;
                REC_LIST_END;
        }
        else
        {
            REC_LIST_BEGIN;

                string sstr = m_user_last_sessions[ustr];

                //# configuration of the personalized vector
                d[ustr] = m_beta;
                d[sstr] = (1 - m_beta);

                for(map<int, vector<int>>::iterator it = user_item_list.begin(); it!=user_item_list.end(); ++it)
                    u_trusted.push_back(it->first);

                for (int i=0; i<u_trusted.size(); i++){
                    string uistr = "u"+to_string(u_trusted[i]);
                    double ui_trust_weight = m_k * 1 * m_user_pearson_similarity[m_user_list_id[users_to_rec[j]]][m_user_list_id[u_trusted[i]]];
                    d[uistr] = m_beta * ui_trust_weight * m_k;
                    d[ustr] += m_beta * (1 - m_k);
                    for(map<string, string>::iterator it = m_user_last_sessions.begin(); it!=m_user_last_sessions.end(); ++it){
                        if(uistr == m_user_last_sessions[it->first]){
                            string suistr = m_user_last_sessions[ustr];
                            d[suistr] = (1 - m_beta) * ui_trust_weight * m_k;
                            d[sstr] += (1 - m_beta) * (1 - m_k);
                        }
                    }
                }
                REC_LIST_SUM_D;

                //# computation of pagerank
                map<string, double> rank_ = pagerank_scipy(m_graph, d, dangling, 10, m_alpha, 1e-15);

                //# reset the personalized vector to 0
                REC_LIST_RESET_PERS_VECTOR;
                REC_LIST_END;
        }

    }
    else if(m_graph_type == 2)
    {
        if(m_kp == 0)
        {
            for (int i=0; i<nodes.size(); i++)
                d[nodes[i]->name] = 0;

            for (int j=0; j<users_to_rec.size(); j++){
                string ustr = "u" + to_string(users_to_rec[j]);
                string last_ut = m_user_last_sessions[ustr];
                d[last_ut] = 1;
                map<string, double> rank_ = pagerank_scipy(m_graph, d, dangling, 30, m_alpha, 1e-16);
                d[last_ut] = 0;

                REC_LIST_END_LSG;
            }
        }
        else if(m_kp == 1)
        {
            for (int i=0; i<nodes.size(); i++)
                d[nodes[i]->name] = 0;

            for (int j=0; j<users_to_rec.size(); j++){
                string ustr = "u" + to_string(users_to_rec[j]);
                string last_ut = m_user_last_sessions[ustr];

                //# configuration of the personalized vector
                d[last_ut] = 1;

                REC_LIST_TRUST;
                for (int i=0; i<u_trusted.size(); i++){
                    string uistr = "u"+to_string(u_trusted[i]);
                    int ui_trust_weight = m_k * 1;
                    d[uistr] = m_beta * ui_trust_weight * m_k;
                    d[ustr] += m_beta * (1 - m_k);
                    for(map<string, string>::iterator it = m_user_last_sessions.begin(); it!=m_user_last_sessions.end(); ++it){
                        if(uistr == m_user_last_sessions[it->first]){
                            string last_uit = m_user_last_sessions[uistr];
                            d[last_uit] = ui_trust_weight * m_k;
                            d[last_ut] += 1 * (1 - m_k);
                        }
                    }
                }
                REC_LIST_SUM_D;
                //# computation of pagerank
                map<string, double> rank_ = pagerank_scipy(m_graph, d, dangling, 30, m_alpha, 1e-16);

                //# reset the personalized vector to 0
                d[last_ut] = 0;
                for (int i=0; i<u_trusted.size(); i++){
                    string uistr = "u"+to_string(u_trusted[i]);
                    d[uistr] = 0;
                    for(map<string, string>::iterator it = m_user_last_sessions.begin(); it!=m_user_last_sessions.end(); ++it){
                        if(uistr == m_user_last_sessions[it->first]){
                            string last_uit = m_user_last_sessions[uistr];
                            d[last_uit] = 0;
                        }
                    }
                }
                REC_LIST_END_LSG;
            }
        }
        else
        {
            for (int i=0; i<nodes.size(); i++)
                d[nodes[i]->name] = 0;
            for (int j=0; j<users_to_rec.size(); j++)
            {
                string ustr = "u" + to_string(users_to_rec[j]);
                string last_ut = m_user_last_sessions[ustr];

                //# configuration of the personalized vector
                d[last_ut] = 1;

                for(map<int, vector<int>>::iterator it = user_item_list.begin(); it!=user_item_list.end(); ++it)
                    u_trusted.push_back(it->first);

                for (int i=0; i<u_trusted.size(); i++)
                {
                    string uistr = "u"+to_string(u_trusted[i]);
                    double ui_trust_weight = m_k * 1 * m_user_pearson_similarity[m_user_list_id[users_to_rec[j]]][m_user_list_id[u_trusted[i]]];
                    for(map<string, string>::iterator it = m_user_last_sessions.begin(); it!=m_user_last_sessions.end(); ++it)
                    {
                        if(uistr == m_user_last_sessions[it->first])
                        {
                            string last_uit = m_user_last_sessions[uistr];
                            d[last_uit] = ui_trust_weight * m_k;
                            d[last_ut] += 1 * (1 - m_k);
                        }
                    }
                }
                REC_LIST_SUM_D;
                //# computation of pagerank
                map<string, double> rank_ = pagerank_scipy(m_graph, d, dangling, 30, m_alpha, 1e-16);

                //# reset the personalized vector to 0
                d[last_ut] = 0;
                for (int i=0; i<u_trusted.size(); i++)
                {

                    string uistr = "u"+to_string(u_trusted[i]);
                    for(map<string, string>::iterator it = m_user_last_sessions.begin(); it!=m_user_last_sessions.end(); ++it)
                    {
                        if(uistr == m_user_last_sessions[it->first])
                        {
                            string last_uit = m_user_last_sessions[uistr];
                            d[last_uit] = 0;
                        }
                    }
                }
                REC_LIST_END_LSG;
        }
        }
    }
}



void GraphRecsys::_time_weight(double tnow){
    if((tnow!=NULL) && (m_time > 0)){
        for (int k=0; k<(m_graph->edges).size(); k++){
            double Dt = tnow - m_graph->edges[k]->time;
            double weight_init = m_graph->edges[k]->w_init;
            if(m_time == 1)
                m_graph->edges[k]->weight = tfunction_identity(weight_init, Dt, m_nt, m_ta);
            else if(m_time == 1)
                m_graph->edges[k]->weight = tfunction_half_life(weight_init, Dt, m_nt, m_ta);
            else if(m_time == 1)
                m_graph->edges[k]->weight = tfunction_logistic(weight_init, Dt, m_nt, m_ta);
            else if(m_time == 1)
                m_graph->edges[k]->weight = tfunction_constant_decay(weight_init, Dt, m_nt, m_ta);
            else
                m_graph->edges[k]->weight = tfunction_short_term(weight_init, Dt, m_nt, m_ta);
        }
    }
}

double rating_to_link_weight(string u, double r, vector<double> user_rating_mean,
                             map<int, int> user_list_id, double max_rating) {
    double r_u_mean = user_rating_mean[user_list_id[stoi(u)]];
    double num = r - r_u_mean;
    double deno = max_rating - r_u_mean;
    if(deno > 0)
        return num/(1.0 * deno);
    if(num >= 0)
        return 1.0;
    return 0.0;

}


RecSys_result* get_recsys(vector<int> recsys_id){
    vector<string> graph = {"bip", "stg", "lsg"};   //#, "lsg2"]
    vector<string> content = {"na", "ci", "cu", "ciu"};
    vector<string> time_weight = {"na", "edf", "lgf"};  //#, "cdf", "stf"]
    vector<string> common_interest = {"na", "tr", "ts"};    //#, "ss"]

    map<string, vector<string>> graph_param, content_param, time_weight_param, common_interest_param;
    graph_param["bip"] = {"alpha"};
    graph_param["stg"] = {"alpha", "delta", "beta"};
    graph_param["lsg"] = {"alpha"};
    //content_param = {"na":[], "ci":[], "cu":[], "ciu":[]}
    //time_weight_param = {"na":[], "edf":["nt"], "lgf":["nt", "ta"]}
    time_weight_param["edf"] = {"nt"};
    time_weight_param["ldf"] = {"nt", "ta"};
    //common_interest_param = {"na":[], "tr":["k"], "ts":["k"]}
    common_interest_param["tr"] = {"k"};
    common_interest_param["ts"] = {"k"};

    int g_id = (int)(recsys_id[0]);
    int c_id = (int)(recsys_id[1]);
    int tw_id = (int)(recsys_id[2]);
    int ci_id = (int)(recsys_id[3]);

    string recsys_name = graph[g_id] + "-" + content[c_id] + "-" + time_weight[tw_id] + "-" + common_interest[ci_id];

    GraphRecsys* recsys = new GraphRecsys(0, recsys_id, recsys_name, g_id, c_id, tw_id, ci_id);

    vector<string> param_list;
//    param_list.insert(param_list.end(),(graph_param[graph[g_id]]).begin(),(graph_param[graph[g_id]]).end());
//    param_list.insert(param_list.end(),(content_param[content[c_id]]).begin(),(content_param[content[c_id]]).end());
//    param_list.insert(param_list.end(),(time_weight_param[time_weight[tw_id]]).begin(),
//                      (time_weight_param[time_weight[tw_id]]).end());
//    param_list.insert(param_list.end(),(common_interest_param[common_interest[ci_id]]).begin(),
//                      (common_interest_param[common_interest[ci_id]]).end());

    RecSys_result* result = new RecSys_result(recsys, param_list);

    return result;

}




