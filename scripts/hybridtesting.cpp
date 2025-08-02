#include "strategies.cpp"

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
    size_t sample_size = 1000; // not too many

    cout << "Testing max_info_entropy_strategy_with_sampling (7-letter words):\n";
    vector<int> S_values;
    for (int S = 2; S <= 4096; S <<= 1) S_values.push_back(S);

    vector<int> sample_indices(seven_letter_words.size());
    iota(sample_indices.begin(), sample_indices.end(), 0);
    shuffle(sample_indices.begin(), sample_indices.end(), rng);
    sample_indices.resize(min(sample_size, seven_letter_words.size()));

    vector<double> avg_guesses_vec, avg_time_ms_vec;
    for (int S : S_values) {
        int total_guesses = 0;
        double total_time = 0.0;
        for (size_t i = 0; i < sample_indices.size(); ++i) {
            auto [guesses, elapsed] = max_info_entropy_strategy_with_sampling(seven_letter_words[sample_indices[i]], 7, S, rng);
            total_guesses += guesses;
            total_time += elapsed;
        }
        double avg_guesses = (double)total_guesses / sample_size;
        double avg_time_ms = (total_time / sample_size) * 1000;
        avg_guesses_vec.push_back(avg_guesses);
        avg_time_ms_vec.push_back(avg_time_ms);
        cout << "S = " << S
             << " | avg guesses: " << avg_guesses
             << " | avg time (ms): " << avg_time_ms << endl;
    }
    // Print summary table
    cout << "\n===== SUMMARY (max_info_entropy_strategy_with_sampling, 7-letter) =====\n";
    cout << left << setw(8) << "S" << setw(16) << "Avg Guesses" << setw(16) << "Avg Time (ms)" << endl;
    cout << string(40, '-') << endl;
    for (size_t i = 0; i < S_values.size(); ++i) {
        cout << left << setw(8) << S_values[i]
             << setw(16) << fixed << setprecision(3) << avg_guesses_vec[i]
             << setw(16) << fixed << setprecision(3) << avg_time_ms_vec[i] << endl;
    }
    return 0;
}
