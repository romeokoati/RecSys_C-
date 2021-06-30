#ifndef DEF_RECSYS
#define DEF_RECSYS


#include "pageRank.hpp"


#define LINK_WEIGHT int t = (int)substream[j][0]; \
                    string u = to_string(substream[j][1]); \
                    string i = to_string(substream[j][2]); \
                    string c = to_string(substream[j][3]); \
                    double r = substream[j][4]; \
                    string ustr = "u"+u, istr = "i"+i, cstr = "c"+c; \
                    double ui_w = rating_to_link_weight(u, r, m_user_rating_mean, m_user_list_id, m_rating_max);

#define ADD_TO_GRAPH_NO_CONTENT m_graph->add_node(ustr, "u"); \
								m_graph->add_node(istr, "i"); \
								m_graph->add_edge(m_graph->getNode(ustr), m_graph->getNode(istr), 1, 1, t); \
								m_graph->add_edge(m_graph->getNode(istr), m_graph->getNode(ustr), 1, 1, t);

#define ADD_TO_GRAPH_NCI m_graph->add_node(cstr, "c"); \
						 m_graph->add_edge(m_graph->getNode(cstr), m_graph->getNode(istr), 1, 1, t); \
						 m_graph->add_edge(m_graph->getNode(istr), m_graph->getNode(cstr), 1, 1, t);

#define ADD_TO_GRAPH_NCU m_graph->add_node(cstr, "c"); \
						 m_graph->add_edge(m_graph->getNode(cstr), m_graph->getNode(ustr), 1, 1, t); \
						 m_graph->add_edge(m_graph->getNode(ustr), m_graph->getNode(cstr), 1, 1, t);

#define ADD_TO_GRAPH_SSTR_ISTR m_graph->add_node(sstr, "s"); \
								m_graph->add_edge(m_graph->getNode(sstr), m_graph->getNode(istr), 1, 1, t); \
								m_graph->add_edge(m_graph->getNode(istr), m_graph->getNode(sstr), 1, 1, t);


#define ADD_TO_GRAPH_LSG_NO_CONTENT m_graph->add_node(ut, "u"); \
						            m_graph->add_node(it, "i"); \
						            m_graph->add_edge(m_graph->getNode(ut), m_graph->getNode(it), 1, 1, t); \
						            m_graph->add_edge(m_graph->getNode(it), m_graph->getNode(ut), 1, 1, t); \
						            \
                                    if(m_user_last_sessions.find(ustr) != m_user_last_sessions.end()){ \
                                        string last_ut = m_user_last_sessions[ustr]; \
                                        m_graph->add_edge(m_graph->getNode(ut), m_graph->getNode(last_ut), 1, 1, t); \
                                        m_graph->add_edge(m_graph->getNode(last_ut), m_graph->getNode(ut), 1, 1, t); \
                                        m_user_last_sessions[ustr] = ut; \
                                    } \
                                    if(m_item_last_sessions.find(istr) != m_item_last_sessions.end()){ \
                                        string last_it = m_item_last_sessions[istr]; \
                                        m_graph->add_edge(m_graph->getNode(it), m_graph->getNode(last_it), 1, 1, t); \
                                        m_graph->add_edge(m_graph->getNode(last_it), m_graph->getNode(it), 1, 1, t); \
                                        m_item_last_sessions[istr] = it; \
                                    }

#define ADD_TO_GRAPH_LSG_NCI m_graph->add_node(ct, "c"); \
								 m_graph->add_edge(m_graph->getNode(ct), m_graph->getNode(it), 1, 1, t); \
								 m_graph->add_edge(m_graph->getNode(it), m_graph->getNode(ct), 1, 1, t); \
                                if(m_content_last_sessions.find(cstr) != m_content_last_sessions.end()){ \
                                    string last_ct = m_content_last_sessions[cstr]; \
                                    m_graph->add_edge(m_graph->getNode(ct), m_graph->getNode(last_ct), 1, 1, t); \
                                    m_graph->add_edge(m_graph->getNode(last_ct), m_graph->getNode(ct), 1, 1, t); \
                                    m_content_last_sessions[cstr] = ct; \
                                }

