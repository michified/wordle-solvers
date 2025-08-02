#pragma GCC optimization("Ofast,unroll-loops")
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <chrono>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

vector<pair<string, int>> four_letter_words;
vector<pair<string, int>> seven_letter_words;

string determine_wordle_colors(const string& guess, const string& answer) {
    string colors(guess.size(), 'X');
    string answer_copy = answer;
    // Check for correct letters in the correct position
    for (size_t i = 0; i < guess.size(); ++i) {
        if (guess[i] == answer[i]) {
            colors[i] = 'G';
            answer_copy[i] = '*'; // Mark as used
        }
    }
    // Check for correct letters in the wrong position
    for (size_t i = 0; i < guess.size(); ++i) {
        if (colors[i] == 'X') {
            auto pos = answer_copy.find(guess[i]);
            if (pos != string::npos) {
                colors[i] = 'Y';
                answer_copy[pos] = '*';
            }
        }
    }
    return colors;
}

string fetch_wordle_colors(int i, int j, int k) {
    if (k == 4) {
        return determine_wordle_colors(four_letter_words[i].first, four_letter_words[j].first);
    } else {
        return determine_wordle_colors(seven_letter_words[i].first, seven_letter_words[j].first);
    }
}

pair<int, double> random_strategy(const pair<string, int>& answer, int k, mt19937& rng) {
    const auto& l = (k == 4) ? four_letter_words : seven_letter_words;
    string f(l[0].first.size(), 'X');
    int guesses = 0;
    double elapsed = 0.0;
    int answer_idx = answer.second;
    vector<int> indices;
    for (const auto& p : l) indices.push_back(p.second);
    while (f != string(l[0].first.size(), 'G')) {
        uniform_int_distribution<size_t> dist(0, indices.size() - 1);
        int guess_idx = indices[dist(rng)];
        guesses++;
        f = fetch_wordle_colors(guess_idx, answer_idx, k);
        vector<int> next_indices;
        auto start = chrono::high_resolution_clock::now();
        for (int w_idx : indices) {
            if (fetch_wordle_colors(guess_idx, w_idx, k) == f) {
                next_indices.push_back(w_idx);
            }
        }
        indices = next_indices;
        auto end = chrono::high_resolution_clock::now();
        elapsed += chrono::duration<double>(end - start).count();
    }
    return {guesses, elapsed};
}

string convert_num_to_colors(int num, int k) {
    string colors(k, 'X');
    for (int i = 0; i < k; ++i) {
        if (num % 3 == 0) colors[i] = 'G';
        else if (num % 3 == 1) colors[i] = 'Y';
        num /= 3;
    }
    return colors;
}

int convert_colors_to_num(const string& colors) {
    int num = 0;
    for (char c : colors) {
        num *= 3;
        if (c == 'G') num += 0;
        else if (c == 'Y') num += 1;
        else num += 2; // 'X'
    }
    return num;
}

