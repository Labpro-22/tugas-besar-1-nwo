#include "models/Player.hpp"

using namespace std;

Player::Player(string name, int startBalance) : username(name), balance(startBalance), position(0), status("ACTIVE"), jailTurns(0), hasUsedSkillCardThisTurn(false) {}
Player::~Player() {} // Hand pointers (SkillCard) dihapus oleh CardDeck, Player tidak mendelete-nya.

string Player::getUsername() const { return username; }
int Player::getBalance() const { return balance; }
int Player::getPosition() const { return position; }
string Player::getStatus() const { return status; }
int Player::getTotalWealth() const {
    int wealth = balance;
    for (auto i = 0 ;i<ownedProperties.size();i++){
        wealth += ownedProperties[i]->getBuyPrice();
    }
    return wealth;
}
const vector<SkillCard*>& Player::getHand() const { return hand; }
const vector<PropertyTile*>& Player::getOwnedProperties() const { return ownedProperties; }
int Player::getJailTurns() const { return jailTurns; }

void Player::setPosition(int pos) { position = pos; }
void Player::setStatus(string stat) { status = stat; }
void Player::setJailTurns(int turns) { jailTurns = turns; }
void Player::setUsedSkillCardThisTurn(bool used) { hasUsedSkillCardThisTurn = used; }

Player& Player::operator+=(int amount) {
    balance += amount;
    return *this;
}
Player& Player::operator-=(int amount) {
    if (balance < amount) throw InsufficientFundsException(amount, balance);
    balance -= amount;
    return *this;
}
bool Player::operator>(const Player& other) const {
    return this->getTotalWealth() > other.getTotalWealth();
}
bool Player::operator<(const Player& other) const {
    return this->getTotalWealth() < other.getTotalWealth();
}

void Player::addSkillCard(SkillCard* card) {
    if (hand.size() >= 3) throw HandFullException();
    hand.push_back(card);
}
void Player::dropSkillCard(int index) {
    if (index >= 0 && index < hand.size()) hand.erase(hand.begin() + index);
}
void Player::addProperty(PropertyTile* prop) { ownedProperties.push_back(prop); }
void Player::removeProperty(PropertyTile* prop) {
    for (auto i=0;i<ownedProperties.size();i++){
        if(ownedProperties[i]->getCode()==prop->getCode()){
            ownedProperties.erase(ownedProperties.begin()+i);
            break;
        }
    }
}