#define ADD_TO_GRAPH_LSG_NCU m_graph->add_node(ct, "c"); \
                             m_graph->add_edge(m_graph->getNode(ct), m_graph->getNode(ut), 1, 1, t); \
                             m_graph->add_edge(m_graph->getNode(ut), m_graph->getNode(ct), 1, 1, t); \
                            if(m_content_last_sessions.find(cstr) != m_content_last_sessions.end()){ \
                                string last_ct = m_content_last_sessions[cstr]; \
                                m_graph->add_edge(m_graph->getNode(ct), m_graph->getNode(last_ct), 1, 1, t); \
                                m_graph->add_edge(m_graph->getNode(last_ct), m_graph->getNode(ct), 1, 1, t); \
                            }

#define REC_LIST_BEGIN for(int j=0; j<all_items.size(); j++){ \
                            all_items_istr.push_back("i" + all_items[j]); \
                        }\
                        for (int i=0; i<nodes.size(); i++){\
                            d[nodes[i]->name] = 0; \
                        }\
                        for (int j=0; j<users_to_rec.size(); j++){ \
                            string ustr = "u" + to_string(users_to_rec[j]); \
                        }\

#define REC_LIST_TRUST if(m_user_trust.find(users_to_rec[j]) != m_user_trust.end())\
                            u_trusted = m_user_trust[users_to_rec[j]];

#define REC_LIST_RESET_PERS_VECTOR d[ustr] = 0; \
                    d[sstr] = 0; \
                    for (int i=0; i<u_trusted.size(); i++){ \
                        string uistr = "u"+to_string(u_trusted[i]); \
                        d[uistr] = 0; \
                        if(m_user_last_sessions.find(uistr) != m_user_last_sessions.end()){ \
                            string suistr = m_user_last_sessions[ustr]; \
                            d[suistr] = 0; \
                        } \
                    }

#define REC_LIST_SUM_D double sum=0.0; \
                        for(map<string, double>::iterator it = d.begin(); it != d.end(); ++it) \
                            sum += it->second; \
                        double sum_d = sum * 1.0; \
                        for(map<string, double>::iterator it = d.begin(); it!=d.end(); ++it) \
                            d[it->first] = (d[it->first] * 1.0)/sum_d;

#define REC_LIST_END    for (int i=0; i<nodes.size(); i++){ \
                            for (int j=0; j<nodes.size(); j++){ \
                                if(m_rating_matrix[i][m_user_list_id[users_to_rec[j]]] == 0.0){ \
                                    new_items_indices.push_back(i); \
                                    new_items.push_back("i" + to_string(m_id_item_list[i])); \
                                } \
                            } \
                        } \
                        for (int j=0; j<new_items.size(); j++) \
                            if(rank_.find(new_items[j]) != rank_.end()) \
                                new_items_final.push_back(new_items[j]); \
                        for (int j=0; j<new_items_final.size(); j++) \
                            if(rank_[new_items_final[j]] > 1e-15) \
                                new_items_rank[stoi(new_items_final[j])] = rank_[new_items_final[j]]; \
                        for(map<int, double>::iterator it = new_items_rank.begin(); (it != new_items_rank.end()) ; ++it) \
                            rec[users_to_rec[j]].push_back(it->first); \
                    } \
                    return rec;

#define REC_LIST_END_LSG vector<int> new_items; \
                for (int i=0; i<all_items.size(); i++) \
                    if(find(user_item_list[users_to_rec[j]].begin(), user_item_list[users_to_rec[j]].end(), all_items[i]) != \
                       user_item_list[users_to_rec[j]].end()) \
                        new_items.push_back(all_items[i]); \
                map<int, double> new_items_rank; \
                for(map<string, double>::iterator it = rank_.begin(); it!=rank_.end(); ++it){ \
                    string str = it->first;\
                    if(to_string(str[0])=="i"){\
                        int item = stoi(str); \
                        for (int i=0; i<new_items.size(); i++){ \
                            if(new_items[i] == item){ \
                                for(map<int, double>::iterator it2 = new_items_rank.begin(); it2!=new_items_rank.end(); ++it2){ \
                                    if((it2->first)==item) \
                                        new_items_rank[item] += rank_[str]; \
                                    else \
                                        new_items_rank[item] = rank_[str]; \
                                } \
                            } \
                        } \
                    }\
                }\
                map<int, double> new_items_rank_final; \
                for(map<int, double>::iterator it2 = new_items_rank.begin(); it2!=new_items_rank.end(); ++it2){ \
                    int item = it2->first; \
                    if(new_items_rank[item] > 1e-16) \
                        new_items_rank_final[item] = new_items_rank[item]; \
                } \
                for(map<int, double>::iterator it = new_items_rank_final.begin(); it != new_items_rank_final.end(); ++it) \
                    rec[users_to_rec[j]].push_back(it->first); \
            }\
            return rec;



