#include <game/card.hpp>

#include <wheel/json.hpp>

#include <game/global.hpp>
#include <game/event.hpp>
#include <game/entity_manager.hpp>

#include <game/component/hp.hpp>
#include <game/component/self.hpp>
#include <game/component/perk.hpp>

namespace wheel {

void Card::use() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    for (auto buff : buffs_) {
        buff(entity);
    }
}

CardManager::CardManager() {
    JsonListType obj = Json::parse_file(game_resource.path + "/player_card.json");
    for (JsonDictType& card_obj : obj) {
        std::string name = card_obj["name"];
        std::vector<Buff> buffs;
        for (JsonDictType& buff_obj : static_cast<JsonListType&>(card_obj["buffs"])) {
            std::string buff_name = buff_obj["name"];
            int value = buff_obj["value"];
            auto buff = BuffManager::instance().create(buff_name, value);
            buffs.emplace_back(buff);
        }
        player_cards_.emplace_back(std::make_shared<Card>(name, buffs));
    }
}

// std::shared_ptr<Card> CardFactory::get(size_t idx) const {
//     std::vector<std::shared_ptr<Card>> cards;
//     return cards[idx];
// }

}  // namespace wheel
