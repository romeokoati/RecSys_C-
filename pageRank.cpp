#include "pageRank.hpp"


std::map<std::string, double> pagerank_scipy (Graph* G, std::map<std::string, double> personalization, std::map<std::string, double> dangling,
                                   int max_iter, double alpha, double tol, std::string weight)
{
    std::map<std::string, double> result;
    int N = G->nodeNum;
    if(N == 0)
        return result;

    std::vector<Node*> nodes = G->nodes;
    //M = nx.to_scipy_sparse_matrix(G, nodelist=nodelist, weight=weight, dtype=float)
    //std::vector<std::vector<double>> M = G->matrix;
    //std::vector<std::vector<double>> Q;
    //std::vector<std::vector<double>> product(N, std::vector<double>(N, 0.0));
    std::vector<double> S;

    SparseMatrix M = to_sparse_matrix(G);
    SparseMatrix Q(N), product(N);

    // Initialize the sum vector
    for (int j=0; j<N; j++)
        S.push_back(0);

    // Sum element over the column axis.
    for (int i=0; i<N; i++)
        S[M.data[i]->col] += M.data[i]->value;

    // normalize the sum
    for (int j=0; j<N; j++)
        if(S[j] != 0.0)
            S[j] = 1.0 / S[j];
    // Diagonal matrix of column sum for M matrix
    for (int i=0; i<N; i++){
        if(S[i] != 0.0)
            Q.set_(i, i, S[i]);
    }
    // compute the next matrix power
    product = Q * M;

    //# initial std::vector
    std::vector<double> x, p, dangling_weights;
    for (int i = 0; i < N; i++)
        x.push_back(1.0 / N);

    //# Personalization std::vector
    if(personalization.empty())
    {
        for (int i = 0; i < N; i++)
            p.push_back(1.0 / N);
    }
    else
    {
        double sum = 0.0;
        for (int i = 0; i < nodes.size(); i++)
        {
            p.push_back(personalization[nodes[i]->name]);
            sum += p[i];
        }
        for (int i = 0; i < p.size(); i++)
            p[i] = p[i] / sum;
    }
    //# Dangling nodes
    if(dangling.empty())
    {
        for (int i = 0; i < p.size(); i++)
            dangling_weights.push_back(p[i]);
    }
    else
    {
        //# Convert the dangling dictionary into an array in nodelist order
        double sum = 0.0;
        for (int i = 0; i < nodes.size(); i++)
        {
            dangling_weights.push_back(dangling[nodes[i]->name]);
            sum += dangling_weights[i];
        }
        for (int i = 0; i < p.size(); i++)
            dangling_weights[i] = dangling_weights[i] / sum;
    }
    std::vector<int> is_dangling;
    for (int i=0; i<S.size(); i++)
        if(S[i] == 0.0)
            is_dangling.push_back(i);

    //# power iteration: make up to max_iter iterations
    for (int i = 0; i < max_iter; i++)
    {
        std::vector<double> xlast, tmp(N, 0.0);
        xlast = x;
        double sum = 0.0;
        for (int i = 0; i < is_dangling.size(); i++)
        {
            sum += x[is_dangling[i]];
        }
        // compute the x*M product for new x vector
        for (int k = 0; k < N; k++)
            tmp[product.data[k]->row] += x[product.data[k]->col] * (product.data[k]->value);
        // Compute the new x vector
        for (int k = 0; k < N; k++)
            x[k] = alpha * (tmp[k] + (sum * dangling_weights[k])) + ((1.0 - alpha) * p[k]);

        //# check convergence, l1 norm
        double err = 0.0;
        for (int k = 0; k < x.size(); k++)
            err += abs(x[k] - xlast[k]);
        if(err < N * tol){
            for (int k = 0; k < x.size(); k++)
                result[nodes[k]->name] = x[k];
            return result;
        }
    }
    for (int k = 0; k < x.size(); k++)
        result[nodes[k]->name] = x[k];
    return result; //#raise nx.PowerIterationFailedConvergence(max_iter)

}

