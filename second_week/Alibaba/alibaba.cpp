#include <iostream>
#include <vector>

using std::cin;
using std::vector;

struct Coin{
    int position;
    int life_in_seconds;
};

void FillArrayOfCoins(vector<Coin>& coins){
    for(auto& coin: coins) {
        cin >> coin.position >> coin.life_in_seconds;
    }
}
int main(){
    int number_coins;
    cin >> number_coins;
    vector<Coin> coins (number_coins);
    FillArrayOfCoins(coins);

}