pair<int, double> max_neg_bin_size_strategy(const pair<string, int>& answer, int k) {
    const auto& l = (k == 4) ? four_letter_words : seven_letter_words;
    string f(l[0].first.size(), 'X');
    int guesses = 0;
    double elapsed = 0.0;
    int answer_idx = answer.second;

    int best_guess_idx = l[0].second;
    for (const auto& p : l) {
        if (p.first == (k == 4 ? "orae" : "tarlies")) {
            best_guess_idx = p.second;
            break;
        }
    }
    vector<int> indices;
    for (const auto& p : l) indices.push_back(p.second);
    while (f != string(l[0].first.size(), 'G')) {
        guesses++;
        auto start = chrono::high_resolution_clock::now();
        f = fetch_wordle_colors(best_guess_idx, answer_idx, k);
        vector<int> next_indices;
        for (int w_idx : indices) {
            if (fetch_wordle_colors(best_guess_idx, w_idx, k) == f) {
                next_indices.push_back(w_idx);
            }
        }
        indices = next_indices;
        if (indices.empty()) break;
        best_guess_idx = indices[0];
        int min_largest_bin = indices.size();
        for (int w0_idx = 0; w0_idx < l.size(); ++w0_idx) {
            map<int, int> bin_sizes;
            for (int w1_idx : indices) {
                string colors = fetch_wordle_colors(w0_idx, w1_idx, k);
                bin_sizes[convert_colors_to_num(colors)]++;
            }
            int largest_bin = 0;
            for (const auto& [_, size] : bin_sizes) {
                if (size > largest_bin) largest_bin = size;
            }
            if (largest_bin < min_largest_bin) {
                min_largest_bin = largest_bin;
                best_guess_idx = w0_idx;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        elapsed += chrono::duration<double>(end - start).count();
    }
    return {guesses, elapsed};
}

pair<int, double> max_num_bins_strategy(const pair<string, int>& answer, int k) {
    const auto& l = (k == 4) ? four_letter_words : seven_letter_words;
    string f(l[0].first.size(), 'X');
    int guesses = 0;
    double elapsed = 0.0;
    int answer_idx = answer.second;

    int best_guess_idx = l[0].second;
    for (const auto& p : l) {
        if (p.first == (k == 4 ? "eats" : "parties")) {
            best_guess_idx = p.second;
            break;
        }
    }
    vector<int> indices;
    for (const auto& p : l) 
        indices.push_back(p.second);
    while (f != string(l[0].first.size(), 'G')) {
        guesses++;
        if (indices.size() == 1) break;
        auto start = chrono::high_resolution_clock::now();
        f = fetch_wordle_colors(best_guess_idx, answer_idx, k);
        vector<int> next_indices;
        for (int w_idx : indices) {
            if (fetch_wordle_colors(best_guess_idx, w_idx, k) == f) {
                next_indices.push_back(w_idx);
            }
        }
        indices = next_indices;
        if (indices.empty()) break;
        best_guess_idx = indices[0];
        int max_num_bins = -1;
        for (int w0_idx = 0; w0_idx < l.size(); ++w0_idx) {
            set<string> bins;
            for (int w1_idx : indices) {
                string colors = fetch_wordle_colors(w0_idx, w1_idx, k);
                bins.insert(colors);
            }
            if ((int)bins.size() > max_num_bins) {
                max_num_bins = bins.size();
                best_guess_idx = w0_idx;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        elapsed += chrono::duration<double>(end - start).count();
    }
    return {guesses, elapsed};
}

double shannon_entropy(const vector<int>& bin_sizes, int total) {
    double entropy = 0.0;
    for (int size : bin_sizes) {
        if (size == 0) continue;
        double p = (double)size / total;
        entropy -= p * log2(p);
    }
    return entropy;
}

pair<int, double> max_info_entropy_strategy(const pair<string, int>& answer, int k) {
    const auto& l = (k == 4) ? four_letter_words : seven_letter_words;
    string f(l[0].first.size(), 'X');
    int guesses = 0;
    double elapsed = 0.0;
    int answer_idx = answer.second;

    int best_guess_idx = l[0].second;
    for (const auto& p : l) {
        if (p.first == (k == 4 ? "sare" : "tarlies")) {
            best_guess_idx = p.second;
            break;
        }
    }
    vector<int> indices;
    for (const auto& p : l) indices.push_back(p.second);
    while (f != string(l[0].first.size(), 'G')) {
        guesses++;
        if (indices.size() == 1) break;
        auto start = chrono::high_resolution_clock::now();
        f = fetch_wordle_colors(best_guess_idx, answer_idx, k);
        vector<int> next_indices;
        for (int w_idx : indices) {
            if (fetch_wordle_colors(best_guess_idx, w_idx, k) == f) {
                next_indices.push_back(w_idx);
            }
        }
        indices = next_indices;
        if (indices.empty()) break;
        best_guess_idx = indices[0];
        double max_entropy = -1.0;
        for (int w0_idx = 0; w0_idx < l.size(); ++w0_idx) {
            map<string, int> bin_sizes_map;
            for (int w1_idx : indices) {
                string colors = fetch_wordle_colors(w0_idx, w1_idx, k);
                bin_sizes_map[colors]++;
            }
            vector<int> bin_sizes;
            for (const auto& [_, size] : bin_sizes_map) bin_sizes.push_back(size);
            double entropy = shannon_entropy(bin_sizes, indices.size());
            if (entropy > max_entropy) {
                max_entropy = entropy;
                best_guess_idx = w0_idx;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        elapsed += chrono::duration<double>(end - start).count();
    }
    return {guesses, elapsed};
}

pair<int, double> max_info_entropy_strategy_with_sampling(const pair<string, int>& answer, int k, int S, mt19937& rng) {
    const auto& l = (k == 4) ? four_letter_words : seven_letter_words;
    string f(l[0].first.size(), 'X');
    int guesses = 1;
    double elapsed = 0.0;
    int answer_idx = answer.second;

    int best_guess_idx = l[0].second;
    for (const auto& p : l) {
        if (p.first == (k == 4 ? "sare" : "tarlies")) {
            best_guess_idx = p.second;
            break;
        }
    }
    vector<int> indices;
    for (const auto& p : l) indices.push_back(p.second);
    while (f != string(l[0].first.size(), 'G')) {
        guesses++;
        f = fetch_wordle_colors(best_guess_idx, answer_idx, k);
        vector<int> next_indices;
        for (int w_idx : indices) {
            if (fetch_wordle_colors(best_guess_idx, w_idx, k) == f) {
                next_indices.push_back(w_idx);
            }
        }
        auto start = chrono::high_resolution_clock::now();
        indices = next_indices;
        if (indices.size() <= 1) break;
        best_guess_idx = indices[0];
        double max_entropy = -1.0;
        for (int w0_idx = 0; w0_idx < l.size(); ++w0_idx) {
            map<string, int> bin_sizes_map;
            if (indices.size() > S) {
                vector<int> sampled_indices = indices;
                shuffle(sampled_indices.begin(), sampled_indices.end(), rng);
                if (sampled_indices.size() > S) sampled_indices.resize(S);
                for (int w1_idx : sampled_indices) {
                    string colors = fetch_wordle_colors(w0_idx, w1_idx, k);
                    bin_sizes_map[colors]++;
                }
            } else {
                for (int w1_idx : indices) {
                    string colors = fetch_wordle_colors(w0_idx, w1_idx, k);
                    bin_sizes_map[colors]++;
                }
            }
            vector<int> bin_sizes;
            for (const auto& [_, size] : bin_sizes_map) bin_sizes.push_back(size);
            double entropy = shannon_entropy(bin_sizes, indices.size());
            if (entropy > max_entropy) {
                max_entropy = entropy;
                best_guess_idx = w0_idx;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        elapsed += chrono::duration<double>(end - start).count();
    }
    return {guesses, elapsed};
}