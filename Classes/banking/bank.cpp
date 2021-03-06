#include "Bank.h"
#include  "GameLogic.h"
#include  "Beatup.h"

Bank* Bank::_instance = NULL;

Bank::Bank()
{
    this->_total_coins_banked = 0.0; //TODO load in from serializer
}

Bank* Bank::getInstance()
{
    if (!Bank::_instance)
    {
        Bank::_instance = new Bank();
    };

    return Bank::_instance;
};

void Bank::pocket_or_bank_coins(res_count_t added_coins)
{
    res_count_t max_coins = BEATUP->get_max_coin_storage();
    res_count_t coin_storage_left = BEATUP->get_coin_storage_left();

    //default values
    res_count_t to_total_coins = added_coins;
    res_count_t to_bank = 0.0;

    //if too many coins to fit, bank the rest
    if (added_coins > coin_storage_left) {
        to_total_coins = coin_storage_left;
        to_bank = added_coins-coin_storage_left;
    }

    BEATUP->add_total_coins(to_total_coins);

    //make sure you're not removing coins
    if (to_bank > 0.0) {
        this->add_total_coins_banked(to_bank);
    } else if (to_bank < 0.0) {
        //CCASSERT(false, "likely negative to_bank");
        //CCLOG("Negative coins to bank, this is likely an error.");
    };
};

res_count_t Bank::get_total_coins_banked() const
{
    //never return less than 0 banked
    return std::max(this->_total_coins_banked, 0.0L);
};

void Bank::add_total_coins_banked(res_count_t added_value)
{
    this->_total_coins_banked = std::max(this->_total_coins_banked + added_value, 0.0L);
};

void Bank::set_total_coins_banked(res_count_t new_total)
{
    this->_total_coins_banked = new_total;
};

void Bank::transfer_to_bank(res_count_t to_bank)
{

    if (to_bank < 0) { return; };

    res_count_t total_coins = BEATUP->get_total_coins();

    //make sure there's enough in total coins
    res_count_t to_transfer = std::min(to_bank, total_coins);

    //remove from total
    BEATUP->add_total_coins(-to_transfer);
    this->add_total_coins_banked(to_transfer);
};

void Bank::transfer_from_bank(res_count_t from_bank)
{
    if (from_bank < 0) { return; };

    res_count_t coin_storage_left = BEATUP->get_coin_storage_left();

    if (from_bank > coin_storage_left && coin_storage_left > 0) {
        from_bank = coin_storage_left;

    }

    //make sure there's enough in coins banked
    res_count_t to_transfer = std::min(from_bank, this->get_total_coins_banked());
    this->add_total_coins_banked(-to_transfer);
    BEATUP->add_total_coins(to_transfer);
};

