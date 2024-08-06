#pragma once

#include <memory>
#include <string>
#include <vector>

#include <wheel/singleton.hpp>

#include <game/card.hpp>

namespace wheel {

class CardManager : public Singleton<CardManager> {
public:
    CardManager();

    std::vector<Card*> get_random_cards(bool player, int num = 3);
    std::vector<Card*> get_random_player_cards(int num = 3);
    std::vector<Card*> get_random_enemy_cards(int num = 3);

    void parse_player_card(const std::string& path);
    void parse_enemy_card(const std::string& path);

    static SDL_Color rarity2color(int rarity);

private:
    std::vector<std::shared_ptr<Card>> player_cards_[6];
    std::vector<std::shared_ptr<Card>> enemy_cards_;

    static inline SDL_Color rarity_colors_[6] = {
        sdl.GRAY,
        sdl.WHITE,
        sdl.GREEN,
        sdl.BLUE,
        sdl.PURPLE,
        sdl.ORANGE
    };
    static inline int rarity_distribution_[6] = {
        0,
        50,
        40,
        30,
        20,
        10
    };
};

}  // namespace wheel
