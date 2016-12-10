#include "Equipment.h"

EquipmentSlot::EquipmentSlot()
{
    this->_item = NULL;
};

EquipmentSlot::EquipmentSlot(spItem item)
{
    EquipmentSlot();
    this->set_item(item);
};

spItem EquipmentSlot::get_item()
{
    return this->_item;
};

void EquipmentSlot::set_item(spItem item)
{
    this->_item = item;
};

Equipment::Equipment()
{
    this->combat_slot = std::make_unique<EquipmentSlot>();
    this->mining_slot = std::make_unique<EquipmentSlot>();
    this->recipe_slot = std::make_unique<EquipmentSlot>();
};
