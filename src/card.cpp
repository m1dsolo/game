#include <game/card.hpp>

#include <game/global.hpp>
#include <game/event.hpp>
#include <game/entity_manager.hpp>

#include <game/component/hp.hpp>
#include <game/component/self.hpp>
#include <game/component/perk.hpp>

namespace wheel {

void MaxHpCard::execute() {
    for (auto [_, hp] : ecs.get_components<SelfComponent, HPComponent>()) {
        hp.max_hp += 1;
    }
}

void HealCard::execute() {
    for (auto [_, hp] : ecs.get_components<SelfComponent, HPComponent>()) {
        hp.hp = std::min(hp.max_hp, hp.hp + 3);
    }
}

void AtkCard::execute() {
    for (auto [_, perk] : ecs.get_components<SelfComponent, PerkComponent>()) {
        perk.atk_ratio += 0.5;
    }
}

void ShootSpeedCard::execute() {
    for (auto [_, perk] : ecs.get_components<SelfComponent, PerkComponent>()) {
        perk.shoot_speed_ratio += 0.5;
    }
}

void ReloadSpeedCard::execute() {
    for (auto [_, perk] : ecs.get_components<SelfComponent, PerkComponent>()) {
        perk.reload_speed_ratio += 1.;
    }
}

void AccuracyCard::execute() {
    for (auto [_, perk] : ecs.get_components<SelfComponent, PerkComponent>()) {
        perk.accuracy_ratio += 0.5;
    }
}

void FlashCooldownCard::execute() {
    for (auto [_, perk] : ecs.get_components<SelfComponent, PerkComponent>()) {
        perk.flash_cooldown_ratio += 0.5;
    }
}

void TowerCard::execute() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    EntityManager::instance().create_tower("archer_tower", entity);
}

CardFactory::CardFactory() {
    cards_map_[typeid(MaxHpCard)] = std::make_shared<MaxHpCard>();
    cards_map_[typeid(HealCard)] = std::make_shared<HealCard>();
    cards_map_[typeid(AtkCard)] = std::make_shared<AtkCard>();
    cards_map_[typeid(ShootSpeedCard)] = std::make_shared<ShootSpeedCard>();
    cards_map_[typeid(ReloadSpeedCard)] = std::make_shared<ReloadSpeedCard>();
    cards_map_[typeid(AccuracyCard)] = std::make_shared<AccuracyCard>();
    cards_map_[typeid(FlashCooldownCard)] = std::make_shared<FlashCooldownCard>();
    cards_map_[typeid(TowerCard)] = std::make_shared<TowerCard>();
}

std::shared_ptr<Card> CardFactory::get(size_t idx) const {
    std::vector<std::shared_ptr<Card>> cards;
    for (const auto& [_, card] : cards_map_) {
        cards.emplace_back(card);
    }
    return cards[idx];
}

}  // namespace wheel
