#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

struct Player {
    int skill_of_player;
    int index;
};

bool ComparePlayerByFeature(const Player& first_player, const Player& second_player) {
    if (first_player.skill_of_player < second_player.skill_of_player) {
        return true;
    }
    return false;
}

bool ComparePlayerByIndex(const Player& first_player, const Player& second_player) {
    if (first_player.index < second_player.index) {
        return true;
    }
    return false;
}

void MakeDataStoreOfPlayers(vector<Player>& players) {
    int number_of_players;
    cin >> number_of_players;
    players.resize(number_of_players);
    int number_of_player = 0;
    for (auto& player : players) {
        int skill_of_player;
        cin >> skill_of_player;
        player.skill_of_player = skill_of_player;
        player.index = number_of_player;
        ++number_of_player;
    }
}

void QuickSort(vector<Player>::iterator first_element_in_vector,
               vector<Player>::iterator last_element_in_vector,
               bool (*compare_players)(const Player&, const Player&)) {
    while (last_element_in_vector - first_element_in_vector > 1) {

        Player pivot = *(first_element_in_vector +
                         rand() % (last_element_in_vector - first_element_in_vector));
        vector<Player>::iterator pointer_less_pivot = first_element_in_vector;
        vector<Player>::iterator pointer_more_pivot = last_element_in_vector;
        while (pointer_less_pivot < pointer_more_pivot) {
            if (pointer_more_pivot != last_element_in_vector) {
                ++pointer_less_pivot;
            }
            --pointer_more_pivot;
            while ((*compare_players)(pivot, *pointer_more_pivot)) {
                --pointer_more_pivot;
            }
            while ((*compare_players)(*pointer_less_pivot, pivot)) {
                ++pointer_less_pivot;
            }
            if (pointer_less_pivot < pointer_more_pivot) {
                std::swap(*pointer_less_pivot, *pointer_more_pivot);
            }
        }
        if (pointer_more_pivot + 1 - first_element_in_vector <
            last_element_in_vector - pointer_more_pivot + 1) {
            QuickSort(first_element_in_vector, pointer_more_pivot + 1, (*compare_players));
            first_element_in_vector = pointer_more_pivot + 1;
        } else {
            QuickSort(pointer_more_pivot + 1, last_element_in_vector, (*compare_players));
            last_element_in_vector = pointer_more_pivot + 1;
        }
    }
}

int64_t FindBestTeamEffectiveness(const vector<Player>& players, vector<Player>& team) {
    if (players.size() == 1) {
        team.resize(1);
        team.at(0).index = 0;
        team.at(0).skill_of_player = players.at(0).skill_of_player;
        return team.at(0).skill_of_player;
    }
    vector<Player> sorted_players = players;
    QuickSort(sorted_players.begin(), sorted_players.end(), ComparePlayerByFeature);
    int64_t best_team_effectiveness = 0;
    int64_t now_sum_effectiveness = 0;
    int sorted_number_of_worst_player = 0;
    int sorted_number_of_first_player_in_best_team = 0;
    int sorted_number_of_last_player_in_best_team = 0;
    int player_index = 0;
    while (player_index < static_cast<int>(sorted_players.size())) {
        int64_t acceptable_skill_of_best_player =
            static_cast<int64_t>(sorted_players[sorted_number_of_worst_player].skill_of_player) +
            static_cast<int64_t>(sorted_players[sorted_number_of_worst_player + 1].skill_of_player);
        if (sorted_players[player_index].skill_of_player <= acceptable_skill_of_best_player) {
            now_sum_effectiveness += sorted_players[player_index].skill_of_player;
            if (player_index + 1 == static_cast<int>(sorted_players.size())) {
                if (now_sum_effectiveness > best_team_effectiveness) {
                    best_team_effectiveness = now_sum_effectiveness;
                    sorted_number_of_first_player_in_best_team = sorted_number_of_worst_player;
                    sorted_number_of_last_player_in_best_team = player_index;
                }
                team.clear();
                for (int number_player_in_sorted_team = sorted_number_of_first_player_in_best_team;
                     number_player_in_sorted_team <= sorted_number_of_last_player_in_best_team;
                     ++number_player_in_sorted_team) {
                    team.push_back(sorted_players[number_player_in_sorted_team]);
                }
                QuickSort(team.begin(), team.end(), ComparePlayerByIndex);
                return best_team_effectiveness;
            }
            ++player_index;
        } else {
            if (now_sum_effectiveness > best_team_effectiveness) {
                best_team_effectiveness = now_sum_effectiveness;
                sorted_number_of_first_player_in_best_team = sorted_number_of_worst_player;
                sorted_number_of_last_player_in_best_team = player_index - 1;
            }
            now_sum_effectiveness -= sorted_players[sorted_number_of_worst_player].skill_of_player;
            ++sorted_number_of_worst_player;
        }
    }
    return best_team_effectiveness;
}

void Output(const int64_t maximal_effectiveness, vector<Player>& team) {
    cout << maximal_effectiveness << "\n";
    for (auto& player : team) {
        cout << player.index + 1 << " ";
    }
}

int main() {
    vector<Player> players;
    MakeDataStoreOfPlayers(players);
    vector<Player> team;
    int64_t maximal_effectiveness = FindBestTeamEffectiveness(players, team);
    Output(maximal_effectiveness, team);
    return 0;
}
