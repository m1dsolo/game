#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>

#include <wheel/singleton.hpp>

namespace wheel {

struct Card {
    Card(const std::string& name) : name(name) {}
    virtual ~Card() = default;

    virtual void execute() = 0;

    std::string name;
};

struct MaxHpCard : Card {
    MaxHpCard() : Card("max_hp+1") {}

    void execute() override;
};

struct HealCard : Card {
    HealCard() : Card("hp+3") {}

    void execute() override;
};

struct AtkCard : Card {
    AtkCard() : Card("atk+50%") {}

    void execute() override;
};

struct ShootSpeedCard : Card {
    ShootSpeedCard() : Card("shoot_speed+50%") {}

    void execute() override;
};

struct ReloadSpeedCard : Card {
    ReloadSpeedCard() : Card("reload_speed+100%") {}

    void execute() override;
};

struct AccuracyCard : Card {
    AccuracyCard() : Card("accuracy+50%") {}

    void execute() override;
};

struct FlashCooldownCard : Card {
    FlashCooldownCard() : Card("flash_cooldown_speed+50%") {}

    void execute() override;
};

struct TowerCard : Card {
    TowerCard() : Card("tower+1") {}

    void execute() override;
};

class CardFactory : public Singleton<CardFactory> {
public:
    CardFactory();

    template <typename CardType>
    CardType create() const {
        return static_cast<CardType>(cards_map_.at(typeid(CardType)));
    }

    std::shared_ptr<Card> get(size_t idx) const;

    size_t size() const { return cards_map_.size(); }

private:
    std::unordered_map<std::type_index, std::shared_ptr<Card>> cards_map_;
};

}  // namespace wheel
