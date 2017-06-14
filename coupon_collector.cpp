#include<iostream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<map>

using namespace std;

//Here we are trying to simulate a scenario to get the number of coupons need to be
//gathered to get all the coupons.
void simulate_prob(int no_it, int prb_spc, int seed, int freq_coupon[], int sum_freq) {
    srand(seed);
    int temp_count = 0;

    // probability array
    double prob_freq[prb_spc];
    for(int it=0; it<prb_spc; it++) {
        prob_freq[it] = double(freq_coupon[it])/sum_freq;
    }

    // cumulative prob_chart
    double prob_chart[prb_spc+1];
    for(int i=0; i < prb_spc + 1; i++) {
        if(i==0) {
            prob_chart[i] = 0;
        } else {
            prob_chart[i] = prob_freq[i-1] + prob_chart[i-1];
        }
    }

    double empiricalvalue =0;
    // no_it is the number of times to repeat experiment
    for(int it =0; it < no_it; it++) {
        int freq_holder[prb_spc] = {0};
        int counter = 0;
        while (1) {
            int product = 1;
            float random_number = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            for(int i_it =0; i_it < prb_spc; i_it++) {
                if (random_number > prob_chart[i_it] && random_number <= prob_chart[i_it+1]) {
                    freq_holder[i_it] += 1;
                }
            }

            for(int i_it =0; i_it < prb_spc; i_it++) {
                product = product * freq_holder[i_it];
            }
            counter++;

            if (product != 0) {
                break;
            }
        }
        empiricalvalue += counter;
    }
    cout << "Empirically predicted # of coupons :" << empiricalvalue/no_it << "\n\n";
}

//Algorithm : Coupon Collector. Here, Evaluated 14b equivalent expression.
void coupon_collector_algo(int prb_spc, int sum_freq, int cpn_freq[]) {
    double prob_freq[prb_spc];
    for(int it=0; it<prb_spc; it++) {
        prob_freq[it] = double(cpn_freq[it])/sum_freq;
    }
    double sum_prob;
    int val = pow(2, prb_spc) - 1;
    map<int, vector<double> > map_sum_count;
    for(int i_it =0; i_it < val ; i_it++) {
        int count = 0;
        double temp_sum = 0;
        for(int i_i_it = 0; i_i_it < prb_spc; i_i_it++) {
            if(i_it & (1 << i_i_it)) {
                temp_sum += prob_freq[i_i_it];
                count++;
            }
        }
        if (count != 0) {
            if(map_sum_count.count( count ) == 1) {
                vector<double> tmp = map_sum_count[count];
                tmp.push_back(temp_sum);
                map_sum_count[count] = tmp;
            } else if (map_sum_count.count( count ) == 0){
                vector<double> tmp;
                tmp.push_back(temp_sum);
                map_sum_count[count] = tmp;
            }
        }
    }

    double theoretical_val = 0;
    double first_val =0;
    for(map<int,vector<double> >::iterator it = map_sum_count.begin(); it != map_sum_count.end(); ++it) {
        first_val = pow(-1, (prb_spc -1 - it->first));
        double second_val = 0;
        double tmp_sum = 0;
        for (int j = 0 ; j < map_sum_count[it->first].size(); j++) {
            second_val += (1 / (1 - double(map_sum_count[it->first][j])));
        }
        theoretical_val += (first_val * second_val);
    }
    cout << "\nTheoretically predicted # of coupons : " << theoretical_val + (pow(-1, prb_spc - 1)) << "\n";
}

//Entry point
int main() {
    //Var declaration.
    int nCoupn;  //Number of coupons.
    int it = 1;

    //Get the number of coupons.
    cout << "Enter number of coupons : ";
    std::cin >> nCoupn;
    int freq_coupon[nCoupn];

    //Get the frequencies of all the coupons.
    int sum_freq = 0;
    int freq_value;
    std::cout << "Enter frequencies of each coupon sequentially : " ;
    while(it <= nCoupn) {
        std::cout << "\nEnter value of " << it << " coupon : ";
        std::cin >> freq_coupon[it - 1];
        sum_freq += freq_coupon[it-1];
        it++;
    }

    //call to coupon_collector_algo function.
    coupon_collector_algo(nCoupn, sum_freq, freq_coupon);

    //For empirically finding number of coupons to be collected.
    int seed;
    int no_it;
    cout << "\nEnter # of iterations for empirical : ";
    cin >> no_it;
    cout << "\nEnter seed : " ;
    cin >> seed;

    simulate_prob(no_it, nCoupn, seed, freq_coupon, sum_freq);
    return 0;
}
