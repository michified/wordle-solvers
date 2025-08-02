import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Patch

# Function to plot guess distribution bar graphs
def plot_guess_distribution(data, averages, times, title, filename, ylim=None):
    fig, ax = plt.subplots(figsize=(12, 8))
    x = np.arange(1, len(data['Random']) + 1)
    width = 0.2
    strategies = ['Random', 'Minimax Binsize', 'MaxNumBins', 'Shannon Entropy']
    colors = ["#3293e1", "#d8a115", "#079219", "#d53232"]
    for i, strategy in enumerate(strategies):
        pos = x + (i - 1.5) * width + (i // 4) * 0.1  # Add 0.1 space after every 4 bars
        ax.bar(pos, data[strategy], width=width, color=colors[i], alpha=0.7)
    ax.set_xlabel('Number of Guesses', fontsize=14)
    ax.set_ylabel('Frequency', fontsize=14)
    ax.set_title(title, fontsize=16)
    ax.set_xticks(x + 0.1)  # Adjust x-ticks to center between groups
    ax.set_xticklabels([str(i) for i in x], fontsize=12)
    if ylim is not None:
        ax.set_ylim(ylim)
    # Add light grey dashed horizontal grid lines
    ax.grid(True, which='both', linestyle='--', linewidth=0.5, color='#d3d3d3', alpha=0.7)
    legend_elements = [
        Patch(facecolor=colors[i], label=f'{strategies[i]} (avg: {averages[strategy]:.3f}, time: {times[strategy]}ms)')
        for i, strategy in enumerate(strategies)
    ]
    ax.legend(handles=legend_elements, loc='upper right', fontsize=12)
    fig.patch.set_facecolor('#ffffff')
    ax.set_facecolor('#ffffff')
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    fig.savefig(filename)
    plt.show()

S = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096]
avg_guesses = [5.332, 4.566, 4.012, 3.699, 3.573, 3.554, 3.549, 3.528, 3.531, 3.531, 3.531, 3.531]
avg_time_ms = [366.185, 378.366, 408.702, 471.796, 657.074, 1057.562, 1535.861, 1725.401, 1737.857, 1778.008, 1777.712, 1778.627]

plt.style.use('default')
fig, ax1 = plt.subplots(figsize=(12, 8), dpi=100)
ax1.plot(S, avg_guesses, label='Average Guesses', marker='o', markersize=8, linewidth=2, color='#1f77b4', alpha=0.8)
ax1.set_xscale('log')
ax1.set_xlim(2, 4096)
ax1.set_xticks(S)
ax1.set_xticklabels([f'$2^{{{int(np.log2(x))}}}$' for x in S], fontsize=12)
ax1.set_xlabel('Number of Samples', fontsize=14, labelpad=10)
ax1.set_ylabel('Average Guesses', fontsize=14, color='#1f77b4', labelpad=10)
ax1.tick_params(axis='y', labelcolor='#1f77b4', labelsize=12)
ax1.axhline(4.01, color='red', linestyle=':', linewidth=2, label='Random Strategy: 4.01 Guesses')
ax2 = ax1.twinx()
ax2.plot(S, avg_time_ms, label='Average Time (ms)', marker='s', markersize=8, linewidth=2, linestyle='--', color='#ff7f0e', alpha=0.8)
ax2.set_ylabel('Average Time (ms)', fontsize=14, color='#ff7f0e', labelpad=10)
ax2.tick_params(axis='y', labelcolor='#ff7f0e', labelsize=12)
plt.title('Number of Samples vs Average Guesses and Time for Shannon Entropy Strategy', fontsize=16, pad=65)
handles1, labels1 = ax1.get_legend_handles_labels()
handles2, labels2 = ax2.get_legend_handles_labels()
fig.legend(handles1 + handles2, labels1 + labels2, loc='upper center', bbox_to_anchor=(0.5, 0.92), ncol=3, fontsize=12, frameon=True, facecolor='white', edgecolor='black', framealpha=0.8)
fig.patch.set_facecolor('#ffffff')
ax1.set_facecolor('#ffffff')
ax1.spines['top'].set_visible(False)
ax1.spines['right'].set_visible(False)
ax2.spines['top'].set_visible(False)
plt.tight_layout()
plt.subplots_adjust(top=0.82)
fig.savefig('random_sampling_graph.png')
plt.show()

data_k4 = {
    'Random': [0, 33, 137, 404, 603, 572, 497, 319, 213, 113, 64, 25, 9, 8, 2, 1, 0, 0, 0, 0],
    'Minimax Binsize': [0, 4, 99, 641, 1148, 862, 219, 24, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    'MaxNumBins': [1, 7, 199, 788, 1086, 708, 194, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    'Shannon Entropy': [0, 6, 130, 826, 1221, 690, 120, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
}
averages_k4 = {'Random': 6.299, 'Minimax Binsize': 5.178, 'MaxNumBins': 4.979, 'Shannon Entropy': 4.949}
times_k4 = {'Random': '<1', 'Minimax Binsize': '710', 'MaxNumBins': '724', 'Shannon Entropy': '615'}
plot_guess_distribution(data_k4, averages_k4, times_k4, '4-letter Wordle: Guess Distribution by Strategy', 'guess_distribution_k4.png', ylim=(0, 1300))

data_k7 = {
    'Random': [0, 40, 847, 1397, 541, 121, 37, 11, 4, 2],
    'Minimax Binsize': [0, 27, 1141, 1763, 69, 0, 0, 0, 0, 0],
    'MaxNumBins': [0, 23, 1458, 1473, 46, 0, 0, 0, 0, 0],
    'Shannon Entropy': [0, 23, 1450, 1487, 40, 0, 0, 0, 0, 0]
}
averages_k7 = {'Random': 4.014, 'Minimax Binsize': 3.625, 'MaxNumBins': 3.514, 'Shannon Entropy': 3.515}
times_k7 = {'Random': '6', 'Minimax Binsize': '1587', 'MaxNumBins': '2001', 'Shannon Entropy': '1761'}
plot_guess_distribution(data_k7, averages_k7, times_k7, '7-letter Wordle: Guess Distribution by Strategy', 'guess_distribution_k7.png', ylim=(0, 1800))