#define cwd getcwd
#define cd chdir



/**#
###############################################################################
###############################################################################
#
#  RECOMMENDER SYSTEMS BASIC GENERIC CLASS
#
###############################################################################
#*/

struct User_trust_map {
	int id_user;
	std::vector<int> id_trusted_users;
};


//# global information on users and items
struct Global_info {
    std::map<int, std::vector<int>> user_trust_map;
    //std::vector<User_trust_map> user_trust_map;
    std::vector<double> user_rating_mean;
    std::vector<std::vector<double>> user_jaccard_similarity;
    double rating_info[3];				// rating info [max, median, min]
    std::map<int, int> user_list_id, item_list_id;
    std::map<int, int> id_user_list, id_item_list;
    int nb_ratings;
    std::vector<std::vector<double>> rating_matrix;

};

struct List_param {
    std::string graph;
    std::string content;
    std::string time;
    std::string trust;
    int delta;
    double beta;
    int t0;
    int k;
    double gamma;
    double alpha;
    List_param(std::string graph, std::string content, std::string time, std::string trust,
               int delta, double beta, int t0, int k, double gamma, double alpha){
        this->graph = graph;
        this->content = content;
        this->time = time;
        this->trust = trust;
        this->delta = delta;
        this->beta = beta;
        this->t0 = t0;
        this->k = k;
        this->gamma = gamma;
        this->alpha = alpha;
    }
};

class Recsysgen {

    public:
    Recsysgen(int tbegin, std::vector<int> recsys_id, std::string name);
    void update_recsys(std::vector<std::vector<double>> substream, std::vector<std::vector<double>> cumulate_substream, Global_info global_info);
    std::map<int, std::vector<int>> get_recommended_list(std::vector<int> users_to_rec, std::map<int, std::vector<int>> user_item_list, std::vector<int> all_items);
    ~Recsysgen();

    //private:
    int m_tbegin;
    std::vector<int> m_recsys_id;
    std::string m_name;
};
/**#
###############################################################################
###############################################################################
#
# GRAPHRS (RECSYS GRAPH)
#
###############################################################################
###############################################################################
#
*/


class GraphRecsys : public Recsysgen {

    public:
    GraphRecsys();
    GraphRecsys(int tbegin, std::vector<int> recsys_id, std::string name, int graph_type=0, double alpha=0,
                 double time=0, double nt=0, double ta=0, int content=0, double delta=0,
                 double beta=0, double kp=0, int k=0);
    std::string __str__();
    void update_recsys(std::vector<std::vector<double>> substream, std::vector<std::vector<double>> cumulate_substream, Global_info global_info);
    std::map<int, std::vector<int>> get_recommended_list(std::vector<int> users_to_rec, std::map<int, std::vector<int>> user_item_list, std::vector<int> all_items);
    void _time_weight(double tnow=0.0);
    ~GraphRecsys();

    public:

    double m_alpha=0, m_time=0, m_nt=0, m_ta=0, m_delta=0, m_beta=0, m_kp=0;

    //private:
    //int m_tbegin;
    //std::vector<int> m_recsys_id;
    //std::string m_name;
    int m_graph_type=0, m_k=0, m_content=0;

    Graph* m_graph;

    std::map<std::string, std::string> m_user_last_sessions, m_item_last_sessions, m_content_last_sessions;
    std::map<std::string, std::string> m_users_info, m_items_info;

    std::map<int, std::vector<int>> m_user_trust;
    //std::vector<User_trust_map> m_user_trust;
    std::vector<double> m_user_rating_mean;
    std::vector<std::vector<double>> m_user_pearson_similarity;

    double m_rating_max, m_rating_median, m_rating_min;
    std::map<int, int> m_user_list_id, m_item_list_id;
    std::map<int, int> m_id_user_list, m_id_item_list;

    std::vector<std::vector<double>> m_rating_matrix;

};

// for get_recSys result return
struct RecSys_result {
	GraphRecsys* recsys;
	std::vector<std::string> param_list;

	RecSys_result(GraphRecsys* recsys, std::vector<std::string> param_list){
        this->recsys = recsys;
		this->param_list = param_list;
	}
	//List_param param_list;
};


double rating_to_link_weight(std::string u, double r, std::vector<double> user_rating_mean, std::map<int, int> user_list_id, double max_rating);

RecSys_result* get_recsys(std::vector<int> recsys_id);






#endif
