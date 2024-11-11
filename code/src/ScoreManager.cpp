#include "ScoreManager.hpp"

#include "Constants.hpp"

ScoreManager::ScoreManager(TextureManager& texture_manager) 
    : texture_manager_(texture_manager)
    , position_(ScoreParamenters::kPosition) {
    UpdateScoreValue(0);
    LoadNumberTextures();
}

void ScoreManager::UpdateScoreValue(int val) {
    score_ = val;
    score_string_ = std::to_string(score_);
    position_.x = (static_cast<float>(kGameWidth) * 0.5f - 
                  (static_cast<float>(score_string_.size()) * ScoreParamenters::kTextureDimensions.x * 0.5f));
}
    
void ScoreManager::IncreaseScore(int val) {
    UpdateScoreValue(score_ + val);
}


void ScoreManager::LoadNumberTextures() {
    for (std::size_t i = 0; i < number_textures_.size(); ++i) {
        // number_textures_[i] = texture_manager_.LoadTexture(kAssetsFolderImages + std::to_string(i) + ".png");
    }
}

void ScoreManager::Render(SDL_Renderer& renderer) {
    float current_x = position_.x;
    /*for (auto& c : score_string_) {
        int num = c - '0';
        SDL_FRect destination {
            current_x,
            position_.y,
            ScoreParamenters::kTextureDimensions.x,
            ScoreParamenters::kTextureDimensions.y};
        SDL_RenderCopyF(&renderer, number_textures_[num], nullptr, &destination);
        
        current_x += ScoreParamenters::kTextureDimensions.x;
    }*/
}

void ScoreManager::Reset() {
    UpdateScoreValue(0);
}
