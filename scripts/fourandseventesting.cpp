#include "strategies.cpp"
#include <iomanip>

void benchmark_strategies(const vector<pair<string, int>>& words, int word_length, int sample_size, mt19937& rng) {
    vector<int> indices(words.size());
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), rng);
    indices.resize(min(sample_size, (int) words.size()));

    // Strategy results: {total_guesses, total_time}
    pair<int, double> random_results = {0, 0};
    pair<int, double> minimax_binsize_results = {0, 0};
    pair<int, double> max_num_bins_results = {0, 0};
    pair<int, double> max_info_entropy_results = {0, 0};

    int progress = 0;
    cout << "Starting benchmark for " << word_length << "-letter words..." << endl;
    for (int idx : indices) {
        if (++progress % 100 == 0) {
            cout << "Progress: " << progress << "/" << sample_size << endl;
        }
        auto [r_guesses, r_time] = random_strategy(words[idx], word_length, rng);
        auto [n_guesses, n_time] = minimax_binsize_strategy(words[idx], word_length);
        auto [b_guesses, b_time] = max_num_bins_strategy(words[idx], word_length);
        auto [e_guesses, e_time] = max_info_entropy_strategy(words[idx], word_length);

        random_results.first += r_guesses;
        random_results.second += r_time;
        minimax_binsize_results.first += n_guesses;
        minimax_binsize_results.second += n_time;
        max_num_bins_results.first += b_guesses;
        max_num_bins_results.second += b_time;
        max_info_entropy_results.first += e_guesses;
        max_info_entropy_results.second += e_time;
    }

    // Print results table
    cout << "\n===== " << word_length << "-letter words (sample size: " << sample_size << ") =====\n";
    cout << left << setw(25) << "Strategy" 
         << setw(20) << "Avg Guesses" 
         << setw(20) << "Avg Time (ms)" << endl;
    cout << string(65, '-') << endl;

    auto print_row = [&](const string& name, const pair<int, double>& results) {
        cout << left << setw(25) << name 
             << setw(20) << fixed << setprecision(3) << (double)results.first / sample_size
             << setw(20) << fixed << setprecision(3) << (results.second * 1000) / sample_size << endl;
    };

    print_row("Random", random_results);
    print_row("Minimax Binsize", minimax_binsize_results);
    print_row("MaxNumBins", max_num_bins_results);
    print_row("MaxInfoEntropy", max_info_entropy_results);
}

int main() {
    ifstream file4("words/4-letter-words.txt");
    ifstream file7("words/7-letter-words.txt");
    string line;
    int idx4 = 0, idx7 = 0;
    while (getline(file4, line)) {
        if (not line.empty()) four_letter_words.push_back({line, idx4++});
    }
    while (getline(file7, line)) {
        if (not line.empty()) seven_letter_words.push_back({line, idx7++});
    }
    file4.close();
    file7.close();

    random_device rd;
    mt19937 rng(rd());
    int sample_size = 5;

    cout << "\nWord list sizes:" << endl;
    cout << "4-letter words: " << four_letter_words.size() << endl;
    cout << "7-letter words: " << seven_letter_words.size() << endl;

    if(four_letter_words.empty() or seven_letter_words.empty()) {
        cerr << "Error: Word lists are empty! Please check file paths." << endl;
        return 1;
    }

    cout << "\nBenchmarking strategies..." << endl;
    benchmark_strategies(four_letter_words, 4, sample_size, rng);
    benchmark_strategies(seven_letter_words, 7, sample_size, rng);

    return 0;
}