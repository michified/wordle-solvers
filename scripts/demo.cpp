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

vector<pair<string, int>> five_letter_words, possible_five_letter_words;

double shannon_entropy(const vector<int>& bin_sizes, int total) {
    double entropy = 0.0;
    for (int size : bin_sizes) {
        if (size == 0) continue;
        double p = (double)size / total;
        entropy -= p * log2(p);
    }
    return entropy;
}

string determine_wordle_colors(const string& guess, const string& answer) {
    string colors(guess.size(), 'X');
    string answer_copy = answer;
    // Check for correct letters in the correct position
    for (int i = 0; i < guess.size(); ++i) {
        if (guess[i] == answer[i]) {
            colors[i] = 'G';
            answer_copy[i] = '*'; // Mark as used
        }
    }
    // Check for correct letters in the wrong position
    for (int i = 0; i < guess.size(); ++i) {
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

string fetch_wordle_colors(int i, int j) {
    return determine_wordle_colors(five_letter_words[i].first, possible_five_letter_words[j].first);
}

string fetch_wordle_colors_from_user() {
    string user_input;
    cout << "Enter colors (G for green, Y for yellow, X for gray, an example is YXGXY): ";
    cin >> user_input;
    return user_input;
}

void max_info_entropy_strategy() {
    string f(5, 'X');

    int best_guess_idx = five_letter_words[0].second;
    for (const auto& p : five_letter_words) {
        if (p.first == "slate") {
            best_guess_idx = p.second;
            break;
        }
    }
    vector<int> indices;
    double max_entropy = -1.0;
    for (const auto& p : possible_five_letter_words) indices.push_back(p.second);
    while (f != "GGGGG") {
        if (max_entropy == -1.0) {
            cout << "Guess: " << five_letter_words[best_guess_idx].first << " (pre-set)" << endl;
        } else {
            cout << "Guess: " << five_letter_words[best_guess_idx].first << " (entropy: " << max_entropy   << ")" << endl;
        }
        f = fetch_wordle_colors_from_user();
        vector<int> next_indices;
        for (int w_idx : indices) {
            if (fetch_wordle_colors(best_guess_idx, w_idx) == f) {
                next_indices.push_back(w_idx);
            }
        }
        indices = next_indices;

        for (const auto& p : five_letter_words) {
            if (p.first == possible_five_letter_words[indices[0]].first) {
                best_guess_idx = p.second;
                break;
            }
        }
        if (indices.size() <= 1) break;
        
        max_entropy = 1.0;
        for (int w0_idx = 0; w0_idx < five_letter_words.size(); ++w0_idx) {
            map<string, int> bin_sizes_map;
            for (int w1_idx : indices) {
                string colors = fetch_wordle_colors(w0_idx, w1_idx);
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
    }
    if (indices.empty()) {
        cout << "A color input was wrong, or the answer does not exist in the word list." << endl;
        return;
    }
    cout << "Guess: " << five_letter_words[best_guess_idx].first << endl;
    cout << "Done!" << endl;
}

int main() {
    ifstream cin("words/5-letter-words.txt");
    string word;
    while (cin >> word) {
        five_letter_words.push_back({word, five_letter_words.size()});
    }
    cin.close();
    ifstream cin2("words/5-letter-words-possible-answers.txt");
    while (cin2 >> word) {
        possible_five_letter_words.push_back({word, possible_five_letter_words.size()});
    }
    cin2.close();
    max_info_entropy_strategy();
    return 0